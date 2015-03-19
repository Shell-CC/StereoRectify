
// RectifyDlgDlg.h : ͷ�ļ�
//

#pragma once

#include <opencv2\core\core.hpp>
#include <opencv2\nonfree\features2d.hpp>

// CRectifyDlgDlg �Ի���
class CRectifyDlgDlg : public CDialogEx
{
// ����
public:
	CRectifyDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RECTIFYDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ͼ������
public:
	cv::Mat srcImg_1, srcImg_2, srcImg_merge;
	cv::Mat Img_1, Img_2;
	cv::Mat keypointImg_1, keypointImg_2;
	cv::Mat matchesImg;
	cv::Mat InlinerMatchesImg;
	cv::Mat RectifyImg_1, RectifyImg_2, RectifyImg_merge;

	// ������ƥ������
public:
	std::vector<cv::KeyPoint> Keypoint_1, Keypoint_2;
	std::vector<cv::KeyPoint> Inliner_1, Inliner_2;
	std::vector<cv::Point2f> mInliner_1, mInliner_2;

	cv::Mat mDescriptor_1, mDescriptor_2;

	std::vector<cv::DMatch> matches;

	int ptCount, OutlinerCount, InlinerCount;

	// У������
public:
	cv::Mat mFundamental;
	cv::Mat mHomography_1, mHomography_2;
	cv::Mat mRectify_1, mRectify_2;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadImg1();
	afx_msg void OnLoadImg2();
	afx_msg void OnBnClickedDetector();
	afx_msg void OnBnClickedMatcher();
	afx_msg void OnGetFoundamentalMat();
	afx_msg void OnBnClickedRectify();

	// ͼ������
	void DrawImgToHDC( cv::Mat img, UINT IDC_PIC );
	afx_msg void OnBnClickedDepth();
};
