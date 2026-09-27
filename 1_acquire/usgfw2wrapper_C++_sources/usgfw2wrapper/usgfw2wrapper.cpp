#pragma once
#include <windows.h>
#include <stdio.h>
#include <sysinfoapi.h>
#include "stdafx.h"
#include "math.h"
#include "usgfw2wrapper.h"
#include <conio.h>
#include "DIBSectn.h"
#include "UsgfwWindow.h"
#include "usgstr.h"
#include "usgstrrf.h"
#include "usgdatarf.h"
#include "ScanConverterCallback.h"
#include "SampleGraberCallback.h"
#include <mtype.h>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std::chrono;

CUsgfwWindow* p_wnd;
HWND hWnd; // ultrasound output window
volatile int frames_counter;
volatile int RF_frames_counter;
int frames_count_previuos;
int RF_frames_count_previuos;
unsigned int* buffer_of_image_data;
INT16* buffer_of_RF_data_frame;
int previous_height = -1;
int previous_width = -1;
int RF_data_buffer_size;
int RF_samples_per_beam;
int RF_scanning_lines;
volatile int Sampling_period;
volatile int flag = 0;

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		CUsgfwWindow::RegisterClass((HINSTANCE)hModule);
	}
	return TRUE;
}

void RF_Data_source_ID(int point_id)
{
	int source_id = 3;
	if (point_id == 1)
		source_id = 3;
	else if (point_id == 2)
		source_id = 2;
	else if (point_id == 3)
		source_id = 1;
	else if (point_id == 4)
		source_id = 0;
	else
		return;

	if (m_pUsgDataSourceRF)
		m_pUsgDataSourceRF->put_Current(source_id);

	rf_data_source_point_id = point_id;
}

