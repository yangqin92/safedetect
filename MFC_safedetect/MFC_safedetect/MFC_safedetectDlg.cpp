
// MFC_safedetectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_safedetect.h"
#include "MFC_safedetectDlg.h"
#include "afxdialogex.h"
#include <string> 
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "cvaux.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <time.h>

using namespace std;
using namespace cv;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFC_safedetectDlg 对话框



CMFC_safedetectDlg::CMFC_safedetectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_SAFEDETECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_safedetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
}

BEGIN_MESSAGE_MAP(CMFC_safedetectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOADPICTURE, &CMFC_safedetectDlg::OnBnClickedLoadpicture)
	ON_BN_CLICKED(IDC_Gray, &CMFC_safedetectDlg::OnBnClickedGray)
	ON_BN_CLICKED(IDC_EQUAL, &CMFC_safedetectDlg::OnBnClickedEqual)
	ON_BN_CLICKED(IDC_BUTTON_YELLOW, &CMFC_safedetectDlg::OnBnClickedButtonYellow)
	ON_BN_CLICKED(IDC_BUTTON_GREEN, &CMFC_safedetectDlg::OnBnClickedButtonGreen)
	ON_BN_CLICKED(IDC_BUTTON_COLOREQUAL, &CMFC_safedetectDlg::OnBnClickedButtonColorequal)
	ON_BN_CLICKED(IDC_BUTTON_BLUE, &CMFC_safedetectDlg::OnBnClickedButtonBlue)
	ON_BN_CLICKED(IDC_LOADPICTURE2, &CMFC_safedetectDlg::OnBnClickedPicturePreDeal)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC_safedetectDlg::OnBnClickedButton_DisposeImage)
	ON_BN_CLICKED(IDC_LOADPICTURE3, &CMFC_safedetectDlg::OnBnClicked_Result)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC_safedetectDlg::OnBnClickedButton_closealarm)
END_MESSAGE_MAP()


// CMFC_safedetectDlg 消息处理程序

BOOL CMFC_safedetectDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OpenSerial();
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_safedetectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_safedetectDlg::OnPaint()
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
HCURSOR CMFC_safedetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_safedetectDlg::OnBnClickedLoadpicture()
{
	// TODO: 在此添加控件通知处理程序代码
	CString fileName;
	TCHAR szFilter[] = _T("JPEG文件(*.jpg)|*.jpg|GIF文件(*.gif)|*.gif|bmp文件(*.bmp)|*.bmp||");
	CFileDialog fileDlg(TRUE, _T("jpg"), NULL, 0, szFilter, this);

	if (fileDlg.DoModal() != IDOK)
		return;

	fileName = fileDlg.GetPathName();
	std::string tempName = (LPCSTR)CStringA(fileName);
	const char *tmp = tempName.c_str();

	if ((src = cvLoadImage(tmp, 1)) == 0)
		return;

	ShowImage(src, IDC_PICTURE);
	p_img[0] = src;
	p_img[1] = cvCloneImage(p_img[0]);//图片1将被转换为HSV
	cvCvtColor(p_img[1], p_img[1], CV_BGR2HSV);
	p_img[2] = cvCloneImage(p_img[0]);//图片1将被转换为HSV
	cvCvtColor(p_img[2], p_img[2], CV_BGR2HSV);
	p_img[3] = cvCloneImage(p_img[0]);//图片1将被转换为HSV
	cvCvtColor(p_img[3], p_img[3], CV_BGR2HSV);

	Mat mat = cvarrToMat(p_img[1], true);
	coll = mat.cols;
	roww = mat.rows;

	
	
	
}
IplImage* EqualizeHistColorImage(IplImage *pImage)//彩色图像直方图平衡化
{
	IplImage *pEquaImage = cvCreateImage(cvGetSize(pImage), pImage->depth, 3);

	// 原图像分成各通道后再均衡化,最后合并即彩色图像的直方图均衡化
	const int MAX_CHANNEL = 4;
	IplImage *pImageChannel[MAX_CHANNEL] = { NULL };

	int i;
	for (i = 0; i < pImage->nChannels; i++)
		pImageChannel[i] = cvCreateImage(cvGetSize(pImage), pImage->depth, 1);

	cvSplit(pImage, pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3]);

	for (i = 0; i < pImage->nChannels; i++)
		cvEqualizeHist(pImageChannel[i], pImageChannel[i]);

	cvMerge(pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3], pEquaImage);

	for (i = 0; i < pImage->nChannels; i++)
		cvReleaseImage(&pImageChannel[i]);

	return pEquaImage;
}
void  CMFC_safedetectDlg::ShowImage(IplImage *image, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();        // 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作

	CRect show_rect;
	GetDlgItem(ID)->GetClientRect(&show_rect);
	// 求出图片控件的宽和高
	int show_width;//图片的宽和高
	int show_height;
	show_width = show_rect.right - show_rect.left;
	show_height = show_rect.bottom - show_rect.top;
	SetRect(show_rect, 0, 0, show_width, show_height);

	CvvImage cimg;
	cimg.CopyOf(image);                            // 复制图片
	cimg.DrawToHDC(hDC, &show_rect);                // 将图片绘制到显示控件的指定区域内

	ReleaseDC(pDC);
}


