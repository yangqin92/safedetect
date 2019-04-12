
// MFC_safedetectDlg.cpp : ʵ���ļ�
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFC_safedetectDlg �Ի���



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


// CMFC_safedetectDlg ��Ϣ�������

BOOL CMFC_safedetectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	OpenSerial();
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_safedetectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_safedetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_safedetectDlg::OnBnClickedLoadpicture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString fileName;
	TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg)|*.jpg|GIF�ļ�(*.gif)|*.gif|bmp�ļ�(*.bmp)|*.bmp||");
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
	p_img[1] = cvCloneImage(p_img[0]);//ͼƬ1����ת��ΪHSV
	cvCvtColor(p_img[1], p_img[1], CV_BGR2HSV);
	p_img[2] = cvCloneImage(p_img[0]);//ͼƬ1����ת��ΪHSV
	cvCvtColor(p_img[2], p_img[2], CV_BGR2HSV);
	p_img[3] = cvCloneImage(p_img[0]);//ͼƬ1����ת��ΪHSV
	cvCvtColor(p_img[3], p_img[3], CV_BGR2HSV);

	Mat mat = cvarrToMat(p_img[1], true);
	coll = mat.cols;
	roww = mat.rows;

	
	
	
}
IplImage* EqualizeHistColorImage(IplImage *pImage)//��ɫͼ��ֱ��ͼƽ�⻯
{
	IplImage *pEquaImage = cvCreateImage(cvGetSize(pImage), pImage->depth, 3);

	// ԭͼ��ֳɸ�ͨ�����پ��⻯,���ϲ�����ɫͼ���ֱ��ͼ���⻯
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
	CDC* pDC = GetDlgItem(ID)->GetDC();        // �����ʾ�ؼ��� DC
	HDC hDC = pDC->GetSafeHdc();                // ��ȡ HDC(�豸���) �����л�ͼ����

	CRect show_rect;
	GetDlgItem(ID)->GetClientRect(&show_rect);
	// ���ͼƬ�ؼ��Ŀ�͸�
	int show_width;//ͼƬ�Ŀ�͸�
	int show_height;
	show_width = show_rect.right - show_rect.left;
	show_height = show_rect.bottom - show_rect.top;
	SetRect(show_rect, 0, 0, show_width, show_height);

	CvvImage cimg;
	cimg.CopyOf(image);                            // ����ͼƬ
	cimg.DrawToHDC(hDC, &show_rect);                // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	ReleaseDC(pDC);
}


