#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat);//图像拼接函数声明 
Mat HVconcat3(Mat image1, Mat image2, Mat image3, int IsHconcat);//图像拼接函数声明 
//void callback(int, void*); //滑动条回调函数

Mat srcImage1, grayImage1, srcImage2, grayImage2, addImage, addweightImage, subImage; //原图

//-----------------------------------【main函数】------------------------------------
int main()
{
	srcImage1 = imread("../resources/61.jpg");
	srcImage2 = imread("../resources/71.jpg");
	resize(srcImage1, srcImage1, Size(srcImage2.cols, srcImage2.rows));

	//直接相加
	add(srcImage1, srcImage2, addImage);
	//按权重融合	
	addWeighted(srcImage1, 0.5, srcImage2, 0.5, 2, addweightImage);

	//直接相减
	subtract(srcImage1, srcImage2, subImage);

	//位运算
	cvtColor(srcImage1, grayImage1, COLOR_RGB2GRAY);
	cvtColor(srcImage2, grayImage2, COLOR_RGB2GRAY);
	Mat bit_and = grayImage1.clone(), bit_or = grayImage1.clone(), bit_not = grayImage1.clone(), bit_xor = grayImage1.clone();

	bitwise_and(grayImage1, grayImage2, bit_and); //与
	bitwise_or(grayImage1, grayImage2, bit_or); //或
	bitwise_not(grayImage1, grayImage2, bit_not); //或
	bitwise_xor(grayImage1, grayImage2, bit_xor); //异或

	Mat result1 = HVconcat3(addImage, addweightImage, subImage, 1);
	imshow("直接相加|权重融合|直接相减", result1);

	Mat result2 = HVconcat2(bit_and, bit_or, 1); //与|或
	Mat result3 = HVconcat2(bit_not, bit_xor, 1); //或|异或
	Mat result4 = HVconcat2(result2, result3, 0); //与|或 + 或|异或
	imshow("与|或 + 或|异或", result4);

	waitKey(0);
	destroyAllWindows();

	return 0;
}

//-----------------------------------【自定义函数】------------------------------------
Mat HVconcat2(Mat image1, Mat image2, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}

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