void cine_loop_controls()
{
	// cine control
	IUnknown* tmp_obj = NULL;
	m_pUsgCine = NULL;
	m_pUsgCineB = NULL;
	m_pUsgCineRF = NULL;
	if (m_pUsgWindowRF != NULL)
		using_rf_stream = true;
	else
		using_rf_stream = false;

	CreateUsgControl(m_data_view, IID_IUsgCineStream, SCAN_MODE_RFB, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
	{
		m_pUsgCineRF = (IUsgCineStream*)tmp_obj;
		m_pUsgCineRF->put_TimeFormat(CINE_FORMAT_FRAME);
	}
	tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgCineStream, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
	{
		m_pUsgCineB = (IUsgCineStream*)tmp_obj;
		// for cine control we will use frames-based format
		m_pUsgCineB->put_TimeFormat(CINE_FORMAT_FRAME);
	}

	m_pUsgCine = m_pUsgCineRF;

	// cine sink control
	tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgCineSink, SCAN_MODE_RFB, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		m_pUsgCineSink = (IUsgCineSink*)tmp_obj;
	else
		m_pUsgCineSink = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void UsgQualProp_control()
{
	// control to get B FPS
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgQualProp, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		m_pUsgQualProp = (IUsgQualProp*)tmp_obj;
	else
		m_pUsgQualProp = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}
void get_cine_interval(INT64* position, INT64* start, INT64* end)
{
	do
	{
		if (!m_pUsgCine)
			break;

		CINE_STATE state = CINE_STATE_PLAY;
		m_pUsgCine->get_State(&state);
		if (state == CINE_STATE_PLAY)
			m_pUsgCine->put_State(CINE_STATE_STOP);

		m_pUsgCine->get_CurrentPosition(position);

		// get continuous interval (without time gaps)
		m_pUsgCine->GetContinuousInterval(*position, start, end);
	}

	while (false);
}

void get_cine_frame(int position)
{
	if (!m_pUsgCine)
		return;

	RF_frames_counter = 0;
	RF_frames_count_previuos = 0;
	frames_counter = 0;
	frames_count_previuos = 0;

	m_pUsgCine->put_CurrentPosition(position);
}

void start_write_RF_to_bin_file(int filename[], int length, int frames_number)
{
	write_RF = true;
	first_frame = true;
	number_of_frames_to_record = frames_number;

	rf_data_file_name = new char[length];
	for (int i = 0; i < length; i++)
		rf_data_file_name[i] = (char)filename[i];

	rf_data_file_handle = fopen(rf_data_file_name, "wb");

}

void stop_write_RF_to_bin_file()
{
	write_RF = false;
	if (rf_data_file_handle)
		fclose(rf_data_file_handle);
}

void SetRFDataI32_full_frame(PULTRASOUNDRF_INFO pFrameInfo, PBYTE pBuffer, int flag, int nSubFrameIndex) //, int* number_of_frames_to_record
{

	if (rf_data_file_handle == NULL)
		return;

	if (first_frame == true)
		fwrite("RF0004", sizeof(char), 6, rf_data_file_handle);

	first_frame = false;

	// sampling period (25 ns)
	long int sampling_period = pFrameInfo->SamplingTime;
	// number of samples in a row
	long int length = pFrameInfo->UsgFrameInfoHeader.SamplesPerChannel - 2; // -2 to drop 4 bytes for time stamp 
	LONG nStartLine, nEndLine, nStartDepth, nEndDepth;

	m_pUsgWindowRF->GetWindow(&nStartLine, &nEndLine, &nStartDepth, &nEndDepth);
	// number of RF rows
	long int number_of_RF_rows = nEndLine - nStartLine + 1;
	// Tx frequency
	long int tx_frequency;
	FREQUENCY3 freq;
	b_frequency_ctrl->get_Current(&freq);
	tx_frequency = freq.nFrequency;
	// Average frame rate
	long int AvgFPS;
	m_pUsgQualProp->get_AvgFrameRate(&AvgFPS); // B mode properties

	long int no_of_bits = pFrameInfo->UsgFrameInfoHeader.SampleBitCount;

	fwrite(&number_of_frames_to_record, sizeof(int), 1, rf_data_file_handle);

	// write size of header and RF data frame in bytes
	int header_size = 11 * 4 + number_of_RF_rows * 3 * 4 + number_of_RF_rows * 4;
	int rf_frame_size = 2 * number_of_RF_rows * length;
	if (rf_data_source_point_id == 4)
		rf_frame_size = rf_frame_size * 2;

	fwrite(&header_size, sizeof(int), 1, rf_data_file_handle);
	fwrite(&rf_frame_size, sizeof(int), 1, rf_data_file_handle);

	fwrite(&rf_data_source_point_id, sizeof(int), 1, rf_data_file_handle);

	fwrite(&tx_frequency, sizeof(int), 1, rf_data_file_handle);
	fwrite(&AvgFPS, sizeof(int), 1, rf_data_file_handle);
	fwrite(&length, sizeof(int), 1, rf_data_file_handle);
	fwrite(&number_of_RF_rows, sizeof(int), 1, rf_data_file_handle);
	fwrite(&sampling_period, sizeof(int), 1, rf_data_file_handle);
	fwrite(&no_of_bits, sizeof(int), 1, rf_data_file_handle);
	// start depth of RF window
	fwrite(&nStartDepth, sizeof(int), 1, rf_data_file_handle);

	int row_start, row_end, row_step;
	LONG val;
	b_image_orientation_ctrl->get_Mirror(&val);
	if (val == 0)
	{
		row_start = 0;
		row_end = number_of_RF_rows;
		row_step = 1;
	}
	else
	{
		row_start = number_of_RF_rows - 1;
		row_end = -1;
		row_step = -1;
	}

	int j;
	j = row_start;
	while (j != row_end)
	{
		int beam_x = ((int)pFrameInfo->UsgBeamVector[j + nSubFrameIndex * number_of_RF_rows].rpVector.StartPoint.x);
		int beam_y = ((int)pFrameInfo->UsgBeamVector[j + nSubFrameIndex * number_of_RF_rows].rpVector.StartPoint.y);
		int angle = (int)(1000000 * (double)pFrameInfo->UsgBeamVector[j + nSubFrameIndex * number_of_RF_rows].rpVector.Direction.value * (double)pFrameInfo->UsgBeamVector[j + nSubFrameIndex * number_of_RF_rows].rpVector.Direction.ScaleFactor.Numerator / (double)pFrameInfo->UsgBeamVector[j + nSubFrameIndex * number_of_RF_rows].rpVector.Direction.ScaleFactor.Denominator);
		if (val == 1)
		{
			beam_x = -beam_x;
			angle = -angle;
		}
		fwrite(&beam_x, sizeof(int), 1, rf_data_file_handle);
		fwrite(&beam_y, sizeof(int), 1, rf_data_file_handle);
		fwrite(&angle, sizeof(int), 1, rf_data_file_handle);

		j += row_step;
	}

	// writes time stamps to RF data files
	if (rf_data_source_point_id != 4)
	{
		WORD A[2];
		j = row_start;
		while (j != row_end)
		{
			int nOffSet_full = pFrameInfo->UsgFrameInfoHeader.Pitch * j;
			INT16* arr = (INT16*)(pBuffer + nOffSet_full);
			INT16* pData = arr;
			for (int i = 0; i < 2; i++)
				A[i] = (WORD)pData[i];

			unsigned int time_stamps = A[1] * 256 * 256 + A[0];

			if (rf_data_file_handle != NULL)
				fwrite(&time_stamps, sizeof(unsigned int), 1, rf_data_file_handle);

			j += row_step;
		}
	}
	// for rf_data_source_point_id = 4 
	else
	{
		WORD A[2];
		j = row_start;
		while (j != row_end)
		{
			int nOffSet_full = pFrameInfo->UsgFrameInfoHeader.Pitch * j;
			INT16* arr = (INT16*)(pBuffer + nOffSet_full);
			INT16* pData = arr;
			INT16* pData1 = arr + 1;

			A[0] = (WORD)pData[0];
			A[1] = (WORD)pData1[0];

			unsigned int time_stamps = A[1] * 256 * 256 + A[0];

			if (rf_data_file_handle != NULL)
				fwrite(&time_stamps, sizeof(unsigned int), 1, rf_data_file_handle);

			j += row_step;
		}
	}

	if (rf_data_source_point_id == 4)
	{
		j = row_start;
		while (j != row_end)
		{
			int nOffSet_full = pFrameInfo->UsgFrameInfoHeader.Pitch * j;
			INT16* arr = (INT16*)(pBuffer + nOffSet_full);
			INT16* pData = arr + 2;
			for (int i = 0; i < length; i++)
			{
				if (rf_data_file_handle != NULL)
					fwrite(pData, sizeof(INT16), 1, rf_data_file_handle);
				pData += flag;
			}
			j += row_step;
		}

		j = row_start;
		while (j != row_end)
		{
			int nOffSet_full = pFrameInfo->UsgFrameInfoHeader.Pitch * j;
			INT16* arr = (INT16*)(pBuffer + nOffSet_full);
			INT16* pData = (arr + 3);
			for (int i = 0; i < length; i++)
			{
				if (rf_data_file_handle != NULL)
					fwrite(pData, sizeof(INT16), 1, rf_data_file_handle);
				pData += flag;
			}
			j += row_step;
		}
	}
	else
	{
		j = row_start;
		while (j != row_end)
		{
			int nOffSet_full = pFrameInfo->UsgFrameInfoHeader.Pitch * j;
			INT16* arr = (INT16*)(pBuffer + nOffSet_full);
			INT16* pData = arr + 2;
			for (int i = 0; i < length; i++)
			{
				if (rf_data_file_handle != NULL)
					fwrite(pData, sizeof(INT16), 1, rf_data_file_handle);
				pData += flag;
			}
			j += row_step;
		}
	}
}

void ScanConverterCallback::SetUsgWnd(HWND wnd, int x, int y)
{
	m_usg_wnd = wnd;
	m_usg_x = x;
	m_usg_y = y;
}

void on_init()
{
	m_usgfw2 = NULL;
	write_RF = false;
	compound_angle_ctrl = NULL;
	compound_frames_ctrl = NULL;
	b_dynamic_range_ctrl = NULL;
	tgc_ctrl = NULL;
	b_image_orientation_ctrl = NULL;
	scan_type_ctrl = NULL;
	wideview_angle_ctrl = NULL;
	probe = NULL;
	hWnd = NULL;
	hr = S_OK;
	data_view = NULL;
	m_data_view = NULL;
	mode = NULL;
	mixer_control = NULL;
	image_properties_b = NULL;
	b_frequency_ctrl = NULL;
	depth_ctrl = NULL;
	b_gain_ctrl = NULL;
	b_power_ctrl = NULL;
	b_lines_density_ctrl = NULL;
	view_area_ctrl = NULL;
	b_focus_ctrl = NULL;
	b_steering_angle_ctrl = NULL;
	tgc_controls_count = 5;
	p_wnd = NULL;
	m_pUsgWindowRF = NULL;
	m_pUsgDataSourceRF = NULL;
	frames_counter = 0;
	frames_count_previuos = -1;
	m_sample_grabber_plugin = NULL;
	m_sample_grabber_callback = NULL;
	RF_frames_counter = 0;
	RF_frames_count_previuos = -1;
	RF_data_buffer_size = 0;
	RF_samples_per_beam = 0;
	RF_scanning_lines = 0;
	buffer_of_RF_data_frame = NULL;
	Sampling_period = -1;
	scan_line_properties_b = NULL;
	m_nSubFrameIndex = 0;
	m_scan_converter_callback = NULL;
	sample_grabber_filter = NULL;
	m_pUsgCineB = NULL;
	m_pUsgCineRF = NULL;
	m_pUsgCine = NULL;
	m_pUsgCineSink = NULL;
	rf_data_file_handle = NULL;
	m_pUsgQualProp = NULL;
	temp_RF_ID = 1;
}

int init_ultrasound_usgfw2()
{
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_Usgfw2, NULL, CLSCTX_INPROC_SERVER, IID_IUsgfw2, (LPVOID*)&m_usgfw2);
	if (hr != S_OK)
	{
		m_usgfw2 = NULL;
		return 2;
	}
	else
	{
		return 3;
	}
}
void CompoundSubframeSetPrevNext(int dir, int* SubFrameIndex)
{
	int val;
	val = m_nSubFrameIndex;
	if (dir > 0)
		val++;
	if (dir < 0)
		val--;

	LONG val_cur;

	if (compound_frames_ctrl)
		compound_frames_ctrl->get_Current(&val_cur);
	else
		return;

	if (val >= val_cur)
		val = val_cur - 1;
	if (val < 0)
		val = 0;
	*SubFrameIndex = val;
	m_nSubFrameIndex = val;
}

void RF_window_get(LONG* depth1, LONG* depth2, LONG* line1, LONG* line2, int* Lines_number)
{
	if (!m_pUsgWindowRF)
		return;

	m_pUsgWindowRF->GetWindow(line1, line2, depth1, depth2);
	*Lines_number = (*line2 - *line1 + 1);
}

void RF_window_set(LONG depth1, LONG depth2, LONG line1, LONG line2)
{

	VALUE_STEPPING LinesRange = { 0,0,0 };
	VALUE_STEPPING DepthsRange = { 0,0,0 };
	if (m_pUsgWindowRF)
	{
		m_pUsgWindowRF->get_LinesRange(&LinesRange);
		m_pUsgWindowRF->get_DepthsRange(&DepthsRange);
	}
	else
		return;

	// if new values are out of range - return
	if (line1 < LinesRange.minimum) return;
	if (line2 > LinesRange.maximum) return;

	if (depth1 < DepthsRange.minimum) return;
	if (depth2 > DepthsRange.maximum) return;

	// set new RF window size
	m_pUsgWindowRF->SetWindow(line1, line2, depth1, depth2);
}


void RF_lines_get(LONG* line1, LONG* line2)
{
	VALUE_STEPPING LinesRange = { 0,0,0 };

	if (m_pUsgWindowRF)
		m_pUsgWindowRF->get_LinesRange(&LinesRange);
	else
		return;

	*line1 = (LONG)LinesRange.minimum;
	*line2 = (LONG)LinesRange.maximum;
}

void RF_WindowMove(int dx, int dy)
{
	if (!m_pUsgWindowRF)
		return;

	LONG depth1, depth2;
	LONG line1, line2;

	// get current RF window size
	m_pUsgWindowRF->GetWindow(&line1, &line2, &depth1, &depth2);

	int dx2, dy2;
	dx2 = dx;
	dy2 = dy;

	bool is_mirror = B_ChangeScanDirectionGet();
	int r = B_RotationAngleGet();
	if (is_mirror == false)
	{
		if (r == 0)
		{
			dx2 = dx;
			dy2 = dy;
		}
		else if (r == 90)
		{
			dx2 = -dy;
			dy2 = dx;
		}
		else if (r == 180)
		{
			dx2 = -dx;
			dy2 = -dy;
		}
		else if (r == 270)
		{
			dx2 = dy;
			dy2 = -dx;
		}
	}
	else
	{
		if (r == 0)
		{
			dx2 = -dx;
			dy2 = dy;
		}
		else if (r == 90)
		{
			dx2 = dy;
			dy2 = dx;
		}
		else if (r == 180)
		{
			dx2 = dx;
			dy2 = -dy;
		}
		else if (r == 270)
		{
			dx2 = -dy;
			dy2 = -dx;
		}
	}

	// calculate new RF window size
	line1 += dx2;
	line2 += dx2;
	depth1 += dy2;
	depth2 += dy2;
	VALUE_STEPPING LinesRange = { 0,0,0 };
	VALUE_STEPPING DepthsRange = { 0,0,0 };

	if (m_pUsgWindowRF)
	{
		m_pUsgWindowRF->get_LinesRange(&LinesRange);
		m_pUsgWindowRF->get_DepthsRange(&DepthsRange);
	}
	else
		return;

	// if new values are out of range - return
	if (line1 < LinesRange.minimum) return;
	if (line2 > LinesRange.maximum) return;

	if (depth1 < DepthsRange.minimum) return;
	if (depth2 > DepthsRange.maximum) return;

	// set new RF window size
	m_pUsgWindowRF->SetWindow(line1, line2, depth1, depth2);

}

void RF_WindowSize(int dx, int dy)
{
	if (!m_pUsgWindowRF)
		return;

	LONG depth1, depth2;
	LONG line1, line2;

	// get current RF window size
	m_pUsgWindowRF->GetWindow(&line1, &line2, &depth1, &depth2);

	int dx2, dy2;
	dx2 = dx;
	dy2 = dy;

	bool is_mirror = B_ChangeScanDirectionGet();
	int r = B_RotationAngleGet();
	if (is_mirror == false)
	{
		if (r == 0)
		{
			dx2 = dx;
			dy2 = dy;
		}
		else if (r == 90)
		{
			dx2 = dy;
			dy2 = dx;
		}
		else if (r == 180)
		{
			dx2 = dx;
			dy2 = dy;
		}
		else if (r == 270)
		{
			dx2 = dy;
			dy2 = dx;
		}
	}
	else
	{
		if (r == 0)
		{
			dx2 = dx;
			dy2 = dy;
		}
		else if (r == 90)
		{
			dx2 = dy;
			dy2 = dx;
		}
		else if (r == 180)
		{
			dx2 = dx;
			dy2 = dy;
		}
		else if (r == 270)
		{
			dx2 = dy;
			dy2 = dx;
		}
	}

	// calculate new RF window size
	line1 -= dx2;
	line2 += dx2;
	depth1 -= dy2;
	depth2 += dy2;

	VALUE_STEPPING WidthRange;
	VALUE_STEPPING VolumeRange;
	m_pUsgWindowRF->get_WidthRange(&WidthRange);
	m_pUsgWindowRF->get_VolumesRange(&VolumeRange);

	VALUE_STEPPING LinesRange;
	VALUE_STEPPING DepthsRange;
	m_pUsgWindowRF->get_LinesRange(&LinesRange);
	m_pUsgWindowRF->get_DepthsRange(&DepthsRange);

	if (line1 < LinesRange.minimum)
		line1 = LinesRange.minimum;
	else if (line1 > LinesRange.maximum)
		line1 = LinesRange.maximum;

	if (line2 < LinesRange.minimum)
		line2 = LinesRange.minimum;
	else if (line2 > LinesRange.maximum)
		line2 = LinesRange.maximum;

	if (depth1 < DepthsRange.minimum)
		depth1 = DepthsRange.minimum;
	else if (depth1 > DepthsRange.maximum)
		depth1 = DepthsRange.maximum;

	if (depth2 < DepthsRange.minimum)
		depth2 = DepthsRange.minimum;
	else if (depth2 > DepthsRange.maximum)
		depth2 = DepthsRange.maximum;

	if ((line2 - line1) < WidthRange.minimum) return;
	if ((line2 - line1) > WidthRange.maximum) return;
	if ((depth2 - depth1) < VolumeRange.minimum) return;
	if ((depth2 - depth1) > VolumeRange.maximum) return;

	// set new RF window size
	m_pUsgWindowRF->SetWindow(line1, line2, depth1, depth2);

}

bool B_ChangeScanDirectionGet()
{
	if (b_image_orientation_ctrl == NULL)
		return false;

	LONG val;
	b_image_orientation_ctrl->get_Mirror(&val);
	if (val == 1)
		return true;
	return false;
}

int B_RotationAngleGet()
{
	if (b_image_orientation_ctrl == NULL)
		return 0;
	tagImageRotate val;
	b_image_orientation_ctrl->get_Rotate(&val);
	if (val == IMAGE_ROTATE_NONE)
		return 0;
	else if (val == IMAGE_ROTATE_90CCW)
		return 90;
	else if (val == IMAGE_ROTATE_180CCW)
		return 180;
	else if (val == IMAGE_ROTATE_270CCW)
		return 270;
	return 0;
}

void get_sampling_period(int* Discretization_period)
{
	*Discretization_period = 25;
}

void convert_units_to_pixels(int Line1, int subframe_idx, int Depth1, int Depth2, float* X1pix, float* Y1pix, float* X2pix, float* Y2pix)
{
	float X1 = 0;
	float Y1 = 0;
	float X2 = 0;
	float Y2 = 0;

	if (scan_line_properties_b)
		scan_line_properties_b->GetCoordinates4(Line1, subframe_idx, &X1, &Y1, &X2, &Y2, (FLOAT)Depth1 / 1000., (FLOAT)Depth2 / 1000.);

	if (image_properties_b)
	{
		image_properties_b->UnitsToPixels(X1, Y1, X1pix, Y1pix, tagPixelsOrigin::PIXELS_ORIGIN_OUTPUTRECT);
		image_properties_b->UnitsToPixels(X2, Y2, X2pix, Y2pix, tagPixelsOrigin::PIXELS_ORIGIN_OUTPUTRECT);
	}
}

int find_connected_probe()
{
	IUnknown* tmp_obj = NULL;
	IUsgCollection* probes_collection = NULL;

	// get collection of connected probes

	if (m_usgfw2)
		m_usgfw2->get_ProbesCollection(&tmp_obj);
	else
		return 5;

	if (tmp_obj == NULL)
	{
		probes_collection = NULL;
		return 5;
	}

	hr = tmp_obj->QueryInterface(IID_IUsgCollection, (void**)&probes_collection);
	if (tmp_obj)
	{
		tmp_obj->Release();
		tmp_obj = NULL;
	}

	if ((hr != S_OK) || (probes_collection == NULL))
	{
		probes_collection = NULL;
		return 5;
	}

	tmp_obj = NULL;
	if (probes_collection)
	{
		probes_collection->Item(0, &tmp_obj); // get first available probe
		probes_collection->Release();
		probes_collection = NULL;
	}
	else
		return 5;

	if (tmp_obj == NULL)
	{
		probe = NULL;
		return 5;
	}
	hr = tmp_obj->QueryInterface(IID_IProbe, (void**)&probe);
	if (tmp_obj)
	{
		tmp_obj->Release();
		tmp_obj = NULL;
	}

	if ((hr != S_OK) || (probe == NULL))
	{
		probe = NULL;
		return 5;
	}
	else
	{
		return 101;
	}
}

int data_view_function()
{
	// create main ultrasound scanning object for selected probe
	if (m_usgfw2)
		m_usgfw2->CreateDataView(probe, &data_view);
	else
		return -99;

	if (data_view == NULL)
	{
		return -99;
	}

	data_view->QueryInterface(IID_IUsgDataView, (void**)&m_data_view);
	if (m_data_view == NULL)
		return -150;

	m_data_view->put_ScanState(SCAN_STATE_STOP);
	m_data_view->GetScanModeObj(SCAN_MODE_B, &mode);

	if (mode == NULL)
	{
		return -100;
	}
	else
	{
		return 100;
	}
}

void get_resolution(float* delta_x_cm, float* delta_y_cm)
{
	tagImageResolution resolution;
	if (image_properties_b)
	{
		image_properties_b->GetResolution(&resolution, 0);
		*delta_x_cm = 1000 / ((float)resolution.nYPelsPerUnit);
		*delta_y_cm = 1000 / ((float)resolution.nXPelsPerUnit);
	}
	else
	{
		*delta_x_cm = 0;
		*delta_y_cm = 0;
	}
}

void CreateUsgControl(IUsgDataView* data_view, const IID type_id, ULONG scan_mode, ULONG stream_id, void** ctrl)
{
	IUsgControl* ctrl2;
	ctrl2 = NULL;

	if (data_view == NULL) return;

	data_view->GetControlObj(&type_id, scan_mode, stream_id, &ctrl2);

	if (ctrl2 != NULL)
	{
		HRESULT hr;
		hr = ctrl2->QueryInterface(type_id, (void**)ctrl);
		if (hr != S_OK)
			*ctrl = NULL;
		SAFE_RELEASE(ctrl2);
	}
}

int mixer_control_function(int left, int top, int right, int bottom, int R, int G, int B)
{
	p_wnd = new CUsgfwWindow();
	if (p_wnd)
		hWnd = p_wnd->Create(L"Usgfw2Imaging", 0, 0, 0, (right - left), (bottom - top), NULL, 0, 0);

	do
	{
		// get B mixer control //
		if (mode == NULL)
		{
			return -97;
		}
		mode->GetMixerControl(SCAN_MODE_B, 0, &mixer_control);

		if (mixer_control == NULL)
		{
			return -97;
		}

		// set B scanning mode
		if (m_data_view)
			m_data_view->put_ScanMode(SCAN_MODE_B);
		else
			return -97;

		mixer_control->SetOutputWindow((long)hWnd);

		tagRECT rect1;
		rect1.left = left;
		rect1.top = top;
		rect1.right = right;
		rect1.bottom = bottom;

		// set B ultrasound output rectangle
		mixer_control->SetOutputRect(&rect1);

		// set background color that surrounds B ultrasound image
		tagPALETTEENTRY clr1;
		clr1.peRed = R;
		clr1.peGreen = G;
		clr1.peBlue = B;
		clr1.peFlags = 0;
		mixer_control->put_BkColor(clr1);

		// image properties B for obtaining pixel size - Resolution
		IUnknown* tmp_obj = NULL;
		CreateUsgControl(m_data_view, IID_IUsgImageProperties, SCAN_MODE_B, 0, (void**)&tmp_obj);
		if (tmp_obj != NULL)
			image_properties_b = (IUsgImageProperties*)tmp_obj;
		else
			image_properties_b = NULL;

		// scan line properties for B stream
		tmp_obj = NULL;
		CreateUsgControl(m_data_view, IID_IUsgScanLineProperties2, SCAN_MODE_B, 0, (void**)&tmp_obj);
		if (tmp_obj != NULL)
			scan_line_properties_b = (IUsgScanLineProperties2*)tmp_obj;
		else
			scan_line_properties_b = NULL;

		m_data_view->put_ScanState(SCAN_STATE_RUN);

		if (tmp_obj)
			tmp_obj->Release();

		return 89;

	} while (false);
}
void return_pixel_values(unsigned int* color)
{

	HDC hdc;
	HDC hdc1;
	hdc = CreateCompatibleDC(NULL);
	hdc1 = CreateCompatibleDC(hdc);
	HBITMAP hbitmap_handle = NULL;

	if (mixer_control)
		mixer_control->GetCurrentBitmap((LONG*)&hbitmap_handle);
	else
		return;

	LPBITMAPINFO pbmi;
	pbmi = DSGetBITMAPINFOForDIBSection(hbitmap_handle);

	SelectObject(hdc1, hbitmap_handle);

	BYTE* bitPointer = NULL;

	BITMAPINFO bitmap;
	ZeroMemory(&bitmap, sizeof(BITMAPINFO));
	bitmap.bmiHeader.biSize = pbmi->bmiHeader.biSize;
	bitmap.bmiHeader.biWidth = pbmi->bmiHeader.biWidth;
	bitmap.bmiHeader.biHeight = pbmi->bmiHeader.biHeight;
	bitmap.bmiHeader.biPlanes = pbmi->bmiHeader.biPlanes;
	bitmap.bmiHeader.biBitCount = pbmi->bmiHeader.biBitCount;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = pbmi->bmiHeader.biSizeImage;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;

	HBITMAP hBitmap2 = CreateDIBSection(hdc, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);

	SelectObject(hdc, hBitmap2);
	BitBlt(hdc, 0, 0, bitmap.bmiHeader.biHeight, bitmap.bmiHeader.biWidth, hdc1, 0, 0, SRCCOPY);

	for (int i = 0; i < bitmap.bmiHeader.biHeight * bitmap.bmiHeader.biWidth * 4; i++)
		*(color + i) = (unsigned int)*(bitPointer + i);

	pbmi = NULL;
	bitPointer = NULL;
	::DeleteObject(hbitmap_handle);
	::DeleteObject(hBitmap2);
	DeleteDC(hdc);
	DeleteDC(hdc1);
}

void set_callback_scan_converter()
{
	m_scan_converter_callback = new ScanConverterCallback();
	m_scan_converter_callback->AddRef();

	if (!m_scan_converter_callback)
		return;

	// set output window where callback will display received ultrasound frames
	m_scan_converter_callback->SetUsgWnd(hWnd, 20, 20);

	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgScanConverterPlugin, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		scp = (IUsgScanConverterPlugin*)tmp_obj;

	// get IUsgfwScanConverterPlugin interface
	IUnknown* tmp_unk = NULL;
	m_scan_converter_callback->m_plugin = NULL;
	if (scp != NULL)
		scp->get_ScanConverter(&tmp_unk);
	if (tmp_unk != NULL)
	{
		hr = tmp_unk->QueryInterface(IID_IUsgfwScanConverterPlugin, (void**)&m_scan_converter_callback->m_plugin);
		if (hr != S_OK)
			m_scan_converter_callback->m_plugin = NULL;
		SAFE_RELEASE(tmp_unk);
	}
	// register COM callback object and specify method(s) to callback
	if ((m_scan_converter_callback->m_plugin != NULL) && (m_scan_converter_callback != NULL))
	{
		hr = m_scan_converter_callback->m_plugin->SetCallback(m_scan_converter_callback, USPC_BUFFER_INPUT_OUTPUT);
		m_scan_converter_callback->SetPlugin(m_scan_converter_callback->m_plugin);
	}

	if (tmp_obj)
		tmp_obj->Release();
}

void return_pixel_values2(unsigned int* color, int* frame_num, double* time_stamp)
{
	if (previous_height != -1 & previous_width != -1)
	{
		bool frame_num = false;
		while (frame_num == false)
		{
			if (frames_counter > frames_count_previuos && buffer_of_image_data != NULL)
				frame_num = true;
		}

		for (int i = 0; i < previous_height * previous_width * 4; i++)
			*(color + i) = (unsigned int)*(buffer_of_image_data + i);
	}

	uint64_t nanoseconds_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	*time_stamp = (double)nanoseconds_since_epoch;

	frames_count_previuos = frames_counter;
	*frame_num = frames_counter;
}

void return_RF_data(INT16* RF_Data, int elements_number, int* frame_numRF, double* time_stamp, int* RF_lines_num, int* RF_samples_num)
{
	if (RF_samples_per_beam != 0 && RF_scanning_lines != 0 && buffer_of_RF_data_frame != NULL)
	{
		bool frame_num = false;
		while (frame_num == false)
		{
			if (RF_frames_counter > RF_frames_count_previuos)
				frame_num = true;
		}

		int arrSize1 = elements_number;

		int arrSize2;
		if (rf_data_source_point_id != 4)
			arrSize2 = RF_samples_per_beam * RF_scanning_lines;  //std::size(buffer_of_RF_data_frame);
		else
			arrSize2 = RF_samples_per_beam * RF_scanning_lines * 2;


		if (arrSize1 == arrSize2)
		{
			while (flag < 1) { ; }
			if (flag == 1 && rf_data_source_point_id != 4)
				memcpy(RF_Data, buffer_of_RF_data_frame, sizeof(INT16) * RF_samples_per_beam * RF_scanning_lines);
			if (flag == 2 && rf_data_source_point_id == 4)
				memcpy(RF_Data, buffer_of_RF_data_frame, sizeof(INT16) * RF_samples_per_beam * RF_scanning_lines * 2);

		}

		*RF_lines_num = RF_scanning_lines;
		*RF_samples_num = RF_samples_per_beam;

		uint64_t nanoseconds_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		*time_stamp = (double)nanoseconds_since_epoch;

		RF_frames_count_previuos = RF_frames_counter;
		*frame_numRF = RF_frames_counter;
	}
}

void Freeze_ultrasound_scanning()
{
	if (m_data_view == NULL) return;
	// freeze ultrasound scanning
	m_data_view->put_ScanState(SCAN_STATE_FREEZE);

}

void Run_ultrasound_scanning()
{
	if (m_data_view == NULL) return;
	// run ultrasound scanning
	m_data_view->put_ScanState(SCAN_STATE_RUN);


}
void Stop_ultrasound_scanning()
{
	if (m_data_view == NULL) return;
	m_data_view->put_ScanState(SCAN_STATE_STOP);
}
void Close_and_release()
{
	ReleaseUsgControls(1);	// release ultrasound scanning controls
}

void ReleaseUsgControls(bool final_release)
{
	if (p_wnd != NULL) {
		p_wnd->WndProc(WM_CLOSE, 0, 0);
		p_wnd = NULL;
	}

	if (m_data_view != NULL)
		m_data_view->put_ScanState(SCAN_STATE_STOP);
	if (hWnd != NULL)
		hWnd = NULL;

	SAFE_RELEASE(m_pUsgCineB);
	SAFE_RELEASE(m_pUsgCineRF);
	m_pUsgCine = NULL;
	SAFE_RELEASE(m_pUsgCineSink);
	SAFE_RELEASE(scan_line_properties_b);
	SAFE_RELEASE(image_properties_b);
	SAFE_RELEASE(b_dynamic_range_ctrl);
	SAFE_RELEASE(tgc_ctrl);
	SAFE_RELEASE(compound_angle_ctrl);
	SAFE_RELEASE(compound_frames_ctrl);
	SAFE_RELEASE(wideview_angle_ctrl);
	SAFE_RELEASE(scan_type_ctrl);
	SAFE_RELEASE(b_image_orientation_ctrl);
	SAFE_RELEASE(b_steering_angle_ctrl);
	SAFE_RELEASE(b_focus_ctrl);
	SAFE_RELEASE(view_area_ctrl);
	SAFE_RELEASE(b_lines_density_ctrl);
	SAFE_RELEASE(b_power_ctrl);
	SAFE_RELEASE(b_gain_ctrl);
	SAFE_RELEASE(depth_ctrl);
	SAFE_RELEASE(b_frequency_ctrl);
	SAFE_RELEASE(mixer_control);
	SAFE_RELEASE(probe);
	SAFE_RELEASE(mode);
	SAFE_RELEASE(data_view);
	SAFE_RELEASE(m_data_view);
	SAFE_RELEASE(compound_angle_ctrl);
	SAFE_RELEASE(m_pUsgDataSourceRF);
	SAFE_RELEASE(m_pUsgWindowRF);
	SAFE_RELEASE(m_pUsgQualProp);


	if (m_sample_grabber_plugin != NULL)
		m_sample_grabber_plugin->SetCallback(NULL, 0); // unregister callback
	if (m_sample_grabber_callback != NULL)
	{
		//m_sample_grabber_callback->Release();
		delete m_sample_grabber_callback;
		m_sample_grabber_callback = NULL;
	}
	SAFE_RELEASE(sample_grabber_filter);
	SAFE_RELEASE(m_sample_grabber_plugin);

	if (m_scan_converter_callback != NULL) {
		if (m_scan_converter_callback->m_plugin != NULL)
			m_scan_converter_callback->m_plugin->SetCallback(NULL, USPC_BUFFER_INPUT_OUTPUT); // unregister callback
	}
	SAFE_RELEASE(scp);
	//SAFE_RELEASE(m_scan_converter_callback->m_plugin);
	SAFE_RELEASE(m_scan_converter_callback);


	m_nSubFrameIndex = 0;
	delete[] buffer_of_image_data;
	delete[] buffer_of_RF_data_frame;
	buffer_of_RF_data_frame = NULL;
	buffer_of_image_data = NULL;
	frames_counter = 0;
	RF_frames_counter = 0;
	RF_samples_per_beam = 0;
	RF_scanning_lines = 0;

	if (final_release == true)
	{
		SAFE_RELEASE(m_usgfw2);
		CoUninitialize();
	}
}


void on_probe_button_pressed(int left, int top, int right, int bottom, int R, int G, int B)
{

	ReleaseUsgControls(0);
	// on_init();
	find_connected_probe();
	data_view_function();

	mixer_control_function(left, top, right, bottom, R, G, B);

	// update frames counters
	RF_frames_counter = 0;
	RF_frames_count_previuos = -1;
	frames_counter = 0;
	frames_count_previuos = -1;

	// Create controls
	gain_control();
	frequency_control();
	depth_control();
	focus_control();
	view_area();
	power_control();
	lines_density();
	steering_angle();
	image_orientation();
	scan_type_control();
	compound_angle();
	wide_view_angle();
	compound_frames_number();
	compound_angle();
	TGC_control();
	B_dynamic_range();
	RF_data_controls();
	set_callback_scan_converter();
	set_callback_Sample_Grabber();
	UsgQualProp_control();
	cine_loop_controls();
}

LONG get_probe_code()
{
	BSTR tmp_probe_name;
	if (probe)
		probe->get_Name(&tmp_probe_name);
	else
		return 0;

	LONG Code;
	if (m_usgfw2)
		m_usgfw2->ProbeCodeFromName(tmp_probe_name, &Code);
	else
		return 0;

	return Code;

}
void frequency_control()
{
	// create B mode frequency control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgProbeFrequency3, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_frequency_ctrl = (IUsgProbeFrequency3*)tmp_obj;
	else
		b_frequency_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void B_FrequencySetPrevNext(int dir, int* val_cur_freq, LONG* val_idx2)
{
	if (b_frequency_ctrl == NULL)
		return;

	FREQUENCY3 val_cur;
	LONG val_idx;
	LONG val_count;

	b_frequency_ctrl->get_Current(&val_cur);

	*val_cur_freq = val_cur.nFrequency;

	val_idx = -1;

	IUsgValues* usg_values = NULL;
	b_frequency_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		FREQUENCY3* val;
		usg_values->Item(i1, &item);
		val = (FREQUENCY3*)(item.pvRecord);
		if ((val_cur.nFrequency == val->nFrequency) && (val_cur.nThiMode == val->nThiMode))
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			FREQUENCY3* val;
			usg_values->Item(val_idx, &item);
			val = (FREQUENCY3*)(item.pvRecord);
			if ((val_cur.nFrequency != val->nFrequency) || (val_cur.nThiMode != val->nThiMode))
			{
				b_frequency_ctrl->put_Current(*val);
				*val_cur_freq = val->nFrequency;
				val = NULL;
			}

			VariantClear(&item);
		}
	}

	if (val_idx < 0)
		*val_idx2 = 0;
	else if (val_idx > (val_count - 1))
		*val_idx2 = (val_count - 1);
	else
		*val_idx2 = val_idx;

	SAFE_RELEASE(usg_values);

}

