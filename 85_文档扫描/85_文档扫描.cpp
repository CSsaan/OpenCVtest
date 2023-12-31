#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//------------------------------【函数声明 & 数值声明】-------------------------------
Mat srcImage, preImage, imgWarp, imgCrop;
vector<Point>initalPoints;
vector<Point>finalPoints;
float w = 420, h = 596; //A4纸（210mm×297mm）

Mat preProcessing(Mat img);
vector<Point>getContours(Mat imgDil);
vector<Point> reorder(vector<Point> initalPoints);
Mat getWarp(Mat imgOriginal, vector<Point> finalPoints, float w, float h);
//-----------------------------------【main函数】------------------------------------
int main()
{
	//灰度+边缘检测
	srcImage = imread("../resources/OCR.jpg");
	preImage = preProcessing(srcImage);

	//检测最大轮廓的四个点(左上角，右下角)
	initalPoints = getContours(preImage); //(268, 64), (91, 319)
	/*for (int i = 0; i < initalPoints.size(); i++)
	{
		cout << initalPoints[i].x << "," << initalPoints[i].y << endl;
	}*/

	//修改点顺序
	finalPoints = reorder(initalPoints);

	//变换图片
	imgWarp = getWarp(srcImage, finalPoints, w, h);

	//裁剪一下边缘非文档部分
	int cropNum = 5;
	Rect roi(cropNum, cropNum, w - 2 * cropNum, h - 2 * cropNum);
	imgCrop = imgWarp(roi);

	//显示
	imshow("srcImage", srcImage);
	imshow("preImage", preImage);
	imshow("imgWarp", imgWarp);
	imshow("imgCrop", imgCrop);
	waitKey(0);

	return 0;
}


//-----------------------------------【自定义函数】------------------------------------
Mat preProcessing(Mat img)
{
	Mat imgBlur, imgCanny, imgDil;
	//转灰度
	cvtColor(img, img, COLOR_RGB2GRAY);
	//高斯降噪+边缘检测
	GaussianBlur(img, imgBlur, Size(5, 5), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75, 3);
	//边缘膨胀
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//膨胀的自定义核
	dilate(imgCanny, imgDil, element); //膨胀
	return imgDil;
}

vector<Point>getContours(Mat imgDil)
{
	vector<vector<Point>> contours; //所以轮廓
	vector<Vec4i> hierarchy; //每个轮廓的四个点
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conpoly(contours.size());
	vector<Point> biggest; //最大轮廓的四个点
	int maxArea = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]); //计算每个轮廓的面积(72976)
		if (area > 1000) // 筛选大的轮廓
		{
			float peri = arcLength(contours[i], true);//计算闭合轮廓长度(1146)
			approxPolyDP(contours[i], conpoly[i], 0.02 * peri, true);//轮廓点稀疏化
			if (area > maxArea && conpoly[i].size() == 4)//查找最大的面积，同时稀疏化的轮廓点为4
			{
				maxArea = area;
				biggest = { conpoly[i][0], conpoly[i][1] ,conpoly[i][2] ,conpoly[i][3] };
				drawContours(srcImage, conpoly, i, Scalar(255, 0, 0), 2); // 只需要绘制最大图形即可
			}
		}
	}
	return biggest;
}

vector<Point> reorder(vector<Point> initalPoints)
{
	vector<Point> tempPoints; // 设置中间变量
	vector<double> sumPoints, subPoints;
	int sumMinIndex, sumMaxIndex; //  查找sum的最大(3)最小值(0)索引
	int subMinIndex, subMaxIndex; // 查找sub的最大()最小值()索引
	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back((double)initalPoints[i].x + initalPoints[i].y); // 和值最大为第四个点
		//和值最小为第一个点 
		// 而 2 3 点则是通过y - x 进行判断
		subPoints.push_back((double)initalPoints[i].x - initalPoints[i].y);
	}
	sumMinIndex = min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin(); // 0
	subMaxIndex = max_element(subPoints.begin(), subPoints.end()) - subPoints.begin(); //1
	subMinIndex = min_element(subPoints.begin(), subPoints.end()) - subPoints.begin();
	sumMaxIndex = max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin();// 3
	tempPoints.push_back(initalPoints[sumMinIndex]); // 0 即最小元素 
	tempPoints.push_back(initalPoints[subMaxIndex]);
	tempPoints.push_back(initalPoints[subMinIndex]);
	tempPoints.push_back(initalPoints[sumMaxIndex]);
	return tempPoints;
}

Mat getWarp(Mat imgOriginal, vector<Point> finalPoints, float w, float h)
{
	Point2f src1[4] = { finalPoints[0], finalPoints[1], finalPoints[2], finalPoints[3] }; //图像中四个点的坐标
	Point2f src2[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f,h}, {w,h} };//待变换到完整屏幕的坐标
	Mat martix = getPerspectiveTransform(src1, src2);//获取变化矩阵
	Mat imgWarp;
	warpPerspective(imgOriginal, imgWarp, martix, Size(w, h)); //变换投影
	return imgWarp;
}