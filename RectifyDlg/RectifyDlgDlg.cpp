
// RectifyDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RectifyDlg.h"
#include "RectifyDlgDlg.h"
#include "afxwin.h"
#include "afxdialogex.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRectifyDlgDlg 对话框




CRectifyDlgDlg::CRectifyDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRectifyDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRectifyDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRectifyDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_READIMG1, &CRectifyDlgDlg::OnLoadImg1)
	ON_BN_CLICKED(IDC_READIMG2, &CRectifyDlgDlg::OnLoadImg2)
	ON_BN_CLICKED(IDC_DETECTOR, &CRectifyDlgDlg::OnBnClickedDetector)
	ON_BN_CLICKED(IDC_MATCHER, &CRectifyDlgDlg::OnBnClickedMatcher)
	ON_BN_CLICKED(IDC_FMAT, &CRectifyDlgDlg::OnGetFoundamentalMat)
	ON_BN_CLICKED(IDC_RECTIFY, &CRectifyDlgDlg::OnBnClickedRectify)
	ON_BN_CLICKED(IDC_DEPTH, &CRectifyDlgDlg::OnBnClickedDepth)
END_MESSAGE_MAP()


// CRectifyDlgDlg 消息处理程序

BOOL CRectifyDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRectifyDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRectifyDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRectifyDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRectifyDlgDlg::DrawImgToHDC( Mat img, UINT IDC_PIC )
{
	CWnd *pWnd = CWnd::GetDlgItem( IDC_PIC );    // 获取pic控件窗口句柄
	CRect rect;
	pWnd->GetClientRect( &rect );                                              // 获取pic控件所在的矩形区域
	CDC *pDC = pWnd->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CvvImage cimg;
	IplImage *iplImg = ( &(IplImage)img ); 
	cimg.CopyOf( iplImg );
	cimg.DrawToHDC( hDC, &rect );                                           // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
}


void CRectifyDlgDlg::OnLoadImg1()
{
	// 打开文件对话框
	CFileDialog dlgLoadImg(TRUE, NULL, "*.bmp");
	if(dlgLoadImg.DoModal() != IDOK)
		return;

	// 获得文件名
	CString ImgName = dlgLoadImg.GetPathName();
	std::string tempName = (LPCSTR)CStringA(ImgName);
	const char* tmp = tempName.c_str();

	// 读取图像
	srcImg_1 = imread(tmp, 1);
	if(!srcImg_1.data)
	{ 
		MessageBox("Could not load the Image!");
		return;
	}

	// 显示图像
	//imshow("Left Image", srcImg_1);
	DrawImgToHDC( srcImg_1, IDC_SRCIMG1 );

	// 转换成灰度图
	cvtColor(srcImg_1, Img_1, CV_BGR2GRAY);

	//ImgSize = Size(Img_1.cols, Img_2.rows);
}


void CRectifyDlgDlg::OnLoadImg2()
{
	// 打开文件对话框
	CFileDialog dlgLoadImg(TRUE, NULL, "*.bmp");
	if(dlgLoadImg.DoModal() != IDOK)
		return;

	// 获得文件名
	CString ImgName = dlgLoadImg.GetPathName();
	std::string tempName = (LPCSTR)CStringA(ImgName);
	const char* tmp = tempName.c_str();

	// 读取图像
	srcImg_2 = imread(tmp, 1);
	if(!srcImg_2.data)
	{ 
		MessageBox("Could not load the Image!");
		return;
	}

	// 显示图像
	//imshow("Right Image", srcImg_2);
	DrawImgToHDC( srcImg_2, IDC_SRCIMG2 );

	// 转换成灰度图
	cvtColor(srcImg_2, Img_2, CV_BGR2GRAY);

	// 合成红蓝图像
	vector<Mat>  mv(3);
	mv[0] = Img_1;
	mv[1] = Img_1;
	mv[2] = Img_2;
	merge(mv, srcImg_merge);
	DrawImgToHDC( srcImg_merge, IDC_SRCMERGE );

	if(!srcImg_merge.data)
	{ 
		MessageBox("Could not merge the Image!");
		return;
	}

	/****************
	if( ImgSize != Size(Img_2.cols, Img_2.rows) )
	{
		MessageBox("请读取相同大小的左右视点图像");
		return;
	}
	***********************/
}