void depth_control()
{
	// create depth control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgDepth, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		depth_ctrl = (IUsgDepth*)tmp_obj;
	else
		depth_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void DepthSetPrevNext(int dir, LONG* val_cur_depht)
{
	if (depth_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	depth_ctrl->get_Current(&val_cur);
	*val_cur_depht = val_cur;

	val_idx = -1;

	IUsgValues* usg_values = NULL;
	depth_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			depth_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				depth_ctrl->put_Current(val_cur);
				*val_cur_depht = val_cur;
			}
			VariantClear(&item);
		}
	}

	SAFE_RELEASE(usg_values);

} // void DepthSetPrevNext


void gain_control()
{
	// create B mode gain control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgGain, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_gain_ctrl = (IUsgGain*)tmp_obj;
	else
		b_gain_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}


void B_GainSetByIdx(int idx, LONG* val_cur_gain)
{
	if (b_gain_ctrl == NULL) return;

	LONG val_cur;
	LONG val_count;

	b_gain_ctrl->get_Current(&val_cur);

	*val_cur_gain = val_cur;

	IUsgValues* usg_values = NULL;
	b_gain_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	if (idx >= val_count) idx = val_count - 1;
	if (idx < 0) idx = 0;

	if ((idx >= 0) && (idx < val_count))
	{
		VARIANT item;
		usg_values->Item(idx, &item);
		val_cur = item.lVal;
		LONG val;
		b_gain_ctrl->get_Current(&val);
		if (val != val_cur)
		{
			b_gain_ctrl->put_Current(val_cur);
			*val_cur_gain = val_cur;
		}
		VariantClear(&item);
	}

	SAFE_RELEASE(usg_values);
} // void CsampleDlg::B_GainSetByIdx


