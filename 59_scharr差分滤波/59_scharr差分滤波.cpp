#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
//图像拼接函数声明 
Mat HVconcat4(Mat image1, Mat image2, Mat image3, Mat image4, int IsHconcat);

Mat srcImage; //原图

//-----------------------------------【main函数】------------------------------------
int main()
{
	Mat scharrDxImage, scharrDyImage, scharrDxDyImage;
	//加载原图
	srcImage = imread("../resources/03.jpg");
	if (!srcImage.data) { return -1; }

	//计算scharr差分滤波
	Scharr(srcImage, scharrDxImage, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(scharrDxImage, scharrDxImage);
	Scharr(srcImage, scharrDyImage, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(scharrDyImage, scharrDyImage);
	addWeighted(scharrDxImage, 0.5, scharrDyImage, 0.5, 0, scharrDxDyImage);

	//拼接
	Mat result = HVconcat4(srcImage, scharrDxImage, scharrDyImage, scharrDxDyImage, 1);
	pyrDown(result, result);

	imshow("scharr差分滤波", result);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

//-----------------------------------【自定义函数】------------------------------------
/**
 * @brief 4张图像横向/纵向拼接
 * @param imageN 第N张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat4(Mat image1, Mat image2, Mat image3, Mat image4, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	vhImage.push_back(image3);
	vhImage.push_back(image4);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}