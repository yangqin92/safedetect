#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>


using namespace std;
using namespace cv;

#define WINDOW1 "原图"
#define WINDOW2 "检测结果图"
int g_srcmin = 126;
int g_temmin = 128;//画出来的为轮廓1作为模板
 //灰度值归一化  
Mat bgr;
//HSV图像  
Mat hsv;

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

//原始图
Mat img_src,img, gray_img,thresh_img;
//分割后的图
Mat blue_img1,orange_img1,blue_img, blueGray_img,blueThresh_img;
Mat green_img, greenGray_img,greenThresh_img;
Mat orange_img, orangeGray_img,orangeThresh_img;
//模板图
Mat img_template_knife,img_template1, gray_img_template1;//刀的模板
Mat img_template_gun,img_template2, gray_img_template2;//枪的模板
Mat img_template_bottle,img_template3, gray_img_template3;//瓶子的模板
Mat res_img;//输出图像

//分割函数
void disposeImage(int &hmin, int &hmax, int &smin, int &smax, int &vmin, int &vmax, string &name, Mat &img1);
//检测的函数
void lookKnife(Mat &blue_img,Mat &img_template1);
void lookGun(Mat &green_img, Mat &img_template2);
void lookBottle(Mat &orange_img, Mat &img_template3);
//主函数
int main()
{
	img_src = imread("src.jpg", 1);//传入需要检测的图
	bilateralFilter(img_src, img, 25, 25 * 2, 25 / 2);
    string blue = "blue.jpg";
	string green = "green.jpg";
	string yellow = "yellow.jpg";

	//彩色图像的灰度值归一化  
	img.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);//保持边缘，降噪平滑的效果
	//颜色空间转换  
	cvtColor(bgr, hsv, COLOR_BGR2HSV);
	namedWindow(WINDOW1, CV_WINDOW_AUTOSIZE);
	imshow(WINDOW1, img);

	//调用图像分割的函数
	disposeImage(bhmin, bhmax, bsmin, bsmax, bvmin, bvmax, blue, blue_img);
	disposeImage(ghmin, ghmax, gsmin, gsmax, gvmin, gvmax, green, green_img);
	disposeImage(yhmin, yhmax, ysmin, ysmax, yvmin, yvmax, yellow, orange_img);


	blue_img1 = imread("blue.jpg",1);
	orange_img1 = imread("yellow.jpg", 1);
	img_template_knife = imread("template1.jpg", 1);//刀的模板图
	img_template_gun = imread("template2.jpg", 1);//枪的模板图
	img_template_bottle = imread("bottle.jpg", 1);//瓶子的模板图

	bilateralFilter(img_template_knife, img_template1, 25, 25 * 2, 25 / 2);
	bilateralFilter(blue_img1, blue_img, 25, 25 * 2, 25 / 2);
	bilateralFilter(img_template_gun, img_template2, 25, 25 * 2, 25 / 2);
	bilateralFilter(orange_img1, orange_img, 25, 25 * 2, 25 / 2);
	bilateralFilter(img_template_bottle, img_template3, 25, 25 * 2, 25 / 2);

   


	//调用检测的函数
	 lookKnife(blue_img,img_template1);
	 lookGun(blue_img,img_template2);
	 lookBottle(orange_img,img_template3);
	//检测结果的显示
	namedWindow(WINDOW2, CV_WINDOW_AUTOSIZE);
	imshow(WINDOW2,img);
	

	waitKey(0);
	return 0;

}
//刀的模板匹配
void lookKnife(Mat &blue_img, Mat &img_template1) {
	
	cvtColor(blue_img, blueGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template1, gray_img_template1, COLOR_BGR2GRAY);
	
	//处理模板图
	threshold(gray_img_template1, gray_img_template1, 200, 255, THRESH_BINARY);
	vector<Mat> contours1;
	vector<Vec4i> hierarchy1;
	findContours(gray_img_template1, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Mat > contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}

	//处理匹配图
	vector<Mat> contours2;//匹配的图
	vector<Vec4i> hierarchy2;
	threshold(blueGray_img, blueGray_img, 3, 255, THRESH_BINARY); 															
	findContours(blueGray_img, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Mat > contours_poly2(contours2.size());
	vector<Rect> boundRect2(contours2.size());
	vector<Point2f>center2(contours2.size());
	vector<float>radius2(contours2.size());
	for (int i = 0; i < contours2.size(); i++)
	{
		approxPolyDP(Mat(contours2[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
	}

	Mat drawing = Mat::zeros(img.size(), CV_8UC3);//创建一个和原图一样大小的图片
	for (int i = 0; i < contours2.size(); i++)//遍历待测试图像的轮廓  
	{

		if (contourArea(contours2[i]) < 2000 || contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//返回此轮廓与模版轮廓之间的相似度,a0越小越相似  
			double a0 = matchShapes(contours1[1], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "模版轮廓与待测试图像轮廓" << i << "的相似度:" << a0 << endl;//输出两个轮廓间的相似度  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//则在待测试图像上画出此轮廓  
			if (a0 < 4) {
				rectangle(img, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
	}


}
//枪的模板匹配
void lookGun(Mat &blue_img, Mat &img_template2) {

	cvtColor(blue_img, blueGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template2, gray_img_template2, COLOR_BGR2GRAY);

	//处理模板图
	threshold(gray_img_template2, gray_img_template2, 100, 255, THRESH_BINARY);
	vector<Mat> contours1;
	vector<Vec4i> hierarchy1;
	findContours(gray_img_template2, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}
	//处理匹配图
	threshold(blueGray_img, blueThresh_img, 13, 255, THRESH_BINARY);//对图像进行二值化  															
	vector<vector<Point>> contours2;//匹配的图
	vector<Vec4i> hierarchy2;
	findContours(blueThresh_img, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly2(contours2.size());
	vector<Rect> boundRect2(contours2.size());
	vector<Point2f>center2(contours2.size());
	vector<float>radius2(contours2.size());
	for (int i = 0; i < contours2.size(); i++)
	{
		approxPolyDP(Mat(contours2[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
	}

	Mat drawing = Mat::zeros(img.size(), CV_8UC3);//创建一个和原图一样大小的图片
	for (int i = 0; i < contours2.size(); i++)//遍历待测试图像的轮廓  
	{

		if (contourArea(contours2[i]) < 3000 || contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//返回此轮廓与模版轮廓之间的相似度,a0越小越相似  

			double a0 = matchShapes(contours1[1], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "模版轮廓与待测试图像轮廓" << i << "的相似度:" << a0 << endl;//输出两个轮廓间的相似度  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//则在待测试图像上画出此轮廓  
			if (a0 < 1) {
				rectangle(img, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
	}
}
//瓶子的模板匹配
void lookBottle(Mat &orange_img, Mat &img_template3) {
	cvtColor(orange_img, orangeGray_img, COLOR_BGR2GRAY);
	cvtColor(img_template3, gray_img_template3, COLOR_BGR2GRAY);

	//处理模板图
	threshold(gray_img_template3, gray_img_template3, 100, 255, THRESH_BINARY);
	vector<vector<Point>> contours1;
	vector<Vec4i> hierarchy1;
	findContours(gray_img_template3, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours1.size());
	vector<Rect> boundRect(contours1.size());
	vector<Point2f>center(contours1.size());
	vector<float>radius(contours1.size());
	for (int i = 0; i < contours1.size(); i++)
	{
		approxPolyDP(Mat(contours1[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}
	//处理匹配图
	threshold(orangeGray_img, orangeThresh_img, 3, 255, THRESH_BINARY);//对图像进行二值化  															
	vector<vector<Point>> contours2;//匹配的图
	vector<Vec4i> hierarchy2;
	findContours(orangeThresh_img, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly2(contours2.size());
	vector<Rect> boundRect2(contours2.size());
	vector<Point2f>center2(contours2.size());
	vector<float>radius2(contours2.size());
	for (int i = 0; i < contours2.size(); i++)
	{
		approxPolyDP(Mat(contours2[i]), contours_poly2[i], 3, true);
		boundRect2[i] = boundingRect(Mat(contours_poly2[i]));
		minEnclosingCircle(contours_poly2[i], center2[i], radius2[i]);
	}

	Mat drawing = Mat::zeros(img.size(), CV_8UC3);//创建一个和原图一样大小的图片
	for (int i = 0; i < contours2.size(); i++)//遍历待测试图像的轮廓  
	{

		if (contourArea(contours2[i]) < 3000 || contourArea(contours2[i]) > 10000) {
			continue;
		}
		else {//返回此轮廓与模版轮廓之间的相似度,a0越小越相似  
			double a0 = matchShapes(contours1[0], contours2[i], CV_CONTOURS_MATCH_I1, 0);
			cout << "模版轮廓与待测试图像轮廓" << i << "的相似度:" << a0 << endl;//输出两个轮廓间的相似度  
																 //drawContours(drawing, contours_poly2, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());//则在待测试图像上画出此轮廓  
			if (a0 < 1) {
				rectangle(img, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
	}
}

void disposeImage(int &hmin, int &hmax, int &smin, int &smax, int &vmin, int &vmax, string &name, Mat &img1)
{
	//输出图像分配内存  
	img1 = Mat::zeros(img.size(), CV_32FC3);//用mat生成目标图像，大小为img的尺寸，三通道
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