void power_control()
{
	// create B mode power control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgPower, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_power_ctrl = (IUsgPower*)tmp_obj;
	else
		b_power_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}


void B_PowerSetByIdx(int idx, LONG* val_cur_power)
{
	if (b_power_ctrl == NULL) return;

	LONG val_cur;
	LONG val_count;

	b_power_ctrl->get_Current(&val_cur);

	*val_cur_power = val_cur;

	IUsgValues* usg_values = NULL;
	b_power_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	if (idx >= val_count) idx = val_count - 1;
	if (idx < 0) idx = 0;

	if ((idx >= 0) && (idx < val_count))
	{
		VARIANT item;
		usg_values->Item(idx, &item);
		val_cur = item.lVal;
		LONG val;
		b_power_ctrl->get_Current(&val);
		if (val != val_cur)
		{
			b_power_ctrl->put_Current(val_cur);
			*val_cur_power = val_cur;
		}
		VariantClear(&item);
	}

	SAFE_RELEASE(usg_values);
} // void CsampleDlg::B_PowerSetByIdx


void lines_density()
{
	// create B mode lines density control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgLineDensity, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_lines_density_ctrl = (IUsgLineDensity*)tmp_obj;
	else
		b_lines_density_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

// increase or decrease B lines density
void B_LinesDensitySetPrevNext(int dir, LONG* val_cur_lines_density)
{
	if (b_lines_density_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	b_lines_density_ctrl->get_Current(&val_cur);
	*val_cur_lines_density = val_cur;
	val_idx = -1;

	IUsgValues* usg_values = NULL;
	b_lines_density_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			b_lines_density_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				b_lines_density_ctrl->put_Current(val_cur);
				*val_cur_lines_density = val_cur;
			}
			VariantClear(&item);
		}
	}

	SAFE_RELEASE(usg_values);

} // void B_LinesDensitySetPrevNext

