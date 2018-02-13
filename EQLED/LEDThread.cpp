#include "stdafx.h"
#include "LEDThread.h"
#include <LogitechLEDLib.h> //Logitech LED Library 8.87
int CLEDThread::m_iaaKeymap[22][6] = {
	{LogiLed::LEFT_CONTROL, LogiLed::LEFT_SHIFT, LogiLed::CAPS_LOCK, LogiLed::TAB, LogiLed::TILDE, LogiLed::ESC},
	{LogiLed::LEFT_WINDOWS, LogiLed::Z, LogiLed::A, LogiLed::Q, LogiLed::ONE, -1 },
	{LogiLed::LEFT_ALT, LogiLed::X, LogiLed::S, LogiLed::W, LogiLed::TWO, LogiLed::F1},
	{LogiLed::SPACE, LogiLed::C, LogiLed::D, LogiLed::E, LogiLed::THREE, LogiLed::F2},
	{LogiLed::SPACE, LogiLed::V, LogiLed::F, LogiLed::R, LogiLed::FOUR, LogiLed::F3},
	{LogiLed::SPACE, LogiLed::B, LogiLed::G, LogiLed::T, LogiLed::FIVE, LogiLed::F4},
	{LogiLed::SPACE, LogiLed::N, LogiLed::H, LogiLed::Y, LogiLed::SIX, LogiLed::F5},
	{LogiLed::SPACE, LogiLed::M, LogiLed::J, LogiLed::U, LogiLed::SEVEN, LogiLed::F6},
	{LogiLed::RIGHT_ALT, LogiLed::COMMA, LogiLed::K, LogiLed::I, LogiLed::EIGHT, LogiLed::F7},
	{LogiLed::RIGHT_ALT, LogiLed::PERIOD, LogiLed::L, LogiLed::O, LogiLed::NINE, LogiLed::F8},
	{LogiLed::RIGHT_WINDOWS, LogiLed::FORWARD_SLASH, LogiLed::SEMICOLON, LogiLed::P, LogiLed::ZERO, -1},
	{LogiLed::APPLICATION_SELECT, LogiLed::RIGHT_SHIFT, LogiLed::APOSTROPHE, LogiLed::OPEN_BRACKET, LogiLed::MINUS, LogiLed::F9},
	{LogiLed::RIGHT_CONTROL, LogiLed::RIGHT_SHIFT, LogiLed::ENTER, LogiLed::CLOSE_BRACKET, LogiLed::EQUALS, LogiLed::F10},
	{LogiLed::RIGHT_CONTROL, LogiLed::RIGHT_SHIFT, LogiLed::ENTER, LogiLed::BACKSLASH, LogiLed::BACKSPACE, LogiLed::F11},
	{LogiLed::RIGHT_CONTROL, LogiLed::RIGHT_SHIFT, LogiLed::ENTER, LogiLed::BACKSLASH, LogiLed::BACKSPACE, LogiLed::F12},
	{LogiLed::ARROW_LEFT, -1, -1, LogiLed::KEYBOARD_DELETE, LogiLed::INSERT, LogiLed::PRINT_SCREEN},
	{LogiLed::ARROW_DOWN, LogiLed::ARROW_UP, -1, LogiLed::END, LogiLed::HOME, LogiLed::SCROLL_LOCK},
	{LogiLed::ARROW_RIGHT, -1, -1, LogiLed::PAGE_DOWN, LogiLed::PAGE_UP, LogiLed::PAUSE_BREAK},
	{LogiLed::NUM_ZERO, LogiLed::NUM_ONE, LogiLed::NUM_FOUR, LogiLed::NUM_SEVEN, LogiLed::NUM_LOCK, -1},
	{LogiLed::NUM_ZERO, LogiLed::NUM_TWO, LogiLed::NUM_FIVE, LogiLed::NUM_EIGHT, LogiLed::NUM_SLASH, -1},
	{ LogiLed::NUM_PERIOD, LogiLed::NUM_THREE, LogiLed::NUM_SIX, LogiLed::NUM_NINE, LogiLed::NUM_ASTERISK, -1 },
	{ LogiLed::NUM_ENTER, LogiLed::NUM_ENTER, LogiLed::NUM_PLUS, LogiLed::NUM_PLUS, LogiLed::NUM_MINUS, -1 },
};
struct KEYRGB CLEDThread::m_raColorMap[MAX_KEYMAP] = {};

const int CLEDThread::__SPECHEIGHT__ = 127;
const int CLEDThread::__SPECWIDTH__ = 368;

BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE; // continue recording
}

