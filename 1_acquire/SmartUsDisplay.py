"""
SmartUsDisplay.py - configuration-driven B-mode ultrasound display & recorder.

No GUI scan controls and NO RF data (the connected SmartUS probe does not
provide RF access). All scan settings are read from the .env file next to
this script.

Keyboard shortcuts:
    i   save a B-mode snapshot (PNG) immediately
    v   toggle video recording (MP4) on / off
    q   quit  (Esc also quits)

Run:
    python SmartUsDisplay.py
"""
import os
import sys
import time
import ctypes
from ctypes import *
from datetime import datetime

import numpy as np
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
from tkinter import Tk, Label
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import cv2
from dotenv import load_dotenv

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
load_dotenv(os.path.join(BASE_DIR, '.env'))


# ----------------------- .env helpers -----------------------
def _get(name):
    v = os.getenv(name)
    if v is None:
        return None
    v = v.strip()
    return v if v else None

def env_int(name, default=None):
    v = _get(name)
    if v is None:
        return default
    try:
        return int(v)
    except ValueError:
        return default

def env_float(name, default):
    v = _get(name)
    if v is None:
        return default
    try:
        return float(v)
    except ValueError:
        return default

def env_bool(name, default=False):
    v = _get(name)
    if v is None:
        return default
    return v.lower() in ('1', 'true', 'yes', 'on')


# ----------------------- configuration -----------------------
W            = env_int('IMAGE_WIDTH', 512)          # B image width
H            = env_int('IMAGE_HEIGHT', 512)        # B image height
WINDOW_TITLE = _get('WINDOW_TITLE') or 'SmartUS B-mode Display'

GAIN         = env_int('GAIN', 70)                 # B gain index (0-90)
POWER        = env_int('POWER', 20)                # B power index (0-20 dB)
TGC_PCT      = [env_int(f'TGC{i}_PCT', d) for i, d in zip(range(1, 6), (50, 60, 60, 60, 60))]

# Optional scan settings - leave empty in .env to keep the device default.
DEPTH_MM         = env_int('DEPTH_MM')             # e.g. 60
FOCUS_DEPTH_MM   = env_int('FOCUS_DEPTH_MM')       # e.g. 30
DYNAMIC_RANGE_DB = env_int('DYNAMIC_RANGE_DB')     # e.g. 60
LINES_DENSITY    = env_int('LINES_DENSITY_IDX')    # 8 / 16 / 22 / 24 / 32
VIEW_AREA_PCT    = env_int('VIEW_AREA_PCT')        # 50-100
SCAN_TYPE        = env_int('SCAN_TYPE', 0)         # 0=B Standard, 1=B WideView, 2=B Compound
WIDE_VIEW_ANGLE_DEG = env_int('WIDE_VIEW_ANGLE_DEG')   # used when SCAN_TYPE=1
COMPOUND_ANGLE_DEG  = env_int('COMPOUND_ANGLE_DEG')    # used when SCAN_TYPE=2
COMPOUND_FRAMES     = env_int('COMPOUND_FRAMES')       # used when SCAN_TYPE=2

RECORD_IMAGE   = env_bool('RECORD_IMAGE', False)
IMAGE_INTERVAL = env_float('IMAGE_SAVE_INTERVAL_SEC', 5.0)
RECORD_VIDEO   = env_bool('RECORD_VIDEO', False)
VIDEO_FPS      = env_int('VIDEO_FPS', 25)

OUTPUT_DIR = _get('OUTPUT_DIR') or './output'
if not os.path.isabs(OUTPUT_DIR):
    OUTPUT_DIR = os.path.join(BASE_DIR, OUTPUT_DIR)
IMAGE_DIR = os.path.join(OUTPUT_DIR, 'images')
VIDEO_DIR = os.path.join(OUTPUT_DIR, 'video')
os.makedirs(IMAGE_DIR, exist_ok=True)
os.makedirs(VIDEO_DIR, exist_ok=True)


# ----------------------- DLL loading -----------------------
dll_path = os.path.join(BASE_DIR, 'usgfw2wrapper.dll')
if not os.path.exists(dll_path):
    dll_path = os.path.join(BASE_DIR, 'usgfw2wrapper_C++_sources', 'x64', 'Release', 'usgfw2wrapper.dll')
