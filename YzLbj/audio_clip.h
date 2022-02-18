#ifndef AUDIO_CLIP_H
#define AUDIO_CLIP_H
#include "stdafx.h"
#include<Windows.h>

class CAudioClip
{
private:
	CString strBuf;
	char buf[256];
	CString filename;
	CString alias;
	int length_ms;
	BOOL open(/*const  CString _filename*/);
	BOOL Send(CString command);
public:
	CAudioClip();
	~CAudioClip();
	void InitAudioClip(CString filename = _T(""), CString alias = _T("media"));
	
	BOOL play(/*int start_ms=0, int end_ms=-1*/);
	BOOL stop();
	BOOL pause();
	BOOL unpause();
	BOOL close();
	int milliseconds();
};

#endif
