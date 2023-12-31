#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
//图像拼接函数声明 
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat);
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat);
void callback(int, void*); //滑动条回调函数

int thresholdValue = 100; //阈值数值

Mat srcImage; //原图

//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载原图
	srcImage = imread("../resources/55.jpg");
	if (!srcImage.data) { return -1; }

	//滑动条
	namedWindow("0:二值|1:反二值|2:TRUNC|3:低零|4:高零");
	createTrackbar("threshold", "0:二值|1:反二值|2:TRUNC|3:低零|4:高零", &thresholdValue, 255, callback);

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
	Mat BINARYImage, BINARY_INVImage, TRUNCImage, TOZEROImage, TOZERO_INVImage;

	threshold(srcImage, BINARYImage, thresholdValue, 255, THRESH_BINARY);     //二值
	threshold(srcImage, BINARY_INVImage, thresholdValue, 255, THRESH_BINARY_INV); //反二值
	threshold(srcImage, TRUNCImage, thresholdValue, 255, THRESH_TRUNC);      //TRUNC
	threshold(srcImage, TOZEROImage, thresholdValue, 255, THRESH_TOZERO);     //低零
	threshold(srcImage, TOZERO_INVImage, thresholdValue, 255, THRESH_TOZERO_INV); //高零
	//threshold(srcImage, MASKImage, thresholdValue, 255, 4); //高零
	//threshold(srcImage, OTSUImage, thresholdValue, 255, 4); //高零
	//threshold(srcImage, TRIANGLEImage, thresholdValue, 255, 4); //高零

	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result1 = HVconcat3(srcImage, BINARYImage, BINARY_INVImage, 1);   //第一行
	Mat result2 = HVconcat3(TRUNCImage, TOZEROImage, TOZERO_INVImage, 1); //第二行
	Mat result = HVconcat2(result1, result2, 0);                         //上下拼接3+3
	//下采样缩小
	pyrDown(result, result);
	//显示频谱幅值图
	imshow("0:二值|1:反二值|2:TRUNC|3:低零|4:高零", result);
}

/**
 * @brief 2张图像横向/纵向拼接
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