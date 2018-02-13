#pragma once
#include <bass.h> //BASS 2.4.13.8
struct KEYRGB {
	float r;
	float g;
	float b;
	bool operator!=(KEYRGB& op) {
		return op.r != this->r || op.g != this->g || op.b != this->b;
	}
	
};
#define MAX_KEYMAP 500
class CLEDThread
{
public:
	CLEDThread();
	~CLEDThread();
	void run();

	BOOL m_bShouldReload;
	void LoadConfig();
	void SaveConfig();
	void SetDevId(int id);
	int GetDevId();
private:
	static int m_iaaKeymap[22][6];
	float m_faPrevColor[22];
	int m_iRecordDevId;
	static struct KEYRGB m_raColorMap[MAX_KEYMAP];
	void ReloadSetting();
	void ReloadDevice();
	void CleanBASS();
	void ResetColor();
	void Step();
	
private:
	static int _GetDeviceCount();
	static void _InitColorMap();
	static KEYRGB ConvertColor(int key, float percent);
	
	
	HRECORD m_hChannelHandle;	// recording channel

	void Draw(float[22]);

	static const int __SPECHEIGHT__;
	static const int __SPECWIDTH__;
	BYTE m_BaSpecBuf[128][368];
};

