#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat);
void callback(int, void*); //滑动条回调函数

int Method = 0; //二值化阈值
Mat srcImage, templateImage;

//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载图片
	srcImage = imread("../resources/84_1.jpg");
	templateImage = imread("../resources/84_2.jpg");
	if (!srcImage.data || !templateImage.data) { cout << "图片加载失败！" << endl; return -1; }
	//滑动条
	namedWindow("模板匹配");
	createTrackbar("Method", "模板匹配", &Method, 5, callback);
	callback(0, 0);
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
	Mat newsrcImage = srcImage.clone();
	Mat resultImage;
	int resultImage_rows = srcImage.rows - templateImage.rows + 1;
	int resultImage_cols = srcImage.cols - templateImage.cols + 1;
	resultImage.create(resultImage_rows, resultImage_cols, CV_32FC1);
	//匹配&归一化
	matchTemplate(srcImage, templateImage, resultImage, Method);
	normalize(resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat());

	//minMaxLoc定位最匹配的位置
	double minVal, maxVal;
	Point minLocation, maxLocation;
	minMaxLoc(resultImage, &minVal, &maxVal, &minLocation, &maxLocation, Mat());
	//匹配位置
	Point matchLocation;
	if (Method == TM_SQDIFF || Method == TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	//绘制矩形显示
	rectangle(newsrcImage, matchLocation,
		Point(matchLocation.x + templateImage.cols, matchLocation.y + templateImage.rows),
		Scalar(0, 0, 255), 2, 8, 0);
	rectangle(resultImage, matchLocation,
		Point(matchLocation.x + templateImage.cols, matchLocation.y + templateImage.rows),
		Scalar(0, 0, 255), 2, 8, 0);

	//图像拼接(1:横向拼接，other:纵向拼接)
	//Mat result = HVconcat2(srcImage, resultImage, 1);

	//下采样缩小
	pyrDown(srcImage, srcImage);
	//pyrDown(resultImage, resultImage);

	imshow("模板匹配", srcImage);
	//imshow("模板匹配", resultImage);

	//显示频谱幅值图
	//imshow("模板匹配", result);
}

/**
 * @brief n张图像横向/纵向拼接
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