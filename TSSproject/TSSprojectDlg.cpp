
// TSSprojectDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TSSproject.h"
#include "TSSprojectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTSSprojectDlg dialog



CTSSprojectDlg::CTSSprojectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TSSPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTSSprojectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_fileList);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_staticImage);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_staticHistogram);
}

BEGIN_MESSAGE_MAP(CTSSprojectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CTSSprojectDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, &CTSSprojectDlg::OnFileClose)
	ON_MESSAGE(WM_DRAW_IMAGE, OnDrawImage)
	ON_MESSAGE(WM_DRAW_HISTOGRAM, OnDrawHist)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTSSprojectDlg message handlers


LRESULT CTSSprojectDlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT st = (LPDRAWITEMSTRUCT)wParam;

	//CDC* pDC = CDC::FromHandle(st->hDC);

	auto gr = Gdiplus::Graphics::FromHDC(st->hDC);

	//gr->DrawImage();


	return LRESULT();
}

LRESULT CTSSprojectDlg::OnDrawHist(WPARAM wParam, LPARAM lParam)
{


	return LRESULT();
}



BOOL CTSSprojectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTSSprojectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTSSprojectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTSSprojectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTSSprojectDlg::OnFileOpen()
{
	// File filter for image types
	CString filter = _T("Image Files (*.png;*.bmp;*.jpg)|*.png;*.bmp;*.jpg|All Files (*.*)|*.*||");

	// Create an open file dialog allowing multiple selection
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, filter, this);

	// Buffer for selected file paths
	const int maxFiles = 100;
	const int maxFilePath = 512;
	wchar_t fileBuffer[maxFiles * maxFilePath] = { 0 };  // Initialize with zeros

	dlg.m_ofn.lpstrFile = fileBuffer;
	dlg.m_ofn.nMaxFile = sizeof(fileBuffer) / sizeof(fileBuffer[0]);

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		int id = 0;
		while (pos != NULL)
		{
			CString filePath = dlg.GetNextPathName(pos);

			// Extract file name from the full path
			CString fileName = filePath.Mid(filePath.ReverseFind(_T('\\')) + 1);

			// Read file data (simplified, in practice, you would handle the image loading properly)
			CFile file;
			if (file.Open(filePath, CFile::modeRead))
			{
				ULONGLONG fileSize = file.GetLength();
				std::vector<BYTE> imageData(static_cast<size_t>(fileSize));
				file.Read(imageData.data(), static_cast<UINT>(fileSize));
				file.Close();

				int width = 100;   // Placeholder value for width
				int height = 100;  // Placeholder value for height
				CString pixelFormat = _T("RGB");  // Example pixel format

				// Create a new CustomImage object and add it to the vector
				CustomImage img(id++, fileName, imageData, width, height, pixelFormat, filePath);
				imageList.push_back(img);
			}
		}
	}

	// Update the file list control with the new images
	for (const auto& image : imageList)
	{
		m_fileList.InsertItem(0, image.Name);  // Add the image name to the list control
	}
}

void CTSSprojectDlg::OnFileClose()
{
	// TODO: Add your command handler code here
}


void CTSSprojectDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (::IsWindow(m_staticImage.m_hWnd) && ::IsWindow(m_staticHistogram.m_hWnd) && ::IsWindow(m_fileList.m_hWnd))
	{
		CRect rectHistogram;
		m_staticHistogram.GetWindowRect(&rectHistogram);
		ScreenToClient(&rectHistogram); 

		int histogramTop = cy - rectHistogram.Height() - 10; 
		m_staticHistogram.SetWindowPos(NULL, rectHistogram.left, histogramTop, rectHistogram.Width(), rectHistogram.Height(), SWP_NOZORDER);

		CRect rectFileList;
		m_fileList.GetWindowRect(&rectFileList);
		ScreenToClient(&rectFileList);

		int newFileListHeight = histogramTop - rectFileList.top - 10;
		m_fileList.SetWindowPos(NULL, rectFileList.left, rectFileList.top, rectFileList.Width(), newFileListHeight, SWP_NOZORDER);

		CRect rectImage;
		m_staticImage.GetWindowRect(&rectImage);
		ScreenToClient(&rectImage);

		int newImageWidth = cx - rectFileList.Width() - 35;  
		int newImageHeight = newFileListHeight + rectHistogram.Height() + 10;

		m_staticImage.SetWindowPos(NULL, rectFileList.right + 10, rectImage.top, newImageWidth, newImageHeight, SWP_NOZORDER);
	}

	Invalidate(TRUE);
}

void CStaticImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	GetParent()->SendMessage(WM_DRAW_IMAGE, (WPARAM)lpDrawItemStruct);

}

void CStaticHistogram::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GetParent()->SendMessage(WM_DRAW_HISTOGRAM, (WPARAM)lpDrawItemStruct);
}
