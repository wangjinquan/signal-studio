// Copyleft 2017 Akshay.C.S. Based on works by Chris Korda and others.
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or any later version.

// PluginParamControl.cpp : implementation file

#include "stdafx.h"
#include "Resource.h"
#include "PluginParamControl.h"
#include "PluginParamsViewer.h"
#include "ladspa.h"
#include "WinAppEx.h"	// for GetLastErrorString
#include "shlwapi.h"	// for PathFindFileName

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluginParamControl dialog

const CControlResizer::CTRL_LIST CPluginParamControl::m_CtrlList[] = {
	{IDC_PLUGIN_VIEW,	BIND_ALL},
	{IDC_PLUGIN_RESET,	BIND_BOTTOM},
	{IDC_PLUGIN_ABOUT,	BIND_BOTTOM},
	{IDOK,				BIND_RIGHT | BIND_BOTTOM},
	{IDCANCEL,			BIND_RIGHT | BIND_BOTTOM},
	{0}	// list terminator
};

CPluginParamControl::CPluginParamControl(LPCTSTR Path, UINT SampleRate, CPlugin::CParamArray& Param, CWnd* pParent /*=NULL*/)
	: CCodePersist(IDD, 0, _T("PluginParamControl"), pParent), m_PluginPath(Path), m_Param(Param)
{
	//{{AFX_DATA_INIT(CPluginParamControl)
	//}}AFX_DATA_INIT
	m_InitSize = CSize(0, 0);
	m_SampleRate = SampleRate;
	m_View = NULL;
}

void CPluginParamControl::GetViewHolderRect(CRect& Rect) const
{
	GetDlgItem(IDC_PLUGIN_VIEW)->GetWindowRect(Rect);	// get view placeholder rect
	ScreenToClient(Rect);
}

bool CPluginParamControl::CreateView()
{
	// create child view that contains parameter controls
	CRuntimeClass	*pFactory = RUNTIME_CLASS(CPluginParamsViewer);
	m_View = DYNAMIC_DOWNCAST(CPluginParamsViewer, pFactory->CreateObject());
	if (m_View == NULL) {
		AfxMessageBox(IDS_PLUGIN_CANT_CREATE_VIEW);
		return(FALSE);
	}
	CRect	rView;
	GetViewHolderRect(rView);
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_BORDER;
    if (!m_View->Create(NULL, NULL, dwStyle, rView, this, 0, NULL)) {
		AfxMessageBox(IDS_PLUGIN_CANT_CREATE_VIEW);
		return(FALSE);
	}
	if (!m_View->InitRows(m_PluginPath, m_SampleRate)) {
		CString	msg;
		AfxFormatString2(msg, IDS_PLUGIN_CANT_LOAD, m_PluginPath,
			CWinAppEx::GetLastErrorString());
		AfxMessageBox(msg);
		return(FALSE);
	}
	if (m_Param.GetSize())
		m_View->SetParams(m_Param);
	SetWindowText(CString(m_View->GetDescriptor()->Name));
	return(TRUE);
}

void CPluginParamControl::DoDataExchange(CDataExchange* pDX)
{
	CCodePersist::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginParamControl)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPluginParamControl, CCodePersist)
	//{{AFX_MSG_MAP(CPluginParamControl)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_PLUGIN_RESET, OnReset)
	ON_BN_CLICKED(IDC_PLUGIN_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginParamControl message handlers

BOOL CPluginParamControl::OnInitDialog() 
{
	CCodePersist::OnInitDialog();
	// save initial size
	CRect	rc;
	GetWindowRect(rc);
	m_InitSize = rc.Size();
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), 0);	// set app icon
	m_Resize.AddControlList(this, m_CtrlList);	// add controls to resizer
	if (!CreateView())	// create parameters view
		EndDialog(IDABORT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPluginParamControl::OnSize(UINT nType, int cx, int cy) 
{
	CCodePersist::OnSize(nType, cx, cy);
	m_Resize.OnSize();	// resize our controls, including view placeholder
	if (m_View != NULL) {	// if view exists
		CRect	rView;
		GetViewHolderRect(rView);
		m_View->MoveWindow(rView);	// resize view to match placeholder
	}
}

void CPluginParamControl::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize = CPoint(m_InitSize);
	CCodePersist::OnGetMinMaxInfo(lpMMI);
}

void CPluginParamControl::OnOK() 
{
	CCodePersist::OnOK();
	if (m_View != NULL)	// if view exists
		m_View->GetParams(m_Param);	// copy parameters from view to caller's array
}

void CPluginParamControl::OnReset() 
{
	if (m_View != NULL)	// if view exists
		m_View->RestoreDefaults();	// restore parameters to their default values
}

void CPluginParamControl::OnAbout() 
{
	if (m_View != NULL) {	// if view exists
		const LADSPA_Descriptor	*desc = m_View->GetDescriptor();
		CPlugin::CPortStats	PortStats;
		CPlugin::GetPortStats(desc, PortStats);
		CString	msg;
		msg.Format(IDS_PLUGIN_ABOUT,
			PathFindFileName(m_PluginPath),
			desc->UniqueID,
			CString(desc->Label),
			CString(desc->Name),
			CString(desc->Maker),
			CString(desc->Copyright),
			PortStats.m_AudioIn.GetSize(),
			PortStats.m_AudioOut.GetSize()
		);
		AfxMessageBox(msg, MB_ICONINFORMATION);
	}
}
