///////////////////////////////////////////////////////////
#pragma once
#include <dshow.h>
#include "qedit_custom.h"
#include <windows.h>
#include "strmif.h"
#include "Usgfw2_h.h"
#include "Usgfw2_i.c"
#include "resource.h"
#include "usgdatarf.h"
#include <initguid.h>


#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

#ifdef __cplusplus
extern "C" {
#endif

int init_ultrasound_usgfw2();
int find_connected_probe();
int data_view_function();
int mixer_control_function(int left, int top, int right, int bottom, int R, int G, int B);
void return_pixel_values(unsigned int*);
void CreateUsgControl(IUsgDataView* data_view, const IID type_id, ULONG scan_mode, ULONG stream_id, void** ctrl);
void get_resolution(float* delta_x_cm, float* delta_y_cm);
void Freeze_ultrasound_scanning(); 
void Run_ultrasound_scanning();
void Close_and_release(); 
void ReleaseUsgControls(bool final_release);
void on_init();
void Stop_ultrasound_scanning();
void on_probe_button_pressed(int left, int top, int right, int bottom, int R, int G, int B);
void frequency_control();
void B_FrequencySetPrevNext(int dir, int* val_cur_freq, LONG* val_idx2);
LONG get_probe_code();
void depth_control();
void DepthSetPrevNext(int dir, LONG* val_cur_depht);
void gain_control();
void B_GainSetByIdx(int idx, LONG* val_cur_gain);
void power_control();
void B_PowerSetByIdx(int idx, LONG* val_cur_power);
void lines_density();
void B_LinesDensitySetPrevNext(int dir,LONG* val_cur_lines_density);
void view_area();
void B_view_areaSetPrevNext(int dir, LONG* val_cur_view_area);
void focus_control();
void B_FocusSetPrevNext(int dir, LONG* val_cur_focus, LONG* focal_zones_cnt, LONG* focal_zone_idx);
int steering_angle();
void B_SteeringAngleSetPrevNext(int dir, LONG* val_angle);
void image_orientation();
void ChangeScanDirection(int val);
void wide_view_angle();
void WideViewAngleSetPrevNext(int dir, LONG* val_angle_wide_view);
void turn_on_scan_type(int type_ID);
void scan_type_control();
void compound_angle();
void compound_frames_number();
void CompoundFramesSetPrevNext(int dir, LONG* val_frames);
void CompoundAngleSetPrevNext(int dir, LONG* val_angle_comp);
void adjust_TGC(int ctrl_idx, int val_idx, LONG* tgc_depth_value);
void TGC_control();
void B_DynamicRangeSetPrevNext(int dir, LONG* value_dynamic_range);
void B_dynamic_range();
void return_pixel_values2(unsigned int* color, int* frame_num, double* time_stamp);
void set_callback_scan_converter();
void set_callback_Sample_Grabber();
void return_RF_data(INT16* RF_Data, int elements_number, int* frame_numRF, double* time_stamp, int* RF_lines_num, int* RF_samples_num);
void RF_data_controls();
void RF_window_get(LONG* depth1, LONG* depth2, LONG* line1, LONG* line2, int* Lines_number);
void RF_window_set(LONG depth1, LONG depth2, LONG line1, LONG line2);
void get_sampling_period(int* Discretization_period);
void convert_units_to_pixels(int Line1, int m_nSubFrameIndex, int Depth1, int Depth2, float* X1pix, float* Y1pix, float* X2pix, float* Y2pix);
void RF_WindowMove(int dx, int dy);
bool B_ChangeScanDirectionGet();
int B_RotationAngleGet();
void RF_WindowSize(int dx, int dy);
void RF_lines_get(LONG* line1, LONG* line2);
void CompoundSubframeSetPrevNext(int dir, int* SubFrameIndex);
void cine_loop_controls();
void get_cine_interval(INT64* position, INT64* start, INT64* end);
void get_cine_frame(int position);
void UsgQualProp_control();
void start_write_RF_to_bin_file(int filename[], int length, int frames_number);
void stop_write_RF_to_bin_file();
void RF_Data_source_ID(int number);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif


bool wideview_is_supported;
bool compound_is_supported;
bool compound_frames_enabled;
int tgc_controls_count;


IUsgfw2* m_usgfw2;			                    // main Usgfw2 library object
IProbe* probe;									// current probe
HRESULT hr;
IUsgDataView* data_view;                        // data view from probe
IUsgDataView* m_data_view;						// data view from probe
IUsgScanMode* mode;                             // scan mode
IUsgMixerControl* mixer_control;				// B mixer control
IUsgImageProperties* image_properties_b;		// B image properties
IUsgProbeFrequency3* b_frequency_ctrl;		// B Frequency
IUsgDepth*				depth_ctrl;				// Depth
IUsgGain*				b_gain_ctrl;			// B Gain
IUsgPower*				b_power_ctrl;           // B Power
IUsgLineDensity*		b_lines_density_ctrl;   // B Lines Density
IUsgViewArea*			view_area_ctrl;			// View Area
IUsgFocus*              b_focus_ctrl;           // B focus 
IUsgSteeringAngle*		b_steering_angle_ctrl;	// B Steering Angle angle (for linear probe)
IUsgImageOrientation*	b_image_orientation_ctrl;	// B Image Orientation
IUsgViewAngle*			wideview_angle_ctrl;	// WideView angle
IUsgScanType*			scan_type_ctrl;			// Scanning type (standard/trapezoid/compound)
IUsgCompoundAngle*		compound_angle_ctrl;	// Compound angle
IUsgCompoundFrames*		compound_frames_ctrl;	// Compound frames number
IUsgTgc*				tgc_ctrl;				// Time Gain Compensation (TGC)
IUsgDynamicRange*		b_dynamic_range_ctrl;	// B Dynamic Range
IUsgScanLineProperties2* scan_line_properties_b;
IUsgCineStream* m_pUsgCine;						// cine control interface
IUsgCineStream* m_pUsgCineB;					// B mode cine control interface
IUsgCineStream* m_pUsgCineRF;					// RF mode cine control interface
IUsgCineSink* m_pUsgCineSink;					// cine buffer control interface
FILE *rf_data_file_handle;          // RF data file handle
IUsgQualProp* m_pUsgQualProp;					// interface for B mode FPS (frames per second)
int rf_data_source_point_id;		// identifier of RF data source
int number_of_KBs;
int temp_RF_ID;

char* rf_data_file_name;
bool write_RF;
bool first_frame;
int number_of_frames_to_record;
int ScanBytes(long w, int bpp);
void SetRFDataI32_full_frame(PULTRASOUNDRF_INFO pFrameInfo, PBYTE pBuffer, int flag, int nSubFrameIndex);


