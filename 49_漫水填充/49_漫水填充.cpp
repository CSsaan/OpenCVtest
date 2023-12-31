#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//图像拼接函数声明
Mat HVconcat(Mat image1, Mat image2, int IsHconcat);
void callback(int, void*);

int seedPointX = 60, seedPointY = 60;
int loDiff = 20, upDiff = 20;

Mat srcImage;

int main()
{
	//加载原图
	srcImage = imread("../resources/green.jpg");
	if (!srcImage.data) { cout << "加载图片失败！" << endl; return -1; }

	//滑动条
	namedWindow("漫水填充");
	createTrackbar("seedPointX","漫水填充",&seedPointX,srcImage.cols,callback);
	createTrackbar("seedPointY","漫水填充",&seedPointY,srcImage.rows,callback);
	createTrackbar("loDiff","漫水填充",&loDiff,100,callback);
	createTrackbar("upDiff","漫水填充",&upDiff,100,callback);

	waitKey(0);
	destroyAllWindows();
	return 0;
}

void callback(int, void*) 
{
	Mat floodFillImage;
	srcImage.copyTo(floodFillImage);
	Rect ccomp;
	floodFill(floodFillImage,Point(seedPointX, seedPointY),Scalar(155, 55, 255),&ccomp,Scalar(loDiff,loDiff,loDiff),Scalar(upDiff,upDiff,upDiff));
	circle(floodFillImage, Point(seedPointX, seedPointY), 5, Scalar(0, 255, 0), 20);
	//imshow("漫水填充", floodFillImage);
	
	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat(srcImage, floodFillImage, 1);
	//下采样缩小
	pyrDown(result, result);

	//显示频谱幅值图
	imshow("漫水填充", result);
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
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}