void CMFC_safedetectDlg::OnBnClickedGray()
{
	// TODO: 在此添加控件通知处理程序代码
	src_gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor(src, src_gray, CV_BGR2GRAY);
	ShowImage(src_gray, IDC_PICTURE_GRAY);

	//显示灰度直方图
	
	int hist_size = 256;    //直方图尺寸  
	int hist_height = 256;
	float range[] = { 0,255 };  //灰度级的范围  
	float* ranges[] = { range };
	//创建一维直方图，统计图像在[0 255]像素的均匀分布  
	CvHistogram* gray_hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	//计算灰度图像的一维直方图  
	cvCalcHist(&src_gray, gray_hist, 0, 0);
	//归一化直方图  
	cvNormalizeHist(gray_hist, 1.0);

	int scale = 2;
	//创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）  
	IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale, hist_height), 8, 3);
	cvZero(hist_image);
	//统计直方图中的最大直方块  
	float max_value = 0;
	cvGetMinMaxHistValue(gray_hist, 0, &max_value, 0, 0);

	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i<hist_size; i++)
	{
		float bin_val = cvQueryHistValue_1D(gray_hist, i); //像素i的概率  
		int intensity = cvRound(bin_val*hist_height / max_value);  //要绘制的高度  
		cvRectangle(hist_image,
			cvPoint(i*scale, hist_height - 1),
			cvPoint((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}
	
	cvNamedWindow("源图像灰度直方图", 1);
	cvShowImage("源图像灰度直方图", hist_image);

	cvWaitKey(0);



}


void CMFC_safedetectDlg::OnBnClickedEqual()//灰度直方图均衡化以及直方图绘制
{
	// TODO: 在此添加控件通知处理程序代码
	//mat_src = cv::Mat(src);
	mat_src_gray = cv::Mat(src_gray);
	cv::Mat src_equal;
	equalizeHist(mat_src_gray, src_equal);
	//IplImage* img;
	//img = &IplImage(src_equal);
	//IplImage *img = cvCloneImage(src_equal);
	
	IplImage input = src_equal;
	IplImage *img = cvCloneImage(&input);
	ShowImage(img, IDC_PICTURE_EQUAL);
	//显示灰度直方图

	int hist_size = 256;    //直方图尺寸  
	int hist_height = 256;
	float range[] = { 0,255 };  //灰度级的范围  
	float* ranges[] = { range };
	//创建一维直方图，统计图像在[0 255]像素的均匀分布  
	CvHistogram* gray_hist2 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	//计算灰度图像的一维直方图  
	cvCalcHist(&img, gray_hist2, 0, 0);
	//归一化直方图  
	cvNormalizeHist(gray_hist2, 1.0);

	int scale = 2;
	//创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）  
	IplImage* hist_image2 = cvCreateImage(cvSize(hist_size*scale, hist_height), 8, 3);
	cvZero(hist_image2);
	//统计直方图中的最大直方块  
	float max_value = 0;
	cvGetMinMaxHistValue(gray_hist2, 0, &max_value, 0, 0);

	//分别将每个直方块的值绘制到图中  
	for (int i = 0; i<hist_size; i++)
	{
		float bin_val = cvQueryHistValue_1D(gray_hist2, i); //像素i的概率  
		int intensity = cvRound(bin_val*hist_height / max_value);  //要绘制的高度  
		cvRectangle(hist_image2,
			cvPoint(i*scale, hist_height - 1),
			cvPoint((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}

	cvNamedWindow("均衡化灰度直方图", 1);
	cvShowImage("均衡化灰度直方图", hist_image2);

	cvWaitKey(0);


}


void CMFC_safedetectDlg::OnBnClickedButtonYellow()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int y = 0; y < roww; y = y + 1)
	{

		uchar* ptr = (uchar*)(p_img[1]->imageData + y * p_img[1]->widthStep);

		for (int x = 0; x <coll; x = x + 1)
		{
			int H = 3 * x;             //因为每一个点都有三个通道
			int S = 3 * x + 1;
			int V = 3 * x + 2;
			//@@@@@@寻找符合条件的像素@@@
			if (ptr[H] >= 2 && ptr[H] <= 43 && ptr[S] >= 43 && ptr[S] <= 255 && ptr[V] >= 46 && ptr[V] <= 255)
			{
				ptr[H] = 255;
				ptr[S] = 255;
				ptr[V] = 255;
			}
			else
			{
				ptr[H] = 0;
				ptr[S] = 0;
				ptr[V] = 0;
				
			}
			
		}


	}
	ShowImage(p_img[1], IDC_PICTURE_YELLOW);

	Mat pimg1(p_img[1]);
	imwrite("yellow.jpg",pimg1 );
	
	
}



void CMFC_safedetectDlg::OnBnClickedButtonGreen()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int y = 0; y < roww; y = y + 1)
	{

		uchar* ptr = (uchar*)(p_img[2]->imageData + y * p_img[2]->widthStep);

		for (int x = 0; x <coll; x = x + 1)
		{
			int H = 3 * x;             //因为每一个点都有三个通道
			int S = 3 * x + 1;
			int V = 3 * x + 2;
			//@@@@@@寻找符合条件的像素@@@
			if (ptr[H] >= 35 && ptr[H] <= 77 && ptr[S] >= 43 && ptr[S] <= 255 && ptr[V] >= 46 && ptr[V] <= 255)
			{
				ptr[H] = 0;
				ptr[S] = 0;
				ptr[V] = 0;
				
			}
			else
			{
				ptr[H] = 255;
				ptr[S] = 255;
				ptr[V] = 255;

			}

		}


	}

	ShowImage(p_img[2], IDC_PICTURE_GREEN);
	Mat pimg2(p_img[2]);
	imwrite("green.jpg", pimg2);
	

}




void CMFC_safedetectDlg::OnBnClickedButtonColorequal()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* img_8uc1 = src;
	IplImage* img_edge1 = cvCreateImage(cvGetSize(img_8uc1), 8, 1);
	IplImage* img_8uc3 = cvCreateImage(cvGetSize(img_8uc1), 8, 3);
	Mat Img;
	//Mat dst1;
	Img = cvarrToMat(img_8uc1);
	cvtColor(Img,Img , CV_BGR2GRAY);
	IplImage* dst1 = &IplImage(Img);
	cvThreshold(dst1, img_edge1, 128, 255, CV_THRESH_BINARY);


	CvMemStorage* storage1 = cvCreateMemStorage();
	CvSeq* first_contour1 = NULL;

	int Nc = cvFindContours(
		img_edge1,
		storage1,
		&first_contour1,
		sizeof(CvContour),
		CV_RETR_LIST
		);

	CvContourTree* tree1 = cvCreateContourTree(
		first_contour1,
		storage1,
		200
		);

	IplImage* img_8uc12 = cvLoadImage("gun.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img_edge12 = cvCreateImage(cvGetSize(img_8uc12), 8, 1);
	IplImage* img_8uc13 = cvCreateImage(cvGetSize(img_8uc12), 8, 3);


	cvThreshold(img_8uc12, img_edge12, 128, 255, CV_THRESH_BINARY);


	CvMemStorage* storage2 = cvCreateMemStorage();
	CvSeq* first_contour2 = NULL;

	int Nc2 = cvFindContours(
		img_edge12,
		storage2,
		&first_contour2,
		sizeof(CvContour),
		CV_RETR_LIST
		);
	CvContourTree* tree2 = cvCreateContourTree(
		first_contour2,
		storage2,
		200
		);

	double n = cvMatchContourTrees(tree1, tree2, CV_CONTOURS_MATCH_I1, 100);

}


void CMFC_safedetectDlg::OnBnClickedButtonBlue()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int y = 0; y < roww; y = y + 1)
	{

		uchar* ptr = (uchar*)(p_img[3]->imageData + y * p_img[3]->widthStep);

		for (int x = 0; x <coll; x = x + 1)
		{
			int H = 3 * x;             //因为每一个点都有三个通道
			int S = 3 * x + 1;
			int V = 3 * x + 2;
			//@@@@@@寻找符合条件的像素@@@
			if (ptr[H] >= 100 && ptr[H] <= 124 && ptr[S] >= 43 && ptr[S] <= 255 && ptr[V] >= 46 && ptr[V] <= 255)
			{
				ptr[H] = 0;
				ptr[S] = 0;
				ptr[V] = 0;

				
			}
			else
			{
				ptr[H] = 255;
				ptr[S] = 255;
				ptr[V] = 255;

			}

		}


	}
	
	ShowImage(p_img[3], IDC_PICTURE_BLUE);
	Mat pimg3(p_img[3]);
	imwrite("blue.jpg", pimg3);
	


}


