#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
//图像拼接函数声明 
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat);
void callback(int, void*); //滑动条回调函数

Mat srcImage, cannyImage, grayImage, grayImage2; //原图
int thresholdValue = 160;

//-----------------------------------【main函数】------------------------------------
int main()
{
	//载入原图
	srcImage = imread("../resources/61.jpg");

	//滑动条
	namedWindow("srcImage|HoughLines|HoughLinesP");
	createTrackbar("threshold", "srcImage|HoughLines|HoughLinesP", &thresholdValue, 255, callback);

	waitKey(0);
	destroyAllWindows();
	return 0;
}

//-----------------------------------【自定义函数】------------------------------------
/**
 * @brief 滑动条回调函数
 */
void callback(int, void*)
{
	//边缘检测并转灰度
	Canny(srcImage, cannyImage, 50, 200);
	cvtColor(cannyImage, grayImage, COLOR_GRAY2BGR);

	//霍夫线变换
	vector<Vec2f> lines1; // 用来存储检测到的霍夫线(HoughLines)
	vector<Vec4i> lines2; // 用来存储检测到的霍夫线(HoughLinesP)

	//绘制HoughLines所有线（极坐标系）
	HoughLines(cannyImage, lines1, 1, CV_PI / 180, thresholdValue);
	for (size_t i = 0; i < lines1.size(); i++)
	{
		float rho = lines1[i][0], theta = lines1[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(grayImage, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA);
	}

	//绘制HoughLinesP所有线
	grayImage.copyTo(grayImage2);
	HoughLinesP(cannyImage, lines2, 1, CV_PI / 180, thresholdValue);
	for (size_t i = 0; i < lines2.size(); i++)
	{
		Vec4i l = lines2[i];
		line(grayImage2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(55, 100, 195), 1, LINE_AA);
	}

	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat3(srcImage, grayImage, grayImage2, 1);   //第一行
	//下采样缩小
	pyrDown(result, result);
	//显示频谱幅值图
	imshow("srcImage|HoughLines|HoughLinesP", result);
}

/**
 * @brief 3张图像横向/纵向拼接
 * @param imageN 第N张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	vhImage.push_back(image3);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}