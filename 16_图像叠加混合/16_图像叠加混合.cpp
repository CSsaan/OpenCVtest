#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdarg.h>

using namespace cv;
using namespace std;

Mat srcImageDota = imread("../resources/dota.jpg"); //576x1024

//声明拼接矩阵
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);

int main()
{
	// 加载图片
	Mat imageDota = imread("../resources/dota.jpg"); //576x1024
	Mat imageLogo = imread("../resources/dota_logo.jpg"); //200x200

	//定位原图中的ROI局部区域（并不是新生成）
	Mat imageROI = imageDota(Rect(800, 350, imageLogo.cols, imageLogo.rows)); //图片中局部位置，用来放logo
	circle(imageDota, Point(800, 350), 9, Scalar(0, 255, 0), -1); //logo左上角
	circle(imageDota, Point(800 + imageLogo.cols, 350 + imageLogo.rows), 9, Scalar(0, 255, 0), -1); //logo右下角

	// logo加上去
	addWeighted(imageROI, 0.5, imageLogo, 0.5, 0.0, imageROI);

	//图像拼接(1:横向拼接，other:纵向拼接)
	putText(srcImageDota, "withoutLOGO", Point(srcImageDota.cols / 2, srcImageDota.rows / 8), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
	putText(imageDota, "withLOGO", Point(imageDota.cols / 2, imageDota.rows / 8), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
	Mat result = HVconcat(srcImageDota, imageDota, 1);

	//下采样缩小
	pyrDown(result, result);
	//显示效果图 
	imshow("添加LOGO", result);

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