void CMFC_safedetectDlg::OnBnClickedPicturePreDeal()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat m;
	m = src;
	bilateralFilter(m, mat_src, 25, 25 * 2, 25 / 2);
	string blue = "blue.jpg";
	string green = "green.jpg";
	string yellow = "yellow.jpg";

	//彩色图像的灰度值归一化  
	mat_src.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);
	//颜色空间转换  
	cvtColor(bgr, hsv, COLOR_BGR2HSV);
	//imwrite("pre", mat_src);
	IplImage *Src = &IplImage(mat_src);
	ShowImage(Src, IDC_PICTURE_PREDEAL);
	Mat y;
	y = Src;
	imwrite("pre.jpg", y);


}
void CMFC_safedetectDlg::disposeImage(int &hmin, int &hmax, int &smin, int &smax, int &vmin, int &vmax, string &name, Mat &img1)
{
	//输出图像分配内存  
	img1 = Mat::zeros(mat_src.size(), CV_32FC3);//用mat生成目标图像，大小为img的尺寸，三通道
											//掩码  
	Mat mask;
	inRange(hsv, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
	//只保留  
	for (int r = 0; r < bgr.rows; r++)
	{
		for (int c = 0; c < bgr.cols; c++)
		{
			if (mask.at<uchar>(r, c) == 255)
			{
				img1.at<Vec3f>(r, c) = bgr.at<Vec3f>(r, c);
			}
		}
	}

	img1.convertTo(img1, CV_8UC3, 255.0, 0);
	imwrite(name, img1);

}


void CMFC_safedetectDlg::OnBnClickedButton_DisposeImage()//图像分割
{
	// TODO: 在此添加控件通知处理程序代码
	disposeImage(bhmin, bhmax, bsmin, bsmax, bvmin, bvmax, blue, blue_img);
	IplImage *bimg = &IplImage(blue_img);
	ShowImage(bimg, IDC_PICTURE_BLUE);
	//blue_img.convertTo(blue_img, CV_8UC3, 255.0, 0);
	imwrite("blue.jpg", blue_img);

	disposeImage(ghmin, ghmax, gsmin, gsmax, gvmin, gvmax, green, green_img);
	IplImage *gimg = &IplImage(green_img);
	ShowImage(gimg, IDC_PICTURE_GREEN);
	//green_img.convertTo(green_img, CV_8UC3, 255.0, 0);
	imwrite("green.jpg", green_img);

	disposeImage(yhmin, yhmax, ysmin, ysmax, yvmin, yvmax, yellow, orange_img);
	IplImage *yimg = &IplImage(orange_img);
	ShowImage(yimg, IDC_PICTURE_YELLOW);
	//orange_img.convertTo(orange_img, CV_8UC3, 255.0, 0);
	imwrite("yellow.jpg", orange_img);

}
//刀的模板匹配
void CMFC_safedetectDlg::lookKnife(Mat &blue_img, Mat &img_template1) {

	cvtColor(blue_img, blueGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template1, gray_img_template1, COLOR_BGR2GRAY);

	//处理模板图
	threshold(gray_img_template1, gray_img_template1, 132, 255, THRESH_BINARY);
	vector<Mat> contours1(10000);
	vector<Vec4i> hierarchy1(10000);
	
	//imageContours = Mat::zeros(gray_img_template1.size(), CV_8UC1);
	//Contours = Mat::zeros(gray_img_template1.size(), CV_8UC1);

	findContours(gray_img_template1, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
	
	
	vector<Mat> contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
   
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		//drawContours(imageContours, contours1, i, Scalar(255), 1, 8, hierarchy1);
	}
	//imshow("ContoursImage", imageContours);
	//imwrite("knife", imageContours);
	
	

	//处理匹配图
	vector<Mat> contours2(20000);//匹配的图
	vector<Vec4i> hierarchy2(20000);
	threshold(blueGray_img, blueGray_img, 13, 255, THRESH_BINARY);
	//imshow("ContoursImage", blueGray_img);
	//imageContours = Mat::zeros(blueGray_img.size(), CV_8UC1);
	//Contours = Mat::zeros(blueGray_img.size(), CV_8UC1);

	findContours(blueGray_img, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Mat> contours_poly2(contours2.size());
	vector<Rect> boundRect2(contours2.size());
	vector<Point2f>center2(contours2.size());
	vector<float>radius2(contours2.size());
	for (int i = 0; i < contours2.size(); i++)
	{
		approxPolyDP(Mat(contours2[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
		//drawContours(imageContours, contours2, i, Scalar(255), 1, 8, hierarchy2);
	}
	//imshow("ContoursImage", imageContours);

	Mat drawing = Mat::zeros(mat_src.size(), CV_8UC3);//创建一个和原图一样大小的图片
	for (int i = 0; i < contours2.size(); i++)//遍历待测试图像的轮廓  
	{

		if (contourArea(contours2[i]) < 500|| contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//返回此轮廓与模版轮廓之间的相似度,a0越小越相似  
			double a0 = matchShapes(contours1[1], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "模版轮廓与待测试图像轮廓" << i << "的相似度:" << a0 << endl;//输出两个轮廓间的相似度  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//则在待测试图像上画出此轮廓  
			if (a0 < 8) {
				rectangle(mat_src, boundRect2[i].tl(), boundRect2[i].br(), Scalar(255, 0, 0), 2, 8, 0);
				std::string text = "Knife";
				int font_face = cv::FONT_HERSHEY_COMPLEX;
				double font_scale = 1;
				int thickness= 2;
				cv::putText(mat_src, text, boundRect2[i].tl(), font_face, font_scale, cv::Scalar(255, 0, 0), thickness, 8, 0);
				Num_Danger++;
			}
		}
	}


}
//枪的模板匹配
void CMFC_safedetectDlg::lookGun(Mat &blue_img, Mat &img_template2) {

	cvtColor(blue_img, blueGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template2, gray_img_template2, COLOR_BGR2GRAY);

	//处理模板图
	threshold(gray_img_template2, gray_img_template2, 100, 255, THRESH_BINARY);
	vector<Mat> contours1(15000);
	vector<Vec4i> hierarchy1(15000);

	//imageContours = Mat::zeros(blueGray_img.size(), CV_8UC1);
	//Contours = Mat::zeros(blueGray_img.size(), CV_8UC1);

	findContours(gray_img_template2, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Mat> contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		//drawContours(imageContours, contours1, i, Scalar(255), 1, 8, hierarchy1);
	}
	//imshow("ContoursImage", imageContours);
	//处理匹配图
	threshold(blueGray_img, blueThresh_img, 13, 255, THRESH_BINARY);//对图像进行二值化  															
	vector<Mat> contours2(10000);//匹配的图
	vector<Vec4i> hierarchy2(10000);
	findContours(blueThresh_img, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Mat> contours_poly2(contours2.size());
	vector<Rect> boundRect2(contours2.size());
	vector<Point2f>center2(contours2.size());
	vector<float>radius2(contours2.size());
	for (int i = 0; i < contours2.size(); i++)
	{
		approxPolyDP(Mat(contours2[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
		//drawContours(imageContours, contours2, i, Scalar(255), 1, 8, hierarchy2);
	}

	Mat drawing = Mat::zeros(mat_src.size(), CV_8UC3);//创建一个和原图一样大小的图片
	for (int i = 0; i < contours2.size(); i++)//遍历待测试图像的轮廓  
	{

		if (contourArea(contours2[i]) < 3000 || contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//返回此轮廓与模版轮廓之间的相似度,a0越小越相似  

			double a0 = matchShapes(contours1[1], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "模版轮廓与待测试图像轮廓" << i << "的相似度:" << a0 << endl;//输出两个轮廓间的相似度  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//则在待测试图像上画出此轮廓  
			if (a0 < 2) {
				rectangle(mat_src, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 255, 0), 2, 8, 0);
				std::string text = "Gun";
				int font_face = cv::FONT_HERSHEY_COMPLEX;
				double font_scale = 1;
				int thickness = 2;
				cv::putText(mat_src, text, boundRect2[i].tl(), font_face, font_scale, cv::Scalar(0, 255, 0), thickness, 8, 0);
				Num_Danger++;
			}
		}
	}
}
//瓶子的模板匹配
void CMFC_safedetectDlg::lookBottle(Mat &orange_img, Mat &img_template3) {
	cvtColor(orange_img, orangeGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template3, gray_img_template3, COLOR_BGR2GRAY);

	//处理模板图
	threshold(gray_img_template3, gray_img_template3, 114, 255, THRESH_BINARY);
	vector<Mat> contours1(40000);
	vector<Vec4i> hierarchy1(40000);
	//imageContours = Mat::zeros(blueGray_img.size(), CV_8UC1);
	// = Mat::zeros(blueGray_img.size(), CV_8UC1);
	findContours(gray_img_template3, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//双重向量，向量内每个元素保存了一组由连续的Point点构成的点的集合的向量，这是一组轮廓，有contours组轮廓
	//分别表示第i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号
	//检测所有轮廓，所有轮廓建立一个等级树结构。外层轮廓包含内层轮廓，内层轮廓还可以继续包含内嵌轮廓
	vector<Mat> contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);//对图像轮廓点进行多边形拟合
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//计算轮廓的垂直边界最小矩形，矩形是与图像上下边界平行的
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		//drawContours(imageContours, contours1, i, Scalar(255), 1, 8, hierarchy1);
	}
	//imshow("ContoursImage", imageContours);
	//处理匹配图
	threshold(orangeGray_img, orangeThresh_img, 30, 255, THRESH_BINARY);//对图像进行二值化  															
	vector<Mat> contours2(40000);//匹配的图
	vector<Vec4i> hierarchy2(40000);

	//imageContours = Mat::zeros(blueGray_img.size(), CV_8UC1);
	//Contours = Mat::zeros(blueGray_img.size(), CV_8UC1);
	findContours(orangeThresh_img, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	vector<Mat> contours_poly2(contours2.size());
	vector<Rect> boundRect2(contours2.size());
	vector<Point2f>center2(contours2.size());
	vector<float>radius2(contours2.size());
	for (int i = 0; i < contours2.size(); i++)
	{
		approxPolyDP(Mat(contours2[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
		//drawContours(imageContours, contours2, i, Scalar(255), 1, 8, hierarchy2);
	}
	//imshow("ContoursImage", imageContours);


	Mat drawing = Mat::zeros(mat_src.size(), CV_8UC3);//创建一个和原图一样大小的图片
	for (int i = 0; i < contours2.size(); i++)//遍历待测试图像的轮廓  
	{

		if (contourArea(contours2[i]) < 3000 || contourArea(contours2[i]) > 30000) {
			continue;
		}
		else {//返回此轮廓与模版轮廓之间的相似度,a0越小越相似  
			double a0 = matchShapes(contours1[0], contours2[i], CV_CONTOURS_MATCH_I1, 0);
		//	cout << "模版轮廓与待测试图像轮廓" << i << "的相似度:" << a0 << endl;//输出两个轮廓间的相似度  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//则在待测试图像上画出此轮廓  
			if (a0 <1) {
				rectangle(mat_src, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 0, 255), 2, 8, 0);
				std::string text = "Bottle";
				int font_face = cv::FONT_HERSHEY_COMPLEX;
				double font_scale = 1;
				int thickness = 2;
				cv::putText(mat_src, text, boundRect2[i].tl(), font_face, font_scale, cv::Scalar(0, 0, 255), thickness, 8, 0);
				Num_Danger++;
			}
		}
	}
}


void CMFC_safedetectDlg::OnBnClicked_Result()
{
	// TODO: 在此添加控件通知处理程序代码
	time_t start, ends;
	start = time(NULL);
	blue_img1 = imread("blue.jpg", 1);
	orange_img1 = imread("yellow.jpg", 1);
	img_template_knife = imread("template1.jpg", 1);//刀的模板图
	img_template_gun = imread("template2.jpg", 1);//枪的模板图
	img_template_bottle = imread("bottle1.jpg", 1);//瓶子的模板图                                                                           

	bilateralFilter(img_template_knife, img_template1, 25, 25 * 2, 25 / 2);
	bilateralFilter(blue_img1, blue_img, 25, 25 * 2, 25 / 2);
	bilateralFilter(img_template_gun, img_template2, 25, 25 * 2, 25 / 2);
	bilateralFilter(orange_img1, orange_img, 25, 25 * 2, 25 / 2);
	bilateralFilter(img_template_bottle, img_template3, 25, 25 * 2, 25 / 2);
	//双边滤波，保边去噪的滤波效果是因为滤波器由两个函数构成：
	//一个函数是由几何空间距离决定滤波器系数，另一个是由像素差值决定滤波器系数。 
	//在局部上，就是在灰度值差异不大的区域平滑，在灰度值差异比较大的边界地区保留边界。
	//所以双边滤波器作用于每个像素的同时，必然会受到领域像素点的距离、灰度值差的权重影响。




	//调用检测的函数
	lookKnife(blue_img, img_template1);
	lookGun(blue_img, img_template2);           
	lookBottle(orange_img, img_template3);
	//检测结果的显示
	ends= time(NULL);
	double t =difftime(ends,start);
	if(Num_Danger>=1)
	{
		SendData();
	}
	Num_Danger = 0;
	IplImage *result = &IplImage(mat_src);
	ShowImage(result, IDC_PICTURE_RESULT);
	//mat_src.convertTo(mat_src, CV_8UC3, 255.0, 0);
	imwrite("result.jpg", mat_src);

	
}
BEGIN_EVENTSINK_MAP(CMFC_safedetectDlg, CDialogEx)
	ON_EVENT(CMFC_safedetectDlg, IDC_MSCOMM1, 1, CMFC_safedetectDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CMFC_safedetectDlg::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
}
void CMFC_safedetectDlg::OpenSerial()
{
	if (flag)//如果输入的串口编号为正整数，才执行下列命令
	{
		if (m_mscomm.get_PortOpen())//返回当前是否打开串口
		{
			m_mscomm.put_PortOpen(FALSE);
		}
		m_mscomm.put_CommPort(flag); //选择COM
		m_mscomm.put_InBufferSize(1024); //接收缓冲区
		m_mscomm.put_OutBufferSize(1024);//发送缓冲区
		m_mscomm.put_InputLen(0);//设置当前接收区数据长度为0,表示全部读取
		m_mscomm.put_InputMode(1);//以二进制方式读写数据
		m_mscomm.put_RThreshold(1);//接收缓冲区有1个及1个以上字符时，将引发接收数据的OnComm
		m_mscomm.put_Settings(_T("9600,n,8,1"));//波特率9600无检验位，8个数据位，1个停止位

		if (!m_mscomm.get_PortOpen())//如果串口没有打开则打开
		{
			m_mscomm.put_PortOpen(TRUE);//打开串口
			//AfxMessageBox(_T("串口打开成功"));
		}
		else
		{
			m_mscomm.put_OutBufferCount(0);
			AfxMessageBox(_T("报警系统串口打开失败"));
		}
	}
	else
	{
		AfxMessageBox(_T("请先确定正确的串口编号"));
	}
}
void CMFC_safedetectDlg::SendData()
{
	data.Format(_T("%d"), senddata);
	m_mscomm.put_Output(COleVariant(data));//发送数据
}


void CMFC_safedetectDlg::OnBnClickedButton_closealarm()
{
	// TODO: 在此添加控件通知处理程序代码
	alarm.Format(_T("%d"), Close_Alarm);
	m_mscomm.put_Output(COleVariant(alarm));//发送数据

}
