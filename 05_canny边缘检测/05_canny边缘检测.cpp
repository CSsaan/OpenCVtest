#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// 原图
	Mat srcImage = imread("../resources/5canny.jpg");

	// 模糊一下
	Mat blurImage;
	blur(srcImage,blurImage,Size(3,3)); // 小卷积核模糊

	// 转灰度图
	Mat greyImage;
	cvtColor(blurImage, greyImage, COLOR_BGR2GRAY);

	// Canny边缘检测
	Mat result;
	// 灰度梯度大于maxVal为边缘；小于minVal不为边缘；位于二者之间看与边缘相连则为边缘。
	Canny(greyImage, result, 3, 9, 3); //阈值 minVal 和 maxVal, kernel_size.
	cvtColor(result, result, COLOR_GRAY2BGR);

	// 拼接
	Mat hresult;
	vector<Mat> hImage;
	hImage.push_back(srcImage);
	hImage.push_back(result);
	hconcat(hImage, hresult);

	// 下采样
	pyrDown(hresult, hresult);

	// 显示
	imshow("Canny灰度图边缘检测", hresult);
	waitKey(0);
	destroyAllWindows();
	return 0;
}