#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//图像拼接函数声明
Mat HVconcat(Mat image1, Mat image2, Mat image3, Mat image4, Mat image5, int IsHconcat);
//TrackBar发生改变的回调函数
void onChangeTrackBar(int pos, void *userdata);

Mat srcImage; //原图
int kSize = 5; //高斯核大小

int main()
{
	//加载原图
	srcImage = imread("../resources/40ed.jpg");
	if (!srcImage.data) { cout << "加载图片失败！" << endl; return -1; }

	//新建滑动条窗口
	namedWindow("【原|膨|腐|膨腐|腐膨】");
	createTrackbar("size", "【原|膨|腐|膨腐|腐膨】", &kSize, 30, onChangeTrackBar);
	
	waitKey(0);//在main里waitKey
	destroyAllWindows();
	return 0;
}

/**
 * @brief 滑动条回调函数，方框滤波以及显示
 */
void onChangeTrackBar(int pos, void *userdata)
{
	//膨胀、腐蚀、膨腐、腐膨
	Mat dilateImage,erodeImage,deImage,edImage;
	//高斯核改为奇数
	int newkSize = kSize;
	if (newkSize < 3)
	{
		newkSize = 3;
	} else if (newkSize % 2 != 1)
	{
		newkSize = newkSize + 1;
	}
	//获取自定义核
	Mat element = getStructuringElement(MORPH_RECT, Size(newkSize, newkSize));
	//膨胀 & 腐蚀
	dilate(srcImage, dilateImage, element); //膨
	erode(srcImage, erodeImage, element);   //腐
	erode(dilateImage, deImage,element);    //膨腐
	dilate(erodeImage, edImage, element);   //腐膨

	//图像拼接(1:横向拼接，other:纵向拼接)
	Mat result = HVconcat(srcImage, dilateImage, erodeImage, deImage, edImage, 1);
	//下采样缩小
	pyrDown(result, result);

	//显示频谱幅值图
	imshow("【原|膨|腐|膨腐|腐膨】", result);
}

/**
 * @brief 两张图像横向/纵向拼接
 * @param image1 第一张待拼接图像
 * @param image2 第二张待拼接图像
 * @param IsHconcat 1:横向拼接，other:纵向拼接
 * @return Mat 拼接结果
 */
Mat HVconcat(Mat image1, Mat image2, Mat image3, Mat image4, Mat image5, int IsHconcat = 1)
{
	vector<Mat>vhImage;
	vhImage.push_back(image1);
	vhImage.push_back(image2);
	vhImage.push_back(image3);
	vhImage.push_back(image4);
	vhImage.push_back(image5);
	Mat result;
	1 == IsHconcat ? hconcat(vhImage, result) : vconcat(vhImage, result); //三元运算符
	return result;
}