void CMFC_safedetectDlg::OnBnClickedGray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	src_gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor(src, src_gray, CV_BGR2GRAY);
	ShowImage(src_gray, IDC_PICTURE_GRAY);

	//��ʾ�Ҷ�ֱ��ͼ
	
	int hist_size = 256;    //ֱ��ͼ�ߴ�  
	int hist_height = 256;
	float range[] = { 0,255 };  //�Ҷȼ��ķ�Χ  
	float* ranges[] = { range };
	//����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�  
	CvHistogram* gray_hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	//����Ҷ�ͼ���һάֱ��ͼ  
	cvCalcHist(&src_gray, gray_hist, 0, 0);
	//��һ��ֱ��ͼ  
	cvNormalizeHist(gray_hist, 1.0);

	int scale = 2;
	//����һ��һάֱ��ͼ�ġ�ͼ����������Ϊ�Ҷȼ���������Ϊ���ظ�����*scale��  
	IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale, hist_height), 8, 3);
	cvZero(hist_image);
	//ͳ��ֱ��ͼ�е����ֱ����  
	float max_value = 0;
	cvGetMinMaxHistValue(gray_hist, 0, &max_value, 0, 0);

	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i<hist_size; i++)
	{
		float bin_val = cvQueryHistValue_1D(gray_hist, i); //����i�ĸ���  
		int intensity = cvRound(bin_val*hist_height / max_value);  //Ҫ���Ƶĸ߶�  
		cvRectangle(hist_image,
			cvPoint(i*scale, hist_height - 1),
			cvPoint((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}
	
	cvNamedWindow("Դͼ��Ҷ�ֱ��ͼ", 1);
	cvShowImage("Դͼ��Ҷ�ֱ��ͼ", hist_image);

	cvWaitKey(0);



}


void CMFC_safedetectDlg::OnBnClickedEqual()//�Ҷ�ֱ��ͼ���⻯�Լ�ֱ��ͼ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//��ʾ�Ҷ�ֱ��ͼ

	int hist_size = 256;    //ֱ��ͼ�ߴ�  
	int hist_height = 256;
	float range[] = { 0,255 };  //�Ҷȼ��ķ�Χ  
	float* ranges[] = { range };
	//����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�  
	CvHistogram* gray_hist2 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	//����Ҷ�ͼ���һάֱ��ͼ  
	cvCalcHist(&img, gray_hist2, 0, 0);
	//��һ��ֱ��ͼ  
	cvNormalizeHist(gray_hist2, 1.0);

	int scale = 2;
	//����һ��һάֱ��ͼ�ġ�ͼ����������Ϊ�Ҷȼ���������Ϊ���ظ�����*scale��  
	IplImage* hist_image2 = cvCreateImage(cvSize(hist_size*scale, hist_height), 8, 3);
	cvZero(hist_image2);
	//ͳ��ֱ��ͼ�е����ֱ����  
	float max_value = 0;
	cvGetMinMaxHistValue(gray_hist2, 0, &max_value, 0, 0);

	//�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
	for (int i = 0; i<hist_size; i++)
	{
		float bin_val = cvQueryHistValue_1D(gray_hist2, i); //����i�ĸ���  
		int intensity = cvRound(bin_val*hist_height / max_value);  //Ҫ���Ƶĸ߶�  
		cvRectangle(hist_image2,
			cvPoint(i*scale, hist_height - 1),
			cvPoint((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}

	cvNamedWindow("���⻯�Ҷ�ֱ��ͼ", 1);
	cvShowImage("���⻯�Ҷ�ֱ��ͼ", hist_image2);

	cvWaitKey(0);


}


void CMFC_safedetectDlg::OnBnClickedButtonYellow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int y = 0; y < roww; y = y + 1)
	{

		uchar* ptr = (uchar*)(p_img[1]->imageData + y * p_img[1]->widthStep);

		for (int x = 0; x <coll; x = x + 1)
		{
			int H = 3 * x;             //��Ϊÿһ���㶼������ͨ��
			int S = 3 * x + 1;
			int V = 3 * x + 2;
			//@@@@@@Ѱ�ҷ�������������@@@
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int y = 0; y < roww; y = y + 1)
	{

		uchar* ptr = (uchar*)(p_img[2]->imageData + y * p_img[2]->widthStep);

		for (int x = 0; x <coll; x = x + 1)
		{
			int H = 3 * x;             //��Ϊÿһ���㶼������ͨ��
			int S = 3 * x + 1;
			int V = 3 * x + 2;
			//@@@@@@Ѱ�ҷ�������������@@@
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int y = 0; y < roww; y = y + 1)
	{

		uchar* ptr = (uchar*)(p_img[3]->imageData + y * p_img[3]->widthStep);

		for (int x = 0; x <coll; x = x + 1)
		{
			int H = 3 * x;             //��Ϊÿһ���㶼������ͨ��
			int S = 3 * x + 1;
			int V = 3 * x + 2;
			//@@@@@@Ѱ�ҷ�������������@@@
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Mat m;
	m = src;
	bilateralFilter(m, mat_src, 25, 25 * 2, 25 / 2);
	string blue = "blue.jpg";
	string green = "green.jpg";
	string yellow = "yellow.jpg";

	//��ɫͼ��ĻҶ�ֵ��һ��  
	mat_src.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);
	//��ɫ�ռ�ת��  
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
	//���ͼ������ڴ�  
	img1 = Mat::zeros(mat_src.size(), CV_32FC3);//��mat����Ŀ��ͼ�񣬴�СΪimg�ĳߴ磬��ͨ��
											//����  
	Mat mask;
	inRange(hsv, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
	//ֻ����  
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


void CMFC_safedetectDlg::OnBnClickedButton_DisposeImage()//ͼ��ָ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
//����ģ��ƥ��
void CMFC_safedetectDlg::lookKnife(Mat &blue_img, Mat &img_template1) {

	cvtColor(blue_img, blueGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template1, gray_img_template1, COLOR_BGR2GRAY);

	//����ģ��ͼ
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
	
	

	//����ƥ��ͼ
	vector<Mat> contours2(20000);//ƥ���ͼ
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

	Mat drawing = Mat::zeros(mat_src.size(), CV_8UC3);//����һ����ԭͼһ����С��ͼƬ
	for (int i = 0; i < contours2.size(); i++)//����������ͼ�������  
	{

		if (contourArea(contours2[i]) < 500|| contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//���ش�������ģ������֮������ƶ�,a0ԽСԽ����  
			double a0 = matchShapes(contours1[1], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "ģ�������������ͼ������" << i << "�����ƶ�:" << a0 << endl;//�����������������ƶ�  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//���ڴ�����ͼ���ϻ���������  
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
//ǹ��ģ��ƥ��
void CMFC_safedetectDlg::lookGun(Mat &blue_img, Mat &img_template2) {

	cvtColor(blue_img, blueGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template2, gray_img_template2, COLOR_BGR2GRAY);

	//����ģ��ͼ
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
	//����ƥ��ͼ
	threshold(blueGray_img, blueThresh_img, 13, 255, THRESH_BINARY);//��ͼ����ж�ֵ��  															
	vector<Mat> contours2(10000);//ƥ���ͼ
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

	Mat drawing = Mat::zeros(mat_src.size(), CV_8UC3);//����һ����ԭͼһ����С��ͼƬ
	for (int i = 0; i < contours2.size(); i++)//����������ͼ�������  
	{

		if (contourArea(contours2[i]) < 3000 || contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//���ش�������ģ������֮������ƶ�,a0ԽСԽ����  

			double a0 = matchShapes(contours1[1], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "ģ�������������ͼ������" << i << "�����ƶ�:" << a0 << endl;//�����������������ƶ�  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//���ڴ�����ͼ���ϻ���������  
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
//ƿ�ӵ�ģ��ƥ��
void CMFC_safedetectDlg::lookBottle(Mat &orange_img, Mat &img_template3) {
	cvtColor(orange_img, orangeGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template3, gray_img_template3, COLOR_BGR2GRAY);

	//����ģ��ͼ
	threshold(gray_img_template3, gray_img_template3, 114, 255, THRESH_BINARY);
	vector<Mat> contours1(40000);
	vector<Vec4i> hierarchy1(40000);
	//imageContours = Mat::zeros(blueGray_img.size(), CV_8UC1);
	// = Mat::zeros(blueGray_img.size(), CV_8UC1);
	findContours(gray_img_template3, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//˫��������������ÿ��Ԫ�ر�����һ����������Point�㹹�ɵĵ�ļ��ϵ�����������һ����������contours������
	//�ֱ��ʾ��i�������ĺ�һ��������ǰһ������������������Ƕ�������������
	//�������������������������һ���ȼ����ṹ��������������ڲ��������ڲ����������Լ���������Ƕ����
	vector<Mat> contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);//��ͼ����������ж�������
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		//drawContours(imageContours, contours1, i, Scalar(255), 1, 8, hierarchy1);
	}
	//imshow("ContoursImage", imageContours);
	//����ƥ��ͼ
	threshold(orangeGray_img, orangeThresh_img, 30, 255, THRESH_BINARY);//��ͼ����ж�ֵ��  															
	vector<Mat> contours2(40000);//ƥ���ͼ
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


	Mat drawing = Mat::zeros(mat_src.size(), CV_8UC3);//����һ����ԭͼһ����С��ͼƬ
	for (int i = 0; i < contours2.size(); i++)//����������ͼ�������  
	{

		if (contourArea(contours2[i]) < 3000 || contourArea(contours2[i]) > 30000) {
			continue;
		}
		else {//���ش�������ģ������֮������ƶ�,a0ԽСԽ����  
			double a0 = matchShapes(contours1[0], contours2[i], CV_CONTOURS_MATCH_I1, 0);
		//	cout << "ģ�������������ͼ������" << i << "�����ƶ�:" << a0 << endl;//�����������������ƶ�  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//���ڴ�����ͼ���ϻ���������  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	time_t start, ends;
	start = time(NULL);
	blue_img1 = imread("blue.jpg", 1);
	orange_img1 = imread("yellow.jpg", 1);
	img_template_knife = imread("template1.jpg", 1);//����ģ��ͼ
	img_template_gun = imread("template2.jpg", 1);//ǹ��ģ��ͼ
	img_template_bottle = imread("bottle1.jpg", 1);//ƿ�ӵ�ģ��ͼ                                                                           

	bilateralFilter(img_template_knife, img_template1, 25, 25 * 2, 25 / 2);
	bilateralFilter(blue_img1, blue_img, 25, 25 * 2, 25 / 2);
	bilateralFilter(img_template_gun, img_template2, 25, 25 * 2, 25 / 2);
	bilateralFilter(orange_img1, orange_img, 25, 25 * 2, 25 / 2);
	bilateralFilter(img_template_bottle, img_template3, 25, 25 * 2, 25 / 2);
	//˫���˲�������ȥ����˲�Ч������Ϊ�˲����������������ɣ�
	//һ���������ɼ��οռ��������˲���ϵ������һ���������ز�ֵ�����˲���ϵ���� 
	//�ھֲ��ϣ������ڻҶ�ֵ���첻�������ƽ�����ڻҶ�ֵ����Ƚϴ�ı߽���������߽硣
	//����˫���˲���������ÿ�����ص�ͬʱ����Ȼ���ܵ��������ص�ľ��롢�Ҷ�ֵ���Ȩ��Ӱ�졣




	//���ü��ĺ���
	lookKnife(blue_img, img_template1);
	lookGun(blue_img, img_template2);           
	lookBottle(orange_img, img_template3);
	//���������ʾ
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
	// TODO: �ڴ˴������Ϣ����������
}
void CMFC_safedetectDlg::OpenSerial()
{
	if (flag)//�������Ĵ��ڱ��Ϊ����������ִ����������
	{
		if (m_mscomm.get_PortOpen())//���ص�ǰ�Ƿ�򿪴���
		{
			m_mscomm.put_PortOpen(FALSE);
		}
		m_mscomm.put_CommPort(flag); //ѡ��COM
		m_mscomm.put_InBufferSize(1024); //���ջ�����
		m_mscomm.put_OutBufferSize(1024);//���ͻ�����
		m_mscomm.put_InputLen(0);//���õ�ǰ���������ݳ���Ϊ0,��ʾȫ����ȡ
		m_mscomm.put_InputMode(1);//�Զ����Ʒ�ʽ��д����
		m_mscomm.put_RThreshold(1);//���ջ�������1����1�������ַ�ʱ���������������ݵ�OnComm
		m_mscomm.put_Settings(_T("9600,n,8,1"));//������9600�޼���λ��8������λ��1��ֹͣλ

		if (!m_mscomm.get_PortOpen())//�������û�д����
		{
			m_mscomm.put_PortOpen(TRUE);//�򿪴���
			//AfxMessageBox(_T("���ڴ򿪳ɹ�"));
		}
		else
		{
			m_mscomm.put_OutBufferCount(0);
			AfxMessageBox(_T("����ϵͳ���ڴ�ʧ��"));
		}
	}
	else
	{
		AfxMessageBox(_T("����ȷ����ȷ�Ĵ��ڱ��"));
	}
}
void CMFC_safedetectDlg::SendData()
{
	data.Format(_T("%d"), senddata);
	m_mscomm.put_Output(COleVariant(data));//��������
}


void CMFC_safedetectDlg::OnBnClickedButton_closealarm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	alarm.Format(_T("%d"), Close_Alarm);
	m_mscomm.put_Output(COleVariant(alarm));//��������

}