print('Loading DLL:', dll_path)
usgfw2 = cdll.LoadLibrary(dll_path)

# ----------------------- probe / scan init -----------------------
usgfw2.on_init()
ERR = usgfw2.init_ultrasound_usgfw2()
if ERR == 2:
    print('Main Usgfw2 library object not created')
    sys.exit()

ERR = usgfw2.find_connected_probe()
if ERR != 101:
    print('Probe not detected, ERR =', ERR)
    sys.exit()

ERR = usgfw2.data_view_function()
if ERR < 0:
    print('Main ultrasound scanning object for selected probe not created')
    sys.exit()

ERR = usgfw2.mixer_control_function(0, 0, W, H, 0, 0, 0)
if ERR < 0:
    print('B mixer control not returned')
    sys.exit()

usgfw2.set_callback_scan_converter()


# ----------------------- scan settings -----------------------
def step_to(func, target, *extra_out_ptrs, max_steps=400):
    """Step a PrevNext-style control until it reaches `target`.

    The DLL only exposes +/- stepping for these parameters, so we read the
    current value (direction 0) and step towards the target. Returns True if
    the exact target was reached, False if a hardware boundary was hit first.
    """
    v = c_int(0)
    args = [ctypes.pointer(v)] + list(extra_out_ptrs)
    func(0, *args)                       # read current value
    cur = v.value
    if cur == target:
        return True
    d = 1 if target > cur else -1
    for _ in range(max_steps):
        func(d, *args)
        if v.value == target:
            return True
        if v.value == cur:               # value stopped changing -> boundary
            return False
        cur = v.value
    return False


# --- frequency (read only; device steps are discrete) ---
usgfw2.frequency_control()
freq = c_int(0)
freq_no = c_long(0)
usgfw2.B_FrequencySetPrevNext(0, ctypes.pointer(freq), ctypes.pointer(freq_no))
frequency_mhz = freq.value / 1e6

# --- depth ---
usgfw2.depth_control()
depth = c_int(0)
usgfw2.DepthSetPrevNext(0, ctypes.pointer(depth))
if DEPTH_MM is not None:
    step_to(usgfw2.DepthSetPrevNext, DEPTH_MM)
    usgfw2.DepthSetPrevNext(0, ctypes.pointer(depth))
depth_mm = depth.value

# --- gain / power ---
usgfw2.gain_control()
gain = c_int(0)
usgfw2.B_GainSetByIdx(GAIN, ctypes.pointer(gain))

usgfw2.power_control()
power = c_int(0)
usgfw2.B_PowerSetByIdx(POWER, ctypes.pointer(power))

# --- TGC ---
usgfw2.TGC_control()
tgc_depths = []
for i, pct in enumerate(TGC_PCT):
    d = c_int(0)
    usgfw2.adjust_TGC(i, pct, ctypes.pointer(d))
    tgc_depths.append(d.value)

# --- focus ---
usgfw2.focus_control()
focal_depth = c_int(0)
focal_zones_count = c_int(0)
focal_zone_idx = c_int(0)
usgfw2.B_FocusSetPrevNext(0, ctypes.pointer(focal_depth),
                          ctypes.pointer(focal_zones_count),
                          ctypes.pointer(focal_zone_idx))
if FOCUS_DEPTH_MM is not None:
    step_to(usgfw2.B_FocusSetPrevNext, FOCUS_DEPTH_MM,
            ctypes.pointer(focal_zones_count), ctypes.pointer(focal_zone_idx))
    usgfw2.B_FocusSetPrevNext(0, ctypes.pointer(focal_depth),
                              ctypes.pointer(focal_zones_count),
                              ctypes.pointer(focal_zone_idx))

# --- dynamic range ---
usgfw2.B_dynamic_range()
dynamic_range = c_int(0)
usgfw2.B_DynamicRangeSetPrevNext(0, ctypes.pointer(dynamic_range))
if DYNAMIC_RANGE_DB is not None:
    step_to(usgfw2.B_DynamicRangeSetPrevNext, DYNAMIC_RANGE_DB)
    usgfw2.B_DynamicRangeSetPrevNext(0, ctypes.pointer(dynamic_range))