void DisplayDeviceInfo(BASS_DEVICEINFO *di)
{
	printf("%s\n\tdriver: %s\n\ttype: ", di->name, di->driver);
	switch (di->flags&BASS_DEVICE_TYPE_MASK) {
	case BASS_DEVICE_TYPE_NETWORK:
		printf("Remote Network");
		break;
	case BASS_DEVICE_TYPE_SPEAKERS:
		printf("Speakers");
		break;
	case BASS_DEVICE_TYPE_LINE:
		printf("Line");
		break;
	case BASS_DEVICE_TYPE_HEADPHONES:
		printf("Headphones");
		break;
	case BASS_DEVICE_TYPE_MICROPHONE:
		printf("Microphone");
		break;
	case BASS_DEVICE_TYPE_HEADSET:
		printf("Headset");
		break;
	case BASS_DEVICE_TYPE_HANDSET:
		printf("Handset");
		break;
	case BASS_DEVICE_TYPE_DIGITAL:
		printf("Digital");
		break;
	case BASS_DEVICE_TYPE_SPDIF:
		printf("SPDIF");
		break;
	case BASS_DEVICE_TYPE_HDMI:
		printf("HDMI");
		break;
	case BASS_DEVICE_TYPE_DISPLAYPORT:
		printf("DisplayPort");
		break;
	default:
		printf("Unknown");
	}
	printf("\n\tflags:");
	if (di->flags&BASS_DEVICE_ENABLED) printf(" enabled");
	if (di->flags&BASS_DEVICE_DEFAULT) printf(" default");
	printf(" (%x)\n", di->flags);
}

CLEDThread::CLEDThread()
{
	m_bShouldReload = true;
	
	_InitColorMap();
	LoadConfig();

	m_hChannelHandle = NULL;
}


CLEDThread::~CLEDThread()
{
	BASS_RecordFree();
	SaveConfig();
}

// FOR TEST
void CLEDThread::Step() {
	double flow = 0;
	double integral = 0;
	modf(flow, &integral);
	integral = flow - integral;
	int cur_depth = int(flow);
	for (int j = 0; j < cur_depth; j++) {
		for (int i = 0; i < 23; i++) {
			int key = m_iaaKeymap[i][j];
			if (key != -1) {
				LogiLed::KeyName k = (LogiLed::KeyName)key;
				KEYRGB rgb;
				if (j < cur_depth)
					rgb = ConvertColor(key, 1);
				else if (j == cur_depth)
					rgb = ConvertColor(key, (float)integral);
				else
					rgb = ConvertColor(key, 0);
				LogiLedSetLightingForKeyWithKeyName(k, rgb.r, rgb.g, rgb.b);
			}
		}
	}

	flow += 0.1;
	if (int(cur_depth) > 5) {
		flow = 0;
		//shouldReload = true;
	}
	Sleep(100);
}

#define INI_FILE	_T(".\\eqled.ini")
#define INI_SECT	_T("CONF")
#define INI_DEV		_T("DEV")
void CLEDThread::LoadConfig() {
	
	int res = GetPrivateProfileInt(INI_SECT, INI_DEV, -1, INI_FILE);
	int devs = _GetDeviceCount();
	if (res >= devs || res < -1)
		res = -1;
	this->m_iRecordDevId = res;
}

int CLEDThread::_GetDeviceCount() {
	int devs = 0;
	BASS_DEVICEINFO buf;
	for (; BASS_RecordGetDeviceInfo(devs, &buf); ++devs);
	return devs;
}

void CLEDThread::SaveConfig() {
	CString buf = _T("");
	buf.Format(_T("%d"), this->m_iRecordDevId);
	WritePrivateProfileString(INI_SECT, INI_DEV, buf, INI_FILE);
}

void CLEDThread::ReloadDevice()
{
	BASS_DEVICEINFO buf;
	if(m_iRecordDevId != -1)
		BASS_RecordGetDeviceInfo(this->m_iRecordDevId, &buf);
	if (!BASS_RecordInit(this->m_iRecordDevId)) {
		int errorcode = BASS_ErrorGetCode();
		CString msg;
		msg.Format(_T("Can't initialize device %s \nError Code : %d"), 
			(m_iRecordDevId != -1 ? CString(buf.name) : _T("Default Device")),
			errorcode);
		CStringA msgA = CStringA(msg);
		MessageBoxA(NULL, msgA, "Can't initialize device", MB_ICONERROR); //assert(true);

		printf("Can't initialize device\n");
		AfxGetMainWnd()->PostMessageW(WM_COMMAND, ID_APP_EXIT);
	}

	if (!(m_hChannelHandle = BASS_RecordStart(44100, 1, 0, &DuffRecording, 0))) {
		int errorcode = BASS_ErrorGetCode();
		CString msg;
		msg.Format(_T("Can't start recording device %s \nError Code : %d"), 
			(m_iRecordDevId != -1 ? CString(buf.name) : _T("Default Device")),
			errorcode);
		CStringA msgA = CStringA(msg);
		MessageBoxA(NULL, msgA, "Can't start recording", MB_ICONERROR); //assert(true);

		printf("Can't start recording\n");
		AfxGetMainWnd()->PostMessageW(WM_COMMAND, ID_APP_EXIT);
	}

}

