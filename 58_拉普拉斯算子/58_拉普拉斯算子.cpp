#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
//图像拼接函数声明 
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat);
void callback(int, void*); //滑动条回调函数

int ksize = 1; //卷积核为ksize(1,3,5,7)

Mat srcImage; //原图

//-----------------------------------【main函数】------------------------------------
int main()
{
	//加载原图
	srcImage = imread("../resources/03.jpg");
	if (!srcImage.data) { return -1; }

	////滑动条
	namedWindow("srcImage|laplacianImage");
	createTrackbar("threshold", "srcImage|laplacianImage", &ksize, 3, callback);

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
	Mat gaussianImage, gaussiangrayImage, laplacianImage;
	//先高斯滤波降噪
	GaussianBlur(srcImage, gaussianImage, Size(3, 3), 0, 0);
	//转灰度
	cvtColor(gaussianImage, gaussiangrayImage, COLOR_RGB2GRAY);
	//再求laplacian
	Laplacian(gaussiangrayImage, laplacianImage, CV_16S, ksize);
	convertScaleAbs(laplacianImage, laplacianImage); //Sobel之后需要转回uint8型才能正常显示
	//图像拼接(1:横向拼接，other:纵向拼接)
	imshow("1", laplacianImage);
	//Mat result = HVconcat2(srcImage, laplacianImage, 1); //横向拼接
	////下采样缩小
	//pyrDown(result, result);
	////显示频谱幅值图
	//imshow("srcImage|laplacianImage", result);
}

/**
 * @brief 4张图像横向/纵向拼接
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