# --- lines density ---
usgfw2.lines_density()
lines_density = c_int(0)
usgfw2.B_LinesDensitySetPrevNext(0, ctypes.pointer(lines_density))
if LINES_DENSITY is not None:
    step_to(usgfw2.B_LinesDensitySetPrevNext, LINES_DENSITY)
    usgfw2.B_LinesDensitySetPrevNext(0, ctypes.pointer(lines_density))

# --- view area ---
usgfw2.view_area()
view_area = c_int(0)
usgfw2.B_view_areaSetPrevNext(0, ctypes.pointer(view_area))
if VIEW_AREA_PCT is not None:
    step_to(usgfw2.B_view_areaSetPrevNext, VIEW_AREA_PCT)
    usgfw2.B_view_areaSetPrevNext(0, ctypes.pointer(view_area))

# --- scan type (Standard / WideView / Compound) + related parameters ---
usgfw2.scan_type_control()
usgfw2.turn_on_scan_type(SCAN_TYPE)

wide_view_angle = None
compound_angle_val = None
compound_frames_val = None

if SCAN_TYPE == 1 and WIDE_VIEW_ANGLE_DEG is not None:
    usgfw2.wide_view_angle()
    wide_view_angle = c_int(0)
    step_to(usgfw2.WideViewAngleSetPrevNext, WIDE_VIEW_ANGLE_DEG)
    usgfw2.WideViewAngleSetPrevNext(0, ctypes.pointer(wide_view_angle))

if SCAN_TYPE == 2:
    usgfw2.compound_angle()
    compound_angle_val = c_int(0)
    if COMPOUND_ANGLE_DEG is not None:
        step_to(usgfw2.CompoundAngleSetPrevNext, COMPOUND_ANGLE_DEG)
    usgfw2.CompoundAngleSetPrevNext(0, ctypes.pointer(compound_angle_val))

    usgfw2.compound_frames_number()
    compound_frames_val = c_int(0)
    if COMPOUND_FRAMES is not None:
        step_to(usgfw2.CompoundFramesSetPrevNext, COMPOUND_FRAMES)
    usgfw2.CompoundFramesSetPrevNext(0, ctypes.pointer(compound_frames_val))

# --- other controls that must be initialised before Run (per the vendor SDK sample app) ---
usgfw2.steering_angle()
usgfw2.image_orientation()
usgfw2.ChangeScanDirection(0)
usgfw2.UsgQualProp_control()
usgfw2.cine_loop_controls()


# ----------------------- video recorder -----------------------
class VideoRecorder:
    def __init__(self):
        self.writer = None
        self.path = None

    def start(self):
        path = os.path.join(VIDEO_DIR, 'ultrasound_' + datetime.now().strftime('%Y%m%d_%H%M%S') + '.mp4')
        self.writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*'mp4v'), VIDEO_FPS, (W, H))
        self.path = path
        print('Video recording started:', path)

    def write(self, gray_frame):
        if self.writer is not None:
            self.writer.write(cv2.cvtColor(gray_frame, cv2.COLOR_GRAY2BGR))

    def stop(self):
        if self.writer is not None:
            self.writer.release()
            print('Video recording stopped:', self.path)
            p = self.path
            self.writer = None
            return p


# ----------------------- GUI -----------------------
window = Tk()
window.title(WINDOW_TITLE)

# Size the figure (and window) to match the image, not the screen.
fig_dpi  = 90
fig_w_in = W / fig_dpi
fig_h_in = H / fig_dpi
canvas_w = round(fig_w_in * fig_dpi)      # ≈ W px
canvas_h = round(fig_h_in * fig_dpi)      # ≈ H px

win_w = canvas_w + 20                     # image width + side margins
win_h = canvas_h + 95                     # + three top label rows (~80 px) + margin
window.geometry(f'{win_w}x{win_h}')
window.resizable(False, False)