void view_area()
{
	// create view area control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgViewArea, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
	{
		view_area_ctrl = (IUsgViewArea*)tmp_obj;
		// set value to 100%
		// view_area_ctrl->put_Current(100);
	}
	else
		view_area_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}


void B_view_areaSetPrevNext(int dir, LONG* val_cur_view_area)
{

	if (view_area_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	view_area_ctrl->get_Current(&val_cur);

	*val_cur_view_area = val_cur;

	val_idx = -1;

	IUsgValues* usg_values = NULL;

	view_area_ctrl->get_Values(&usg_values);

	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			view_area_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				view_area_ctrl->put_Current(val_cur);
				*val_cur_view_area = val_cur;
			}

			VariantClear(&item);
		}
	}
	SAFE_RELEASE(usg_values);
}

void focus_control()
{
	// create B mode focus control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgFocus, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_focus_ctrl = (IUsgFocus*)tmp_obj;
	else
		b_focus_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void B_FocusSetPrevNext(int dir, LONG* val_cur_focus, LONG* focal_zones_cnt, LONG* focal_zone_idx)
{
	if (b_focus_ctrl == NULL) return;

	LONG focus_mode = 0;
	LONG focus_set = 0;
	LONG focal_zones_count = 0;
	FOCAL_ZONE zone;

	b_focus_ctrl->get_CurrentMode(&focus_mode);
	b_focus_ctrl->get_FocusSet(focus_mode, &focus_set);
	b_focus_ctrl->GetFocalZonesCount(focus_mode, focus_set, &focal_zones_count);

	*focal_zones_cnt = focal_zones_count;

	// find the index of first active focus zone
	int focus_zone_idx = -1;
	LONG* arr;
	arr = new LONG[focal_zones_count];
	b_focus_ctrl->GetFocusState(focus_mode, focus_set, focal_zones_count, arr);

	for (int i1 = 0; i1 < focal_zones_count; i1++)
	{
		if (arr[i1] != 0)
		{
			focus_zone_idx = i1;
			break;
		}
	}
	if (focus_zone_idx >= 0)
	{
		focus_zone_idx += dir;
		if ((focus_zone_idx >= 0) && (focus_zone_idx < focal_zones_count))
		{
			for (int i1 = 0; i1 < focal_zones_count; i1++)
				arr[i1] = 0;
			arr[focus_zone_idx] = 1;
			b_focus_ctrl->SetFocusState(focus_mode, focus_set, focal_zones_count, arr);

			b_focus_ctrl->GetFocalZone(focus_mode, focus_set, focus_zone_idx, &zone);
			*val_cur_focus = zone.nFocusPoint;
			*focal_zone_idx = focus_zone_idx;
		}
	}
	delete[] arr;
} // void B_FocusSetPrevNext


int steering_angle()
{
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgSteeringAngle, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_steering_angle_ctrl = (IUsgSteeringAngle*)tmp_obj;
	else
		b_steering_angle_ctrl = NULL;

	if (b_steering_angle_ctrl == NULL)
		return 0;
	else
		return 1;

	if (tmp_obj)
		tmp_obj->Release();
}

void B_SteeringAngleSetPrevNext(int dir, LONG* val_angle)
{
	if (b_steering_angle_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	b_steering_angle_ctrl->get_Current(&val_cur);
	*val_angle = val_cur;
	val_idx = -1;

	IUsgValues* usg_values = NULL;
	b_steering_angle_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			b_steering_angle_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				b_steering_angle_ctrl->put_Current(val_cur);
				*val_angle = val_cur;

			}

			VariantClear(&item);
		}
	}

	SAFE_RELEASE(usg_values);
}


