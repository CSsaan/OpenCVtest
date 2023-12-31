#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
//图像拼接函数声明 
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat);
void callback(int, void*); //滑动条回调函数

Mat srcImage, grayImage, gaussianImage, srcImage2; //原图
int minRadius = 0;
int maxRadius = 80;
int param1 = 100;
int param2 = 100;

//-----------------------------------【main函数】------------------------------------
int main()
{
	//载入原图
	srcImage = imread("../resources/63th.jpg");

	//滑动条
	namedWindow("srcImage|HoughCircles");
	createTrackbar("minRadius", "srcImage|HoughCircles", &minRadius, min(srcImage.cols, srcImage.rows), callback);
	createTrackbar("maxRadius", "srcImage|HoughCircles", &maxRadius, min(srcImage.cols, srcImage.rows), callback);
	createTrackbar("param2", "srcImage|HoughCircles", &param2, 250, callback);
	createTrackbar("param1", "srcImage|HoughCircles", &param1, 250, callback);

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
	//转灰度并图像平滑
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	GaussianBlur(grayImage, gaussianImage, Size(9, 9), 2, 2);

	//霍夫圆变换
	vector<Vec3f> circles; // 用来存储检测到的霍夫圆(圆心，半径)

	//绘制HoughLines所有圆【圆心:(circles[i][0],circles[i][1])，半径:circles[i][0]】
	//cout << minDist << ";" << minRadius << ";" << maxRadius << endl;
	if (param2 < 5) { param2 = 5; }
	HoughCircles(gaussianImage, circles, HOUGH_GRADIENT, 1.5, 10, param1, param2, minRadius, maxRadius);
	Mat srcImage2 = srcImage.clone();
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1])); //圆心
		int redius = cvRound(circles[i][2]);                          //半径
		//绘制圆形
		circle(srcImage2, center, 3, Scalar(0, 255, 0), -1);        //圆心
		circle(srcImage2, center, redius, Scalar(155, 50, 255), 3); //半径
	}
	imshow("srcImage|HoughCircles", srcImage2);

	//图像拼接(1:横向拼接，other:纵向拼接)
	//Mat result = HVconcat2(srcImage, srcImage2, 1);   //第一行
	//下采样缩小
	//pyrDown(result, result);
	//显示频谱幅值图
	//imshow("srcImage|HoughCircles", result);
}

/**
 * @brief 3张图像横向/纵向拼接
 * @param imageN 第N张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}