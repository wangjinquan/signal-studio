// Copyleft 2017 Akshay.C.S. Based on works by Chris Korda and others.
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or any later version.

#if !defined(AFX_CodeSearch_H__7AC939AA_8FC7_46F8_AFA6_AE0C528A46FC__INCLUDED_)
#define AFX_CodeSearch_H__7AC939AA_8FC7_46F8_AFA6_AE0C528A46FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

// CodeSearch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeSearch dialog

class CCodeSearch : public CDialog
{
	DECLARE_DYNAMIC(CCodeSearch);
public:
// Construction
	CCodeSearch(LPCTSTR Folder = NULL, CWnd* pParent = NULL);
	virtual ~CCodeSearch();

// Constants
	enum DRIVE_TYPE_MASK {
		DTM_UNKNOWN		= (1 << DRIVE_UNKNOWN),		// The drive type cannot be determined. 
		DTM_NO_ROOT_DIR	= (1 << DRIVE_NO_ROOT_DIR),	// The root directory does not exist. 
		DTM_REMOVABLE	= (1 << DRIVE_REMOVABLE),	// The disk can be removed from the drive. 
		DTM_FIXED		= (1 << DRIVE_FIXED),		// The disk cannot be removed from the drive. 
		DTM_REMOTE		= (1 << DRIVE_REMOTE),		// The drive is a remote (network) drive. 
		DTM_CDROM		= (1 << DRIVE_CDROM),		// The drive is a CD-ROM drive. 
		DTM_RAMDISK		= (1 << DRIVE_RAMDISK),		// The drive is a RAM disk. 
		DTM_ALL			= -1
	};

// Attributes
	void	SetDriveTypeMask(int Mask);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
// Dialog data
	//{{AFX_DATA(CCodeSearch)
	enum { IDD = IDD_FILE_SEARCH_DLG };
	CStatic	m_CurrentPath;
	//}}AFX_DATA

// Generated message map functions
	//{{AFX_MSG(CCodeSearch)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnTimer(W64UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnSearchDone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPathReply(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// Constants
	enum {
		PATH_TIMER_ID = 1789,		// path timer ID; vive la revolution
		PATH_TIMER_PERIOD = 500,	// path timer period, in milliseconds
	};
	enum {
		UWM_FIRST = WM_USER,
		UWM_SEARCH_DONE,			// posted by worker thread when it exits
		UWM_PATH_REPLY,				// posted by worker thread in reply to path
									// request; WPARAM is pointer to path string
									// allocated via strdup, which recipient is 
									// responsible for freeing to avoid a leak
	};

// Member data
	CString	m_Folder;
	volatile bool	m_Cancel;		// true if user canceled search
	volatile bool	m_PathRequest;	// true if path request pending
	int		m_DriveTypeMask;		// drive types to be searched

// Helpers
	bool	SearchFolder(LPCTSTR Folder);
	bool	SearchAllDrives();
	void	Search();
	static	UINT	SearchThread(LPVOID pParam);
	virtual	bool	OnFile(LPCTSTR Path) = 0;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CodeSearch_H__7AC939AA_8FC7_46F8_AFA6_AE0C528A46FC__INCLUDED_)
