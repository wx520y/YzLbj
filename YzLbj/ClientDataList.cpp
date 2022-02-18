// ClientDataList.cpp: implementation of the CClientDataList class.
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


CClientDataList::CClientDataList()
{	
	nCount = 0;
	pData = new CClientData[MAX_CONNECT];
}

CClientDataList::~CClientDataList()
{
	delete []pData;
}

void CClientDataList::Clear()
{
	nCount = 0;
}

int CClientDataList::GetCount()
{
	return nCount;
}

BOOL CClientDataList::Add(CClientData ClientData)
{
	if(nCount >= MAX_CONNECT)
		return FALSE;

	pData[nCount] = ClientData;
	nCount ++;

	return TRUE;
}

BOOL CClientDataList::RemoveBySocket(SOCKET s,CClientData& Data,int& nPos)
{
	for(int i=0; i<nCount; i++)
	{
		if(pData[i].s == s)
		{
			Data = pData[i];
			for(int j=i; j<nCount-1; j++)
				pData[j] = pData[j+1];
			nCount --;
			nPos = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CClientDataList::GetClientDataBySocket(SOCKET s,CClientData& Data,int& nPos)
{	
	for(int i=0; i<nCount; i++)
	{
		if(pData[i].s == s)
		{
			nPos = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CClientDataList::SetItem(int Index,CClientData ClientData)
{
	if((Index < 0 ) || Index > nCount-1)
		return FALSE;

	pData[Index] = ClientData;

	return TRUE;
}

BOOL CClientDataList::GetItem(int Index,CClientData& ClientData)
{
	if((Index < 0 ) || Index > nCount-1)
		return FALSE;

	ClientData = pData[Index];

	return TRUE;
}

