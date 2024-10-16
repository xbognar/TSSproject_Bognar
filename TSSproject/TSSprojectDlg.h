
// TSSprojectDlg.h : header file
//
#include <iostream>
#include <vector>
#include <algorithm>
#include "CustomImage.h"

#pragma once
using namespace std;


enum
{
	WM_DRAW_IMAGE = WM_USER + 1,
	WM_DRAW_HISTOGRAM
};

class CStaticImage : public CStatic
{
public:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};

class CStaticHistogram : public CStatic
{
public:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};


// CTSSprojectDlg dialog
class CTSSprojectDlg : public CDialogEx
{
// Construction
public:
	CTSSprojectDlg(CWnd* pParent = nullptr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TSSPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


// Implementation
protected:
	HICON m_hIcon;

	std::vector<CustomImage> imageList;
	int selectedIndex = -1;

	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHist(WPARAM wParam, LPARAM lParam);

	void OnFileListSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CListCtrl m_fileList;
	CStaticImage m_staticImage;
	CStaticHistogram m_staticHistogram;
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileDelete();
};