void CLEDThread::CleanBASS()
{
	if (m_hChannelHandle != NULL) {
		BASS_RecordFree();
		BASS_ChannelStop(m_hChannelHandle);
		m_hChannelHandle = NULL;
	}
}

void CLEDThread::SetDevId(int id)
{
	int devs = _GetDeviceCount();
	if (id >= devs || id < -1)
		id = -1;
	this->m_iRecordDevId = id;
	SaveConfig();
}

int CLEDThread::GetDevId()
{
	return this->m_iRecordDevId;
}

void CLEDThread::Draw(float data[22])
{
	int i,j;
	double integral = 0;
	int cur_depth;
	for (i = 0; i < 22; i++) {
		if (data[i] == m_faPrevColor[i])
			continue;
		m_faPrevColor[i] = data[i];
		modf(data[i], &integral);
		integral = data[i] - integral;
		cur_depth = int(data[i]);
		for (j = 0; j < 6; j++) {
			int key = m_iaaKeymap[i][j];
			if (key != -1) {
				LogiLed::KeyName k = (LogiLed::KeyName)key;
				KEYRGB rgb;

				if (j < cur_depth)
					rgb = ConvertColor(key, 1);
				else if (j >= cur_depth + 1) 
					rgb = { 0, 0, 0 };
				else 
					rgb = ConvertColor(key, integral);

				LogiLedSetLightingForKeyWithKeyName(k, rgb.r, rgb.g, rgb.b);
			}
		}
	}
}

void CLEDThread::run()
{
	int x;
	double y;
	float convert[22] = {};
	float first_convert[11] = {};
#ifdef TEST
	float test[22] = { 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6 };
#endif
	while (true) {
		if (m_bShouldReload) {
			CleanBASS();
			ReloadSetting();
			ReloadDevice();
			m_bShouldReload = false;
		}

#ifdef TEST
		draw(test);
		Sleep(10);
		continue;
#endif
		
		int b0 = 0;
		float fft[128];
#define BANDS 22
#define MAX_WIDTH 100
		BASS_ChannelGetData(m_hChannelHandle, fft, BASS_DATA_FFT256); // get the FFT data
		memset(m_BaSpecBuf, 0, __SPECWIDTH__*__SPECHEIGHT__);
		for (x = 0; x<BANDS; x++) {
			float peak = 0;
			int b1 = pow(2, x*5.0 / (BANDS-1));
			if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin
			if (b1>MAX_WIDTH) b1 = MAX_WIDTH;
			//if (b1>127) b1 = 127;
			for (; b0<b1; b0++)
				if (peak<fft[1 + b0]) peak = fft[1 + b0];
			double ssqrt = sqrt(peak);
			y = ssqrt * 6.0f * 6; // scale it (sqrt to make low values more visible)
			if (y>6) y = 6.0f * 6; // cap it
			//first_convert[x] = y;
			convert[x] = y;
		}
		/*
		for (x = 0; x < BANDS; x++) {
			convert[x] = first_convert[x];
			convert[BANDS+x] = first_convert[x];
		}
		*/

		Draw(convert);
		Sleep(50);
	}
}

void CLEDThread::ReloadSetting()
{
	LogiLedInit();
	ResetColor();
}

void CLEDThread::ResetColor() {
	LogiLedSetLighting(0, 0, 0);
	for (int i = 0; i < 22; i++) {
		m_faPrevColor[i] = 0;
	}
}