scan_type_name = {0: 'B Standard', 1: 'B WideView', 2: 'B Compound'}.get(SCAN_TYPE, str(SCAN_TYPE))
small_font = ('Arial', 9)
info_line1 = (f'{frequency_mhz} MHz | depth {depth_mm} mm | gain {GAIN}% | '
              f'power {POWER} dB | focus {focal_depth.value} mm')
info_line2 = (f'DR {dynamic_range.value} dB | LD {lines_density.value} | view {view_area.value}% | '
              f'{scan_type_name} | TGC: ' + '/'.join(str(p) for p in TGC_PCT))

info_label1 = Label(window, text=info_line1, font=small_font)
info_label1.place(x=20, y=10)

info_label2 = Label(window, text=info_line2, font=small_font)
info_label2.place(x=20, y=30)

status_label = Label(window, text='', fg='blue', font=small_font)
status_label.place(x=20, y=52)

fps_label = Label(window, text='FPS = ', font=small_font)
fps_label.place(x=win_w - 140, y=52)

fig, ax = plt.subplots(figsize=(fig_w_in, fig_h_in), dpi=fig_dpi)
fig.patch.set_facecolor((0.94, 0.94, 0.94))
ax.set_facecolor((0.94, 0.94, 0.94))

img = ax.imshow(np.zeros((H, W), dtype=np.uint8), cmap='gray', vmin=0, vmax=255)
ax.set_xlabel('Width [px]')
ax.set_ylabel('Depth [px]')

canvas = FigureCanvasTkAgg(fig, window)
canvas.get_tk_widget().place(x=10, y=75)


# ----------------------- snapshot / status -----------------------
last_frame = np.zeros((H, W), dtype=np.uint8)

def save_snapshot(gray):
    name = 'snapshot_' + datetime.now().strftime('%Y%m%d_%H%M%S_%f')[:-3] + '.png'
    path = os.path.join(IMAGE_DIR, name)
    if cv2.imwrite(path, gray):
        print('Snapshot saved:', path)
        status_label.config(text='Image saved: ' + name)
        return True
    return False

def set_status(text):
    status_label.config(text=text)


video = VideoRecorder()
if RECORD_VIDEO:
    video.start()
set_status('Video: ON   |   Auto image: ' + ('ON' if RECORD_IMAGE else 'OFF'))


# ----------------------- key bindings -----------------------
def on_key_i(_event):
    save_snapshot(last_frame)

def on_key_v(_event):
    if video.writer is None:
        video.start()
    else:
        video.stop()
    set_status('Video: ' + ('ON' if video.writer is not None else 'OFF') +
               '   |   Auto image: ' + ('ON' if RECORD_IMAGE else 'OFF'))

def on_closing():
    # Only flag the loop; actual cleanup happens after the loop exits,
    # because key handlers run inside window.update() mid-frame.
    global running
    running = False

window.bind('i', on_key_i)
window.bind('v', on_key_v)
window.bind('q', lambda e: on_closing())
window.bind('<Escape>', lambda e: on_closing())
window.protocol("WM_DELETE_WINDOW", on_closing)


# ----------------------- main loop -----------------------
print('Starting live B-mode imaging.  Keys: [i] snapshot, [v] video toggle, [q] quit')
usgfw2.Run_ultrasound_scanning()

p_array = (ctypes.c_uint * W * H * 4)()
p1 = c_int(0)
p2 = c_float(0.0)

running = True
last_img_save = 0.0

while running:
    t0 = time.time()

    usgfw2.return_pixel_values2(ctypes.pointer(p_array), ctypes.pointer(p1), ctypes.pointer(p2))
    buf = np.frombuffer(p_array, np.uint32)
    last_frame = np.ascontiguousarray(np.reshape(buf, (W, H, 4))[::-1, :, 0], dtype=np.uint8)

    img.set_data(last_frame)

    canvas.draw_idle()
    window.update()

    fps_label.config(text='FPS = ' + str(round(1 / max(time.time() - t0, 1e-6))))

    if RECORD_IMAGE and (time.time() - last_img_save) >= IMAGE_INTERVAL:
        save_snapshot(last_frame)
        last_img_save = time.time()

    video.write(last_frame)

video.stop()
try:
    usgfw2.Close_and_release()
finally:
    window.destroy()
