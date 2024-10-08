
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
	ON_COMMAND(ID_FILE_DELETE, &CTSSprojectDlg::OnFileDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CTSSprojectDlg::OnFileListSelectionChanged)
END_MESSAGE_MAP()

void CTSSprojectDlg::OnFileListSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		selectedIndex = pNMLV->iItem; // Update selected index
		Invalidate(FALSE); // Redraw with the new selection
	}

	*pResult = 0;
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

HCURSOR CTSSprojectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTSSprojectDlg::OnFileOpen()
{
	// File dialog logic remains the same
	CString filter = _T("Image Files (*.png;*.bmp;*.jpg)|*.png;*.bmp;*.jpg|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, filter, this);

	const int maxFiles = 100;
	const int maxFilePath = 512;
	wchar_t fileBuffer[maxFiles * maxFilePath] = { 0 };
	dlg.m_ofn.lpstrFile = fileBuffer;
	dlg.m_ofn.nMaxFile = maxFiles * maxFilePath;

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		int id = imageList.size();

		while (pos != NULL)
		{
			CString filePath = dlg.GetNextPathName(pos);
			CString fileName = filePath.Mid(filePath.ReverseFind(_T('\\')) + 1);

			// Check for duplicates
			if (std::any_of(imageList.begin(), imageList.end(), [&](const CustomImage& img) { return img.Path == filePath; }))
				continue;

			Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(filePath);
			if (bitmap != nullptr && bitmap->GetLastStatus() == Gdiplus::Ok)
			{
				int width = bitmap->GetWidth();
				int height = bitmap->GetHeight();
				CString pixelFormat = _T("RGB");
				CustomImage img(id++, fileName, bitmap, width, height, pixelFormat, filePath);
				imageList.push_back(img);
			}
			else
			{
				delete bitmap;
				AfxMessageBox(_T("Failed to load image."));
			}
		}
	}

	// Update list control
	m_fileList.DeleteAllItems();
	for (const auto& image : imageList)
	{
		int index = m_fileList.GetItemCount();
		m_fileList.InsertItem(index, image.Name);
	}

	// Automatically load and display the first image if any images were loaded
	if (!imageList.empty())
	{
		selectedIndex = 0;
		m_fileList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		Invalidate(FALSE); // Redraw to display the first image
	}
}

void CTSSprojectDlg::OnFileDelete()
{
	int selectedItem = m_fileList.GetNextItem(-1, LVNI_SELECTED);

	if (selectedItem == -1)
		return;

	CString fileName = m_fileList.GetItemText(selectedItem, 1);
	CString confirmationMessage;
	confirmationMessage.Format(_T("Are you sure you want to delete the file: %s?"), fileName);

	if (AfxMessageBox(confirmationMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		imageList.erase(imageList.begin() + selectedItem);
		m_fileList.DeleteItem(selectedItem);

		selectedIndex = (m_fileList.GetItemCount() > 0) ? 0 : -1;
		if (selectedIndex >= 0)
		{
			m_fileList.SetItemState(selectedIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		Invalidate(FALSE); // Redraw the image
	}
}

void CTSSprojectDlg::OnFileClose()
{
	OnOK();
}

void CTSSprojectDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (::IsWindow(m_staticImage.m_hWnd) && ::IsWindow(m_staticHistogram.m_hWnd) && ::IsWindow(m_fileList.m_hWnd))
	{
		// Constants to define spacing and layout
		const int padding = 10;  // Padding between elements
		const int columnWidth = 250;  // Fixed width of the left column (file list + histogram)
		const int histogramSize = 250;  // Fixed size of the histogram (250x250)

		// Position and size the histogram in the bottom left corner
		int histogramX = padding;  // Keep it at the left
		int histogramY = cy - histogramSize - padding;  // Bottom left corner
		m_staticHistogram.SetWindowPos(NULL, histogramX, histogramY, histogramSize, histogramSize, SWP_NOZORDER);

		// Position and size the file list above the histogram
		int fileListHeight = histogramY - (2 * padding);  // File list height is dynamic, taking space above the histogram
		m_fileList.SetWindowPos(NULL, padding, padding, columnWidth, fileListHeight, SWP_NOZORDER);

		// Calculate the size and position for the static image on the right
		int imageX = columnWidth + (2 * padding);  // Start after the left column
		int imageWidth = cx - imageX - padding;  // Remaining width for the image
		int imageHeight = cy - (2 * padding);  // Remaining height for the image

		// Position and size the image control
		m_staticImage.SetWindowPos(NULL, imageX, padding, imageWidth, imageHeight, SWP_NOZORDER);
	}

	Invalidate(FALSE);  // Force a redraw to reflect the changes
}

void CStaticImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	GetParent()->SendMessage(WM_DRAW_IMAGE, (WPARAM)lpDrawItemStruct);

}

void CStaticHistogram::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GetParent()->SendMessage(WM_DRAW_HISTOGRAM, (WPARAM)lpDrawItemStruct);
}

LRESULT CTSSprojectDlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	if (selectedIndex < 0 || selectedIndex >= imageList.size())
		return LRESULT();

	LPDRAWITEMSTRUCT lpDrawItemStruct = reinterpret_cast<LPDRAWITEMSTRUCT>(wParam);
	Gdiplus::Graphics graphics(lpDrawItemStruct->hDC);

	CRect rect;
	m_staticImage.GetClientRect(&rect);
	graphics.Clear(Gdiplus::Color(255, 255, 255, 255));

	CustomImage& selectedImage = imageList[selectedIndex];
	Gdiplus::Bitmap* bitmap = selectedImage.Bitmap;

	if (bitmap != nullptr && bitmap->GetLastStatus() == Gdiplus::Ok)
	{
		int imgWidth = bitmap->GetWidth();
		int imgHeight = bitmap->GetHeight();
		int destWidth = rect.Width();
		int destHeight = rect.Height();

		float aspectRatio = static_cast<float>(imgWidth) / imgHeight;
		if (destWidth / aspectRatio <= destHeight)
		{
			destHeight = static_cast<int>(destWidth / aspectRatio);
		}
		else
		{
			destWidth = static_cast<int>(destHeight * aspectRatio);
		}

		int offsetX = (rect.Width() - destWidth) / 2;
		int offsetY = (rect.Height() - destHeight) / 2;

		graphics.DrawImage(bitmap, offsetX, offsetY, destWidth, destHeight);
	}

	return LRESULT();
}


LRESULT CTSSprojectDlg::OnDrawHist(WPARAM wParam, LPARAM lParam)
{


	return LRESULT();
}