void image_orientation()
{

	// create B image orientation control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgImageOrientation, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_image_orientation_ctrl = (IUsgImageOrientation*)tmp_obj;
	else
		b_image_orientation_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}


void ChangeScanDirection(int val)
{
	if (b_image_orientation_ctrl == NULL)
		return;

	if (val == 1)
		b_image_orientation_ctrl->put_Mirror(val);
	else
		b_image_orientation_ctrl->put_Mirror(val);
}

void wide_view_angle()
{
	// create WideView angle control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgViewAngle, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		wideview_angle_ctrl = (IUsgViewAngle*)tmp_obj;
	else
		wideview_angle_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void WideViewAngleSetPrevNext(int dir, LONG* val_angle_wide_view)
{
	if (wideview_angle_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	wideview_angle_ctrl->get_Current(&val_cur);
	*val_angle_wide_view = val_cur;
	val_idx = -1;

	IUsgValues* usg_values = NULL;
	wideview_angle_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			wideview_angle_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				wideview_angle_ctrl->put_Current(val_cur);
				*val_angle_wide_view = val_cur;
			}
			VariantClear(&item);
		}
	}

	SAFE_RELEASE(usg_values);
}

void scan_type_control()
{
	// create scanning type control
	wideview_is_supported = false;
	compound_is_supported = false;
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgScanType, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
	{
		scan_type_ctrl = (IUsgScanType*)tmp_obj;

		// check what B scanning types are supported
		IUsgValues* usg_values = NULL;
		scan_type_ctrl->get_Values(&usg_values);
		if (usg_values != NULL)
		{
			LONG val_count;
			usg_values->get_Count(&val_count);
			for (LONG i1 = 0; i1 < val_count; i1++)
			{
				VARIANT item;
				LONG val;
				usg_values->Item(i1, &item);
				val = item.lVal;
				if (val == SCAN_TYPE_VIRTUALCONVEX)
					wideview_is_supported = true;
				else if (val == SCAN_TYPE_COMPOUND)
					compound_is_supported = true;
				VariantClear(&item);
			}
			SAFE_RELEASE(usg_values);
		}
	}
	else
	{
		scan_type_ctrl = NULL;
	}

	if (tmp_obj)
		tmp_obj->Release();
}

void turn_on_scan_type(int type_ID)
{

	if (scan_type_ctrl == NULL) return;

	if (type_ID == 0)
		scan_type_ctrl->put_Current(SCAN_TYPE_NORMAL);
	if (type_ID == 1 && wideview_is_supported)
		scan_type_ctrl->put_Current(SCAN_TYPE_VIRTUALCONVEX);
	if (type_ID == 2 && compound_is_supported)
		scan_type_ctrl->put_Current(SCAN_TYPE_COMPOUND);

}

void compound_angle()
{
	// create Compound angle control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgCompoundAngle, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		compound_angle_ctrl = (IUsgCompoundAngle*)tmp_obj;
	else
		compound_angle_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void compound_frames_number()
{
	// create Compound frames number control
	compound_frames_enabled = false;
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgCompoundFrames, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
	{
		compound_frames_ctrl = (IUsgCompoundFrames*)tmp_obj;

		compound_frames_enabled = true;
		LONG val_count = 0;
		IUsgValues* usg_values = NULL;
		compound_frames_ctrl->get_Values(&usg_values);
		if (usg_values != NULL)
			usg_values->get_Count(&val_count);
		SAFE_RELEASE(usg_values);
		if (val_count <= 1)
			compound_frames_enabled = false;
	}
	else
		compound_frames_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}


void CompoundFramesSetPrevNext(int dir, LONG* val_frames)
{
	if (compound_frames_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	compound_frames_ctrl->get_Current(&val_cur);

	*val_frames = val_cur;

	val_idx = -1;

	IUsgValues* usg_values = NULL;
	compound_frames_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			compound_frames_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				compound_frames_ctrl->put_Current(val_cur);
				*val_frames = val_cur;
			}
			VariantClear(&item);
		}
	}

	SAFE_RELEASE(usg_values);
}

void CompoundAngleSetPrevNext(int dir, LONG* val_angle_comp)
{
	if (compound_angle_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	compound_angle_ctrl->get_Current(&val_cur);

	*val_angle_comp = val_cur;

	val_idx = -1;

	IUsgValues* usg_values = NULL;
	compound_angle_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			compound_angle_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				compound_angle_ctrl->put_Current(val_cur);
				*val_angle_comp = val_cur;
			}
			VariantClear(&item);
		}
	}

	SAFE_RELEASE(usg_values);
}

