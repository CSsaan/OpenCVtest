#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat); //图像拼接
void callback(int, void*); //滑动条回调函数

Mat srcImage;
int mode = 0; //轮廓的检索模式(0:最外围轮廓, 1:所有的轮廓, 2:所有的轮廓, 3:所有轮廓 )
int method = 0; //轮廓的近似方法(0:所有连续的轮廓点, 1:轮廓的拐点, 2、3:... )
//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载图片
	srcImage = imread("../resources/dota_logo.jpg", 0); //以灰度图打开
	if (!srcImage.data) { return -1; }

	//滑动条
	namedWindow("srcImage|dstImage");
	createTrackbar("mode", "srcImage|dstImage", &mode, 5, callback);
	createTrackbar("method", "srcImage|dstImage", &method, 4, callback);

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
	Mat dstImage = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);
	Mat Contours = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);
	//轮廓和层次结构
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	srcImage = srcImage > 128;

	//查找轮廓
	findContours(srcImage, contours, hierarchy, mode, method);

	//画轮廓
	for (int i = 0; i < contours.size(); i++)
	{
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数  
		for (int j = 0; j < contours[i].size(); j++)
		{
			//绘制出contours向量内所有的像素点  
			Point P = Point(contours[i][j].x, contours[i][j].y);
			Contours.at<uchar>(P) = 255;
		}
		drawContours(dstImage, contours, i, Scalar(255), 1, 8, hierarchy);
	}

	imshow("srcImage|dstImage", dstImage);//轮廓
	imshow("srcImage|Contours", Contours);//点集

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
	try
	{
		1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}

	return result;
}