void CRectifyDlgDlg::OnBnClickedDetector()
{
	if( !Img_1.data || !Img_2.data)
	{
		MessageBox("请先读取左右视点图像");
		return;
	}
	
	// Detect the keypoints using SURF Detector

	int minHessian = 400;
	SurfFeatureDetector detector( minHessian );

	detector.detect( Img_1, Keypoint_1 );
	detector.detect( Img_2, Keypoint_2 );

	// Draw and show the key points
	drawKeypoints(Img_1, Keypoint_1, keypointImg_1, Scalar(40,0,0),0);
	drawKeypoints(Img_2, Keypoint_2, keypointImg_2, Scalar(40,0,0),0);

	//imshow("Left Image", keypointImg_1);
	//imshow("Right Image", keypointImg_2);
	DrawImgToHDC( keypointImg_1, IDC_SRCIMG1 );
	DrawImgToHDC( keypointImg_2, IDC_SRCIMG2 );

	// Calculate descriptors

	SurfDescriptorExtractor extractor;

	extractor.compute( Img_1, Keypoint_1, mDescriptor_1 );
	extractor.compute( Img_2, Keypoint_2, mDescriptor_2 );
}


void CRectifyDlgDlg::OnBnClickedMatcher()
{
	// 4.Match descriptor vectors with a brute force matcher
	if( Keypoint_1.empty() || Keypoint_2.empty() )
	{
		MessageBox("特征点没有成功提取");
		return;
	}

	if( !mDescriptor_1.data || !mDescriptor_2.data )
	{
		MessageBox("特征点没有成功描述");
		return;
	}

	BFMatcher matcher (NORM_L2);

	matcher.match(mDescriptor_1, mDescriptor_2, matches);

	ptCount = (int)matches.size();

	 // Draw and show Matches
	 drawMatches( Img_1, Keypoint_1, Img_2, Keypoint_2, matches, matchesImg);
	 //imshow("Matches", matchesImg);
	 DrawImgToHDC( matchesImg, IDC_MATCHIMG );
}