void TGC_control()
{
	// create TGC control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgTgc, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		tgc_ctrl = (IUsgTgc*)tmp_obj;
	else
		tgc_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void adjust_TGC(int ctrl_idx, int val_idx, LONG* tgc_depth_value)
{

	if (tgc_ctrl == NULL)
		return;

	IUsgValues* values = NULL;
	tgc_ctrl->get_ValuesTgc(&values);
	LONG tgc_pos, tgc_depth;


	if (values == NULL)
		return;

	LONG count;

	values->get_Count(&count);

	LONG val;
	if ((val_idx >= 0) && (val_idx < count))
	{
		VARIANT item;
		values->Item(val_idx, &item);
		val = item.lVal;
		LONG cur;
		tgc_ctrl->get_Current(ctrl_idx, &cur);
		if (cur != val)
		{
			tgc_ctrl->put_Current(ctrl_idx, val);
			tgc_ctrl->get_CtlPointPos(ctrl_idx, &tgc_pos);
			tgc_ctrl->PositionToDepth(tgc_pos, &tgc_depth);
			*tgc_depth_value = tgc_depth;
		}
		else
		{
			tgc_ctrl->get_CtlPointPos(ctrl_idx, &tgc_pos);
			tgc_ctrl->PositionToDepth(tgc_pos, &tgc_depth);
			*tgc_depth_value = tgc_depth;
		}
	}
	else
	{
		*tgc_depth_value = -1;
	}
	SAFE_RELEASE(values);
}

void B_dynamic_range()
{
	// create B mode dynamic range control
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgDynamicRange, SCAN_MODE_B, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		b_dynamic_range_ctrl = (IUsgDynamicRange*)tmp_obj;
	else
		b_dynamic_range_ctrl = NULL;

	if (tmp_obj)
		tmp_obj->Release();
}

void B_DynamicRangeSetPrevNext(int dir, LONG* value_dynamic_range)
{

	if (b_dynamic_range_ctrl == NULL) return;

	LONG val_cur;
	LONG val_idx;
	LONG val_count;

	b_dynamic_range_ctrl->get_Current(&val_cur);

	*value_dynamic_range = val_cur;

	val_idx = -1;

	IUsgValues* usg_values = NULL;
	b_dynamic_range_ctrl->get_Values(&usg_values);
	if (usg_values == NULL) return;

	usg_values->get_Count(&val_count);

	for (LONG i1 = 0; i1 < val_count; i1++)
	{
		VARIANT item;
		LONG val;
		usg_values->Item(i1, &item);
		val = item.lVal;
		if (val_cur == val)
			val_idx = i1;
		VariantClear(&item);
	}

	if (val_idx >= 0)
	{
		val_idx += dir;
		if ((val_idx >= 0) && (val_idx < val_count))
		{
			VARIANT item;
			usg_values->Item(val_idx, &item);
			val_cur = item.lVal;
			LONG val;
			b_dynamic_range_ctrl->get_Current(&val);
			if (val != val_cur)
			{
				b_dynamic_range_ctrl->put_Current(val_cur);
				*value_dynamic_range = val_cur;
			}

			VariantClear(&item);
		}
	}
	SAFE_RELEASE(usg_values);
}

STDMETHODIMP SampleGrabberCallback::SampleCB(double SampleTime, IMediaSample* pSample)
{
	HRESULT hr = S_OK;
	bool fUpdateGUI = false;
	do
	{
		if (!pSample)
			break;

		// first try to retrieve media type
		AM_MEDIA_TYPE* pmt;
		hr = pSample->GetMediaType(&pmt);
		if (hr == S_OK)
		{
			if (m_pMediaType)
				DeleteMediaType(m_pMediaType);
			m_pMediaType = pmt;
			fUpdateGUI = true;
		}

		int nSubFrameIndex = 0;
		IMediaSample2* pSample2;
		AM_SAMPLE2_PROPERTIES aspSampleProps;
		if (SUCCEEDED(pSample->QueryInterface(IID_IMediaSample2, (void**)&pSample2)))
		{
			HRESULT hr = pSample2->GetProperties(sizeof(aspSampleProps), (PBYTE)&aspSampleProps);
			//ATLTRACE2(__FUNCTION__" dwTypeSpecificFlags = 0x%08x\n", aspSampleProps.dwTypeSpecificFlags);
			pSample2->Release();

			if (SUCCEEDED(hr))
			{
				nSubFrameIndex = (aspSampleProps.dwTypeSpecificFlags >> 4) & 0xf;
			}
		}

		//if (!hWnd)
		//	break;
		if (!m_pMediaType || !m_pMediaType->pbFormat)
			break;

		if (!IsEqualGUID(m_pMediaType->majortype, KSDATAFORMAT_TYPE_ULTRASOUND))
			break;
		if (!IsEqualGUID(m_pMediaType->subtype, KSDATAFORMAT_SUBTYPE_ARTUSDEVICE))
			break;
		if (!IsEqualGUID(m_pMediaType->formattype, KSDATAFORMAT_SPECIFIER_ULTRASOUNDINFORF))
			break;

		BYTE* pBuffer = 0;
		long BufferLen = 0;

		hr = pSample->GetPointer(&pBuffer);
		if (FAILED(hr))
			break;

		if (!pBuffer)
			break;

		BufferLen = pSample->GetActualDataLength();
		if (!BufferLen)
			break;

		PULTRASOUNDRF_INFO pFrameInfo = (PULTRASOUNDRF_INFO)m_pMediaType->pbFormat;

		LONG line1;
		LONG line2;
		m_pUsgWindowRF->GetWidth(&line1, &line2);

		LONG depth1;
		LONG depth2;
		m_pUsgWindowRF->GetVolume(&depth1, &depth2);

		Sampling_period = pFrameInfo->SamplingTime;
		LONG samples_per_RF_window = floor(2 * ((double)depth2 - (double)depth1) / 1000 / 1540 / ((double)Sampling_period / 1000000000));
		LONG scanning_lines_per_RF_window = line2 - line1 + 1;

		flag = 0;
		if ((temp_RF_ID != rf_data_source_point_id || RF_samples_per_beam != samples_per_RF_window || RF_scanning_lines != scanning_lines_per_RF_window || RF_frames_counter == 0) && buffer_of_RF_data_frame != NULL)
		{
			delete[] buffer_of_RF_data_frame;
			buffer_of_RF_data_frame = NULL;
		}

		RF_samples_per_beam = samples_per_RF_window;
		RF_scanning_lines = scanning_lines_per_RF_window;

		if (write_RF == true)
			if (rf_data_source_point_id != 4)
				SetRFDataI32_full_frame(pFrameInfo, pBuffer, 1, nSubFrameIndex); //nSubFrameIndex number_of_frames_to_record; 3rd parameter 1 for 1-3 outputs and 2  for hilbert
			else
				SetRFDataI32_full_frame(pFrameInfo, pBuffer, 2, nSubFrameIndex);


		if (RF_frames_counter == 0 || buffer_of_RF_data_frame == NULL)
		{
			if (rf_data_source_point_id != 4)
				buffer_of_RF_data_frame = new INT16[RF_samples_per_beam * RF_scanning_lines]();
			else
				buffer_of_RF_data_frame = new INT16[RF_samples_per_beam * RF_scanning_lines * 2]();
		}

		if (BufferLen >= (long)pFrameInfo->UsgFrameInfoHeader.FrameSize && rf_data_source_point_id != 4)
		{
			INT16* p_src_buff = (INT16*)pBuffer;
			for (int i = 0; i < RF_scanning_lines; i++)
			{
				memcpy((buffer_of_RF_data_frame + i * RF_samples_per_beam), p_src_buff, sizeof(INT16) * RF_samples_per_beam);
				pBuffer += pFrameInfo->UsgFrameInfoHeader.Pitch;
				p_src_buff = (INT16*)pBuffer;
			}
			flag = 1;
		}


		if (BufferLen >= (long)pFrameInfo->UsgFrameInfoHeader.FrameSize && rf_data_source_point_id == 4)
		{
			int k, k1;
			k = 0; k1 = 0;
			INT16* p_src_buff = (INT16*)pBuffer;
			int frame_offset = RF_scanning_lines * RF_samples_per_beam;
			for (int i = 0; i < RF_scanning_lines; i++)
			{

				for (int j = 0; j < RF_samples_per_beam * 2; j++) {

					if (j % 2 != 1)
					{
						*(buffer_of_RF_data_frame + k) = *(p_src_buff + j + i * RF_samples_per_beam * 2);
						k++;
					}
					else
					{
						*(buffer_of_RF_data_frame + frame_offset + k1) = *(p_src_buff + j + i * RF_samples_per_beam * 2);
						k1++;
					}
				}
			}
			flag = 2;
		}

		temp_RF_ID = rf_data_source_point_id;
		RF_data_buffer_size = BufferLen;
		RF_frames_counter++;

	}

	while (false);
	return S_OK;
}


STDMETHODIMP SampleGrabberCallback::BufferCB(double SampleTime, BYTE* pBuffer, long BufferLen)
{
	return S_OK;
}

void SampleGrabberCallback::SetPlugin(ISampleGrabber* plugin)
{
	m_plugin = plugin;
	if (!m_plugin)
		return;

	AM_MEDIA_TYPE mt;
	if (SUCCEEDED(m_plugin->GetConnectedMediaType(&mt)))
	{
		if (m_pMediaType)
			DeleteMediaType(m_pMediaType);
		m_pMediaType = NULL;
		m_pMediaType = CreateMediaType(&mt);
		FreeMediaType(mt);
	}
}

HRESULT __stdcall SampleGrabberCallback::QueryInterface(REFIID riid, void** ppObj)
{
	if ((riid == IID_IUnknown) || (riid == IID_ISampleGrabberCB))
	{
		*ppObj = (void*)(this);
		AddRef();
		return S_OK;
	}
	*ppObj = NULL;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE SampleGrabberCallback::AddRef(void)
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG STDMETHODCALLTYPE SampleGrabberCallback::Release(void)
{
	long nRefCount = 0;
	nRefCount = InterlockedDecrement(&m_nRefCount);
	if (nRefCount == 0)
		delete this;
	return nRefCount;
}

SampleGrabberCallback::SampleGrabberCallback()
	: m_pMediaType(NULL)
{
	m_nRefCount = 0;
	m_plugin = NULL;
	//m_usg_wnd = NULL;
	m_usg_x = 0;
	m_usg_y = 0;
}

SampleGrabberCallback::~SampleGrabberCallback()
{
	if (m_pMediaType)
		DeleteMediaType(m_pMediaType);
	m_pMediaType = NULL;
}

void set_callback_Sample_Grabber()
{

	// create sample grabber callback COM object that can be implemented as class like in
		// this sample or can be implemented as a usual COM object that is created using CoCreateInstance
	m_sample_grabber_callback = new SampleGrabberCallback();
	m_sample_grabber_callback->AddRef();

	if (!m_sample_grabber_callback)
		return;

	// enable B RF data
	IUsgStreamEnable* pUsgStreamEnable = NULL;
	CreateUsgControl(m_data_view, IID_IUsgStreamEnable, SCAN_MODE_RFB, 0, (void**)&pUsgStreamEnable);
	if (pUsgStreamEnable)
	{
		pUsgStreamEnable->put_Enable(TRUE);
		pUsgStreamEnable->Release();
	}
	else
	{
		pUsgStreamEnable = NULL;
		return;
	}

	// get ISampleGrabberFilter for desired ultrasound data *stream*
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_ISampleGrabberFilter, SCAN_MODE_RFB, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		sample_grabber_filter = (ISampleGrabberFilter*)tmp_obj;
	SAFE_RELEASE(tmp_obj);

	// get sample grabber interface
	IUnknown* tmp_unk = NULL;
	m_sample_grabber_plugin = NULL;
	if (sample_grabber_filter != NULL)
		sample_grabber_filter->get_FilterObj(0, &tmp_unk);

	// get plugin
	if (tmp_unk != NULL)
	{
		hr = tmp_unk->QueryInterface(IID_ISampleGrabber, (void**)&m_sample_grabber_plugin);
		if (hr != S_OK)
			m_sample_grabber_plugin = NULL;
		SAFE_RELEASE(tmp_unk);
	}

	// register COM callback object and specify method(s) to callback
	if ((m_sample_grabber_plugin != NULL) && (m_sample_grabber_callback != NULL))
	{
		hr = m_sample_grabber_plugin->SetCallback(m_sample_grabber_callback, 0);
		m_sample_grabber_callback->SetPlugin(m_sample_grabber_plugin);
	}
}

void ProcessSample(PBYTE pBuffer, int nBufferLen, PULTRASOUNDRF_INFO pFrameInfo, int nSubFrameIndex, bool updateGUI)
{
	do
	{
		if (!pBuffer || !nBufferLen || !pFrameInfo)
			break;

		m_nLineIndex = min((LONG)pFrameInfo->UsgFrameInfoHeader.BeamsPerFrame - 1, max(0, m_nLineIndex));

		int nOffset = pFrameInfo->UsgFrameInfoHeader.Pitch * (m_nLineIndex);

		if (pFrameInfo->UsgFrameInfoHeader.ChannelsPerBeam)
			nOffset *= pFrameInfo->UsgFrameInfoHeader.ChannelsPerBeam;

		if (pFrameInfo->UsgFrameInfoHeader.SubBeamsNumber)
			nOffset *= pFrameInfo->UsgFrameInfoHeader.SubBeamsNumber;

		bool set_iq = true;
		if (scan_type_ctrl != NULL)
		{
			LONG scan_type;
			scan_type_ctrl->get_Current(&scan_type);
			if ((compound_is_supported) && (scan_type == SCAN_TYPE_COMPOUND) && (nSubFrameIndex != m_nSubFrameIndex))
				set_iq = false;
		}


	} while (false);
}

void RF_data_controls()
{
	// enable B RF data
	IUsgStreamEnable* pUsgStreamEnable = NULL;
	CreateUsgControl(m_data_view, IID_IUsgStreamEnable, SCAN_MODE_RFB, 0, (void**)&pUsgStreamEnable);
	if (pUsgStreamEnable)
	{
		pUsgStreamEnable->put_Enable(TRUE);
		pUsgStreamEnable->Release();
	}
	else
	{
		pUsgStreamEnable = NULL;
		return;
	}

	// control to set RF window
	IUnknown* tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgWindowRF, SCAN_MODE_RFB, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
		m_pUsgWindowRF = (IUsgWindowRF*)tmp_obj;
	else
	{
		m_pUsgWindowRF = NULL;
		return;
	}

	// control to set RF data type
	tmp_obj = NULL;
	CreateUsgControl(m_data_view, IID_IUsgDataSourceRF, SCAN_MODE_RFB, 0, (void**)&tmp_obj);
	if (tmp_obj != NULL)
	{
		m_pUsgDataSourceRF = (IUsgDataSourceRF*)tmp_obj;
		m_pUsgDataSourceRF->put_Current(3);
		rf_data_source_point_id = 1;
	}
	else
	{
		m_pUsgDataSourceRF = NULL;
		return;
	}
	if (tmp_obj)
		tmp_obj->Release();
}

ScanConverterCallback::ScanConverterCallback()
{
	m_nRefCount = 0;
	m_plugin = NULL;
}

ScanConverterCallback::~ScanConverterCallback()
{
}

HRESULT __stdcall ScanConverterCallback::QueryInterface(REFIID riid, void** ppObj)
{
	if (riid == IID_IUnknown || riid == IID_IUsgfwScanConverterPluginCB)
	{
		*ppObj = (void*)(this);
		AddRef();
		return S_OK;
	}
	*ppObj = NULL;
	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE ScanConverterCallback::AddRef(void)
{
	return InterlockedIncrement(&m_nRefCount);
}

//------------------------------

ULONG STDMETHODCALLTYPE ScanConverterCallback::Release(void)
{
	long nRefCount = 0;
	nRefCount = InterlockedDecrement(&m_nRefCount);
	if (nRefCount == 0)
		delete this;
	return nRefCount;
}


HRESULT ScanConverterCallback::BufferCB(THIS_
	PBYTE pBufferIn,
	int nInBufferLen,
	PBYTE pBufferOut,
	int nOutBufferLen,
	int nOutX1,
	int nOutY1,
	int nOutX2,
	int nOutY2
)
{
	if ((m_plugin == NULL) || (pBufferOut == NULL) || (nOutBufferLen == NULL)) //  || (m_usg_wnd == NULL)
		return S_OK;


	AM_MEDIA_TYPE mtOut;
	VIDEOINFO* pVideoInfo = NULL;

	if (m_plugin->GetConnectedMediaType(PINDIR_OUTPUT, &mtOut) != S_OK)
		return S_OK;

	// get output image format
	pVideoInfo = (VIDEOINFO*)mtOut.pbFormat;
	if (pVideoInfo == NULL)
	{
		FreeMediaType(mtOut);
		return S_OK;
	}

	int output_width, output_height, output_bits_per_pixel, output_pitch;
	output_width = pVideoInfo->bmiHeader.biWidth;
	output_height = abs(pVideoInfo->bmiHeader.biHeight);
	output_bits_per_pixel = pVideoInfo->bmiHeader.biBitCount;
	output_pitch = ScanBytes(output_width, output_bits_per_pixel);


	// ---------- Make a copy of image array ------------------------
	if (frames_counter == 0 && buffer_of_image_data != NULL)
	{
		delete[] buffer_of_image_data;
	}
	if (frames_counter == 0 || previous_height != output_height || previous_width != output_width)
	{
		buffer_of_image_data = new unsigned int[output_height * output_width * 4];
	}

	for (int i = 0; i < output_height * output_width * 4; i++)//*4
		*(buffer_of_image_data + i) = (unsigned int)*(pBufferOut + i);

	frames_counter++;

	previous_height = output_height;
	previous_width = output_width;


	// -------------------------------------------------------------- 

	FreeMediaType(mtOut);

	//------------------------------------------------------
	return S_OK;
} // HRESULT ScanConverterCallback::BufferCB

int ScanBytes(long w, int bpp)
{
	return (int)(((w * bpp + 31) & (~31)) / 8);
}

void ScanConverterCallback::SetPlugin(IUsgfwScanConverterPlugin* plugin)
{
	m_plugin = plugin;
}