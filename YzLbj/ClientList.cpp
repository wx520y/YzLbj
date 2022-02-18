// ClientList.cpp: implementation of the CClientList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCPServer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
		制作着:苍竹先生
		E-mail:cangzhu@163.com
		时间:2008-11-15
		版本:1.0.0.1
*/


CClientList::CClientList()
{
	nCount = 0;
	pData = new CClient[MAX_CONNECT];
}

CClientList::~CClientList()
{
	delete []pData;
}

void CClientList::Clear()
{
	nCount = 0;
}

int CClientList::GetCount()
{
	return nCount;
}

BOOL CClientList::Add(CClient Client)
{
	if(nCount >= MAX_CONNECT)
		return FALSE;

	pData[nCount] = Client;
	nCount ++;

	return TRUE;
}

BOOL CClientList::RemoveBySocket(SOCKET s,CClient& Client,int& nPos)
{
	for(int i=0; i<nCount; i++)
	{
		if(pData[i].s == s)
		{
			Client = pData[i];
			for(int j=i; j<nCount-1; j++)
				pData[j] = pData[j+1];
			nCount --;
			nPos = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CClientList::GetClientBySocket(SOCKET s,CClient& Client,int& nPos)
{	
	for(int i=0; i<nCount; i++)
	{
		if(pData[i].s == s)
		{
			Client = pData[i];
			nPos = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CClientList::SetItem(int Index,CClient Client)
{
	if((Index < 0 ) || Index > nCount-1)
		return FALSE;

	pData[Index] = Client;

	return TRUE;
}

BOOL CClientList::GetItem(int Index,CClient& Client)
{
	if((Index < 0 ) || Index > nCount-1)
		return FALSE;

	Client = pData[Index];

	return TRUE;
}