void CRectifyDlgDlg::OnGetFoundamentalMat()
{
	 // 5.Estimate foundamental matrix using the RANSAC algrithom

	 // Convert KeyPoint into Mat
	 Mat mKeypoint_1(ptCount, 2, CV_32F);
	 Mat mKeypoint_2(ptCount, 2, CV_32F);

	 Point2f pt;
	 for(int i = 0; i < ptCount; i++)
	 {
		 pt = Keypoint_1[matches[i].queryIdx].pt;
		 mKeypoint_1.at<float>(i, 0) = pt.x;
		 mKeypoint_1.at<float>(i, 1) = pt.y;

		 pt = Keypoint_2[matches[i].trainIdx].pt;
		 mKeypoint_2.at<float>(i, 0) = pt.x;
		 mKeypoint_2.at<float>(i, 1) = pt.y;
	 }

	 vector<uchar> RANSACStatus;    // 0 for outliers and 1 for the other points
	 mFundamental = findFundamentalMat(mKeypoint_1, mKeypoint_2, FM_RANSAC, 3, 0.99, RANSACStatus);

	 // Calculate the outliers
	 OutlinerCount = 0;
	 for( int i = 0; i < ptCount; i++)
	 {
		 if( RANSACStatus[i] == 0 )
		 {
			 OutlinerCount++;
		 }
	 }

	 // Calculate the inliers
	 InlinerCount = ptCount - OutlinerCount;

	 std::vector<DMatch> InlinerMatches;

	 mInliner_1.resize(InlinerCount);
	 mInliner_2.resize(InlinerCount);
	 InlinerMatches.resize(InlinerCount);

	 InlinerCount = 0;

	 for(int i = 0; i < ptCount; i++)
	 {
		 if(RANSACStatus[i] != 0)
		 {
			 mInliner_1[InlinerCount].x = mKeypoint_1.at<float>(i, 0);
			 mInliner_1[InlinerCount].y = mKeypoint_1.at<float>(i, 1);
			 mInliner_2[InlinerCount].x = mKeypoint_2.at<float>(i, 0);
			 mInliner_2[InlinerCount].y = mKeypoint_2.at<float>(i, 1);

			 InlinerMatches[InlinerCount].queryIdx = InlinerCount;
			 InlinerMatches[InlinerCount].trainIdx = InlinerCount;

			 InlinerCount++;
		 }
	 }

	 // Convert Mat into KeyPoint
	 KeyPoint::convert(mInliner_1, Inliner_1);
	 KeyPoint::convert(mInliner_2, Inliner_2);

	 // Draw and show inliner matches
	 drawMatches( Img_1, Inliner_1, Img_2, Inliner_2, InlinerMatches, InlinerMatchesImg);
	 //imshow("Matches", InlinerMatchesImg);
	DrawImgToHDC( InlinerMatchesImg, IDC_MATCHIMG );
}


void CRectifyDlgDlg::OnBnClickedRectify()
{
	Size ImgSize( Img_1.cols, Img_1.rows);   // 此处可改进
	// Computes the rectification homography matrix using Hartley algorithm

	stereoRectifyUncalibrated(Mat(mInliner_1), Mat(mInliner_2), mFundamental, ImgSize, mHomography_1, mHomography_2);

	// 
	Mat Mrect = Mat::eye(3, 3, CV_64F);
	Mat invMrect =Mrect.inv(DECOMP_SVD);

	mRectify_1 = invMrect * mHomography_1 * Mrect;
	mRectify_2 = invMrect * mHomography_2 * Mrect;

	Mat LeftMapx, LeftMapy;
	Mat RightMapx, RightMapy;
	Mat distCoeffs;
	
	initUndistortRectifyMap(Mrect, distCoeffs, mRectify_1, Mrect, ImgSize, CV_32FC1, LeftMapx, LeftMapy);
	initUndistortRectifyMap(Mrect, distCoeffs, mRectify_2, Mrect, ImgSize, CV_32FC1, RightMapx, RightMapy);

	// 
	remap(Img_1, RectifyImg_1, LeftMapx, LeftMapy, INTER_LINEAR);
	remap(Img_2, RectifyImg_2, RightMapx, RightMapy, INTER_LINEAR);

	// 合成红蓝图像
	vector<Mat>  mv(3);
	mv[0] = RectifyImg_1;
	mv[1] = RectifyImg_1;
	mv[2] = RectifyImg_2;
	merge(mv, RectifyImg_merge);

	// 显示校正后的图像
	//imshow("Left Rectified Image", RectifyImg_1);
	//imshow("Right Recttified Image", RectifyImg_2);	
	DrawImgToHDC( RectifyImg_1, IDC_DSTIMG1 );
	DrawImgToHDC( RectifyImg_2, IDC_DSTIMG2 );
	DrawImgToHDC( RectifyImg_merge, IDC_DSTMERGE );
}


void CRectifyDlgDlg::OnBnClickedDepth()
{
	Mat dispImg, dispBM;

	 StereoBM bm;
	 bm(RectifyImg_1, RectifyImg_2, dispBM, CV_16S);
	 dispBM.convertTo(dispImg, CV_8U);
	 
	 //imshow("Depth", dispImg);
	DrawImgToHDC( dispImg, IDC_DEPTHIMG );
}


