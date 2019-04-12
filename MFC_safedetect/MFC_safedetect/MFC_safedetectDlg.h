
// MFC_safedetectDlg.h : 头文件
//

#pragma once
#include "cv.h"
#include "afxwin.h"
#include "highgui.hpp"
#include <vector>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include "mscomm1.h"

using namespace std;
using namespace cv;



// CMFC_safedetectDlg 对话框
class CMFC_safedetectDlg : public CDialogEx
{
// 构造
public:
	CMFC_safedetectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_SAFEDETECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadpicture();
	void ShowImage(IplImage * image, UINT ID);
	IplImage *src;//源图像
    //原始图
	Mat  gray_img, thresh_img;
	IplImage *src_gray;//源图像灰度图
	cv::Mat mat_src;
	cv::Mat mat_src_gray;
	afx_msg void OnBnClickedGray();
	afx_msg void OnBnClickedEqual();
	afx_msg void OnBnClickedButtonYellow();
	//void Viewallpicture();//at方式遍历整张图片
	vector <int> rgb_b;//整张图片的RGB值
	vector <int> rgb_g;
	vector <int> rgb_r;
	int roww;//图像行数和列数
	int coll;
	afx_msg void OnBnClickedButtonGreen();
	afx_msg void OnBnClickedButtonColorequal();
	
	IplImage *equal_src;//源图进行直方图均衡化
	Mat m_src;//gaosi滤波后的图像
	IplImage *img;
	IplImage *p_img[4];//将源图像克隆了三张，分别用来提取三种颜色
	//IplImage *p_img[1];
	afx_msg void OnBnClickedButtonBlue();

	
	
	afx_msg void OnBnClickedPicturePreDeal();

	void disposeImage(int & hmin, int & hmax, int & smin, int & smax, int & vmin, int & vmax, string & name, Mat & img1);

	int g_srcmin = 126;
	int g_temmin = 128;//画出来的为轮廓1作为模板
	Mat bgr;//灰度值归一化  
	Mat hsv;//HSV图像  
	afx_msg void OnBnClickedButton_DisposeImage();
	void lookKnife(Mat & blue_img, Mat & img_template1);
	//色相  
	int hmin = 0;
	int hmin_Max = 360;
	int hmax = 360;
	int hmax_Max = 360;
	//饱和度  
	int smin = 0;
	int smin_Max = 255;
	int smax = 255;
	int smax_Max = 255;
	//亮度  
	int vmin = 106;
	int vmin_Max = 255;
	int vmax = 250;
	int vmax_Max = 255;
	int bhmin = 170, bhmax = 360, bsmin = 70, bsmax = 255, bvmin = 0, bvmax = 255;
	int ghmin = 58, ghmax = 183, gsmin = 44, gsmax = 255, gvmin = 0, gvmax = 255;
	int yhmin = 0, yhmax = 49, ysmin = 81, ysmax = 255, yvmin = 0, yvmax = 255;
	//分割后的图,分别用来提取有机物，无机物和混合物
	Mat blue_img, blueGray_img, blueThresh_img;
	Mat green_img, greenGray_img, greenThresh_img;
	Mat orange_img, orangeGray_img, orangeThresh_img;
	//模板图
	Mat img_template1, gray_img_template1;//刀的模板
	Mat img_template2, gray_img_template2;//枪的模板
	Mat img_template3, gray_img_template3;//瓶子的模板
	Mat res_img;//输出图像
	string blue = "blue.jpg";
	string green = "green.jpg";
	string yellow = "yellow.jpg";
	afx_msg void lookGun(Mat & blue_img, Mat & img_template2);
	afx_msg void lookBottle(Mat & orange_img, Mat & img_template3);
	afx_msg void OnBnClicked_Result();
	Mat blue_img1, orange_img1;
	Mat img_template_knife;//刀的模板
	Mat img_template_gun;//枪的模板
	Mat img_template_bottle;//瓶子的模板

	//Mat imageContours;
	//Mat Contours;

	CMscomm1 m_mscomm;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	void OpenSerial();
	void SendData();
	int flag = 3;//报警系统串口编号
	int Num_Danger=0;
	int senddata = 1;
	CString data;
	int Close_Alarm = 0;
	CString alarm;
	afx_msg void OnBnClickedButton_closealarm();
};