KEYRGB operator+(const KEYRGB& op1, const KEYRGB& op2) {
	return { (op1.r + op2.r) / 2, (op1.g + op2.g) / 2, (op1.b + op2.b) / 2 };
}
void CLEDThread::_InitColorMap()
{
	static const KEYRGB CPURPLE		 = { 0.5f, 0, 0.7f };
	static const KEYRGB CBLUE		 = { 0, 0, 1 };
	static const KEYRGB CPUPLE_BLUE = CPURPLE + CBLUE;
	static const KEYRGB CGREEN		 = { 0, 0.5f, 0 };
	static const KEYRGB CYELLOW		 = { 1, 1, 0 };
	static const KEYRGB CGREEN_YELLOW = CGREEN + CYELLOW;
	static const KEYRGB CORANGE		 = { 1, 0.5f, 0 };
	static const KEYRGB CRED		 = { 1, 0, 0 };
	m_raColorMap[LogiLed::LEFT_CONTROL]	 = CPURPLE;
	m_raColorMap[LogiLed::LEFT_WINDOWS]	 = CPURPLE;
	m_raColorMap[LogiLed::LEFT_ALT]		 = CPURPLE;
	m_raColorMap[LogiLed::SPACE]			 = CPURPLE;
	m_raColorMap[LogiLed::RIGHT_ALT]		 = CPURPLE;
	m_raColorMap[LogiLed::RIGHT_WINDOWS]	 = CPURPLE;
	m_raColorMap[LogiLed::RIGHT_CONTROL]	 = CPURPLE;
	m_raColorMap[LogiLed::APPLICATION_SELECT] = CPURPLE;
	m_raColorMap[LogiLed::ARROW_LEFT]		 = CPURPLE;
	m_raColorMap[LogiLed::ARROW_DOWN]		 = CPURPLE;
	m_raColorMap[LogiLed::ARROW_RIGHT]		 = CPURPLE;
	m_raColorMap[LogiLed::NUM_ZERO]		 = CPURPLE;
	m_raColorMap[LogiLed::NUM_PERIOD]		 = CPURPLE;
	m_raColorMap[LogiLed::NUM_ENTER]		 = CPUPLE_BLUE;//{ 0.3f, 0, 0.8f };
	m_raColorMap[LogiLed::LEFT_SHIFT]		 = CBLUE;
	m_raColorMap[LogiLed::Z]				 = CBLUE;
	m_raColorMap[LogiLed::X]				 = CBLUE;
	m_raColorMap[LogiLed::C]				 = CBLUE;
	m_raColorMap[LogiLed::V]				 = CBLUE;
	m_raColorMap[LogiLed::B]				 = CBLUE;
	m_raColorMap[LogiLed::N]				 = CBLUE;
	m_raColorMap[LogiLed::N]				 = CBLUE;
	m_raColorMap[LogiLed::M]				 = CBLUE;
	m_raColorMap[LogiLed::COMMA]			 = CBLUE;
	m_raColorMap[LogiLed::PERIOD]			 = CBLUE;
	m_raColorMap[LogiLed::FORWARD_SLASH]	 = CBLUE;
	m_raColorMap[LogiLed::RIGHT_SHIFT]		 = CBLUE;
	m_raColorMap[LogiLed::ARROW_UP]		 = CBLUE;
	m_raColorMap[LogiLed::NUM_ONE]			 = CBLUE;
	m_raColorMap[LogiLed::NUM_TWO]			 = CBLUE;
	m_raColorMap[LogiLed::NUM_THREE]		 = CBLUE;
	m_raColorMap[LogiLed::CAPS_LOCK]		 = CGREEN;
	m_raColorMap[LogiLed::A]				 = CGREEN;
	m_raColorMap[LogiLed::S]				 = CGREEN;
	m_raColorMap[LogiLed::D]				 = CGREEN;
	m_raColorMap[LogiLed::F]				 = CGREEN;
	m_raColorMap[LogiLed::G]				 = CGREEN;
	m_raColorMap[LogiLed::H]				 = CGREEN;
	m_raColorMap[LogiLed::J]				 = CGREEN;
	m_raColorMap[LogiLed::K]				 = CGREEN;
	m_raColorMap[LogiLed::L]				 = CGREEN;
	m_raColorMap[LogiLed::SEMICOLON]		 = CGREEN;
	m_raColorMap[LogiLed::APOSTROPHE]		 = CGREEN;
	m_raColorMap[LogiLed::ENTER]			 = CGREEN;
	m_raColorMap[LogiLed::NUM_FOUR]		 = CGREEN;
	m_raColorMap[LogiLed::NUM_FIVE]		 = CGREEN;
	m_raColorMap[LogiLed::NUM_SIX]			 = CGREEN;
	m_raColorMap[LogiLed::NUM_PLUS]		 = CGREEN_YELLOW;//{ 0.3f, 0.8f, 0 };
	m_raColorMap[LogiLed::TAB]				 = CYELLOW;
	m_raColorMap[LogiLed::Q]				 = CYELLOW;
	m_raColorMap[LogiLed::W]				 = CYELLOW;
	m_raColorMap[LogiLed::E]				 = CYELLOW;
	m_raColorMap[LogiLed::R]				 = CYELLOW;
	m_raColorMap[LogiLed::T]				 = CYELLOW;
	m_raColorMap[LogiLed::Y]				 = CYELLOW;
	m_raColorMap[LogiLed::U]				 = CYELLOW;
	m_raColorMap[LogiLed::I]				 = CYELLOW;
	m_raColorMap[LogiLed::O]				 = CYELLOW;
	m_raColorMap[LogiLed::P]				 = CYELLOW;
	m_raColorMap[LogiLed::OPEN_BRACKET]	 = CYELLOW;
	m_raColorMap[LogiLed::CLOSE_BRACKET]	 = CYELLOW;
	m_raColorMap[LogiLed::BACKSLASH]		 = CYELLOW;
	m_raColorMap[LogiLed::KEYBOARD_DELETE]	 = CYELLOW;
	m_raColorMap[LogiLed::END]				 = CYELLOW;
	m_raColorMap[LogiLed::PAGE_DOWN]		 = CYELLOW;
	m_raColorMap[LogiLed::NUM_SEVEN]		 = CYELLOW;
	m_raColorMap[LogiLed::NUM_EIGHT]		 = CYELLOW;
	m_raColorMap[LogiLed::NUM_NINE]		 = CYELLOW;
	m_raColorMap[LogiLed::TILDE]			 = CORANGE;
	m_raColorMap[LogiLed::ONE]				 = CORANGE;
	m_raColorMap[LogiLed::TWO]				 = CORANGE;
	m_raColorMap[LogiLed::THREE]			 = CORANGE;
	m_raColorMap[LogiLed::FOUR]			 = CORANGE;
	m_raColorMap[LogiLed::FIVE]			 = CORANGE;
	m_raColorMap[LogiLed::SIX]				 = CORANGE;
	m_raColorMap[LogiLed::SEVEN]			 = CORANGE;
	m_raColorMap[LogiLed::EIGHT]			 = CORANGE;
	m_raColorMap[LogiLed::NINE]			 = CORANGE;
	m_raColorMap[LogiLed::ZERO]			 = CORANGE;
	m_raColorMap[LogiLed::MINUS]			 = CORANGE;
	m_raColorMap[LogiLed::EQUALS]			 = CORANGE;
	m_raColorMap[LogiLed::BACKSPACE]		 = CORANGE;
	m_raColorMap[LogiLed::INSERT]			 = CORANGE;
	m_raColorMap[LogiLed::HOME]			 = CORANGE;
	m_raColorMap[LogiLed::PAGE_UP]			 = CORANGE;
	m_raColorMap[LogiLed::NUM_LOCK]		 = CORANGE;
	m_raColorMap[LogiLed::NUM_SLASH]		 = CORANGE;
	m_raColorMap[LogiLed::NUM_ASTERISK]	 = CORANGE;
	m_raColorMap[LogiLed::NUM_MINUS]		 = CORANGE;
	m_raColorMap[LogiLed::ESC]				 = CRED;
	m_raColorMap[LogiLed::F1]				 = CRED;
	m_raColorMap[LogiLed::F2]				 = CRED;
	m_raColorMap[LogiLed::F3]				 = CRED;
	m_raColorMap[LogiLed::F4]				 = CRED;
	m_raColorMap[LogiLed::F5]				 = CRED;
	m_raColorMap[LogiLed::F6]				 = CRED;
	m_raColorMap[LogiLed::F7]				 = CRED;
	m_raColorMap[LogiLed::F8]				 = CRED;
	m_raColorMap[LogiLed::F9]				 = CRED;
	m_raColorMap[LogiLed::F10]				 = CRED;
	m_raColorMap[LogiLed::F11]				 = CRED;
	m_raColorMap[LogiLed::F12]				 = CRED;
	m_raColorMap[LogiLed::PRINT_SCREEN]	 = CRED;
	m_raColorMap[LogiLed::SCROLL_LOCK]		 = CRED;
	m_raColorMap[LogiLed::PAUSE_BREAK]		 = CRED;
}

KEYRGB CLEDThread::ConvertColor(int key, float percent)
{
	if (key < 0)
		return { 0,0,0 };
	else if (key > MAX_KEYMAP)
		return { 0,0,0 };
	KEYRGB new_val = m_raColorMap[key];
	float new_r = int(new_val.r * percent * 100);
	float new_g = int(new_val.g * percent * 100);
	float new_b = int(new_val.b * percent * 100);
	return KEYRGB{ new_r, new_g, new_b };
}
