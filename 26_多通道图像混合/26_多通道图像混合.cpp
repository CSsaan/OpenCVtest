#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdarg.h>

using namespace cv;
using namespace std;

vector<Mat> channels; // 拆分通道
//声明拼接矩阵
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);

int main()
{
	// 原图
	Mat imageDota = imread("../resources/dota.jpg"); //576x1024
	Mat imageLogo = imread("../resources/dota_logo.jpg", 0); //200x200  0:一通道
	if (!imageDota.data) { printf("Oh，no，读取imageDota错误~！ \n"); return false; }
	if (!imageLogo.data) { printf("Oh，no，读取imageLogo错误~！ \n"); return false; }

	//通道拆分
	split(imageDota, channels);
	Mat redChannel = channels[2]; // 获取红色通道 或者.at(2)

	//将原图的红色通道的（500,250）坐标处右下方的一块区域和logo图进行加权操作，将得到的混合结果存到imageRedChannel中
	Mat redRIO = redChannel(Rect(500, 250, imageLogo.cols, imageLogo.rows));
	addWeighted(redRIO, 0.5, imageLogo, 0.3, 0.0, redRIO); // 原图红色通道的RIO区域被加权

	//通道合并
	Mat mergeImageDota;
	merge(channels, mergeImageDota);

	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat(imageDota, mergeImageDota, 1);

	//下采样缩小
	pyrDown(result, result);

	imshow("红色通道加权结果", result);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

/**
 * @brief 两张图像横向/纵向拼接
 * @param image1 第一张待拼接图像
 * @param image2 第二张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat(Mat image1, Mat image2, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result);
	return result;
}