#include"audio_clip.h"

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")

BOOL CAudioClip::open(/*const CString _filename*/)
{
	CString strfilename = filename;
	for (unsigned int i = 0; i < strfilename.GetLength(); i++)
	{
		if (strfilename[i] == '/')
			strfilename.SetAt(i, '\\');
	}
	CString cmd;
	cmd.Format(_T("open %s  alias %s"), strfilename, alias);
	return Send(cmd);
}

BOOL CAudioClip::Send(CString command)
{
	int errcode = mciSendString(command, strBuf.GetBuffer(256), 254, 0);
	strBuf.ReleaseBuffer();
	if (errcode)
	{
		mciGetErrorString(errcode, strBuf.GetBuffer(256), 254);
		strBuf.ReleaseBuffer();
		return FALSE;
	}
	return TRUE;
}

CAudioClip::CAudioClip()
{
	InitAudioClip();

}

CAudioClip::~CAudioClip()
{
	close();
}

void CAudioClip::InitAudioClip(CString filename, CString alias)
{
	this->filename = filename;
	this->alias = alias;
	open();
}

BOOL CAudioClip::play()
{
	CString cmd;
	cmd.Format(_T("play %s repeat"), alias);
	return Send(cmd);

}

BOOL CAudioClip::stop()
{
	CString cmd;
	cmd.Format(_T("stop %s"), alias);
	return Send(cmd);
	//cmd = "seek " + alias + " to start";
	//cmd.Format(_T("seek %s to start"), alias);
	//Send(cmd);
}

BOOL CAudioClip::pause()
{
	CString cmd;
	cmd.Format(_T("pause %s"), alias);
	return Send(cmd);
}

BOOL CAudioClip::unpause()
{
	CString cmd;
	cmd.Format(_T("resume %s"), alias);
	return Send(cmd);
}

BOOL CAudioClip::close()
{
	CString cmd;
	cmd.Format(_T("close %s"), alias);
	return Send(cmd);
}
