#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//------------------------------【函数声明 & 数值声明】-------------------------------
int labelTarget(Mat& src, Mat& mask, int thresh = 100);

//-----------------------------------【main函数】------------------------------------
int main()
{
	Mat frame, foreGround, backGround, fgMask;

	//加载视频
	VideoCapture cap;

	//打开视频
	cap.open("../resources/20.avi");
	if (!cap.isOpened())
	{
		cout << "无法打开视频！" << endl;
		return -1;
	}

	//高斯背景模型
	Ptr<BackgroundSubtractorMOG2> pBgModel = createBackgroundSubtractorMOG2();
	pBgModel->setHistory(20);  //最后的帧数
	pBgModel->setVarThreshold(100); //阈值
	pBgModel->setDetectShadows(false); //是否检测阴影

	//读取视频帧
	while (1)
	{
		cap >> frame; //读取视频帧
		if (frame.empty()) { break; }
		resize(frame, frame, Size(frame.cols / 2, frame.rows / 2), INTER_LINEAR);
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (foreGround.empty())
		{
			foreGround.create(frame.size(), frame.type());
		}
		//模型背景更新
		pBgModel->apply(frame, fgMask);

		//二值化
		//GaussianBlur(fgMask, fgMask, Size(5, 5), 0);
		//threshold(fgMask, fgMask, 30, 255, THRESH_BINARY);

		foreGround = Scalar::all(0);
		//fgMask为0的像素，设为透明
		frame.copyTo(foreGround, fgMask);

		imshow("前景", foreGround);

		//标记找到的目标
		int nTargets = labelTarget(frame, fgMask);
		cout << "共找到目标个数：" << nTargets << endl;
	}


	return 0;
}

//-----------------------------------【自定义函数】------------------------------------
/**
 * @brief 图像前后景分割
 * @param src 前景
 * @param mask 掩膜
 * @param thresh 阈值
 * @return
 */
int labelTarget(Mat& src, Mat& mask, int thresh)
{
	//以下是图像分割
	Mat seg = mask.clone();
	vector<vector<Point>> cnts;
	findContours(seg, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//以下进行筛选
	float area;
	Rect rect;
	int count = 0;
	string strCount;
	for (int i = cnts.size() - 1; i >= 0; i--)
	{
		vector<Point> c = cnts[i];
		area = contourArea(c);
		if (area < thresh)
			continue;
		count++;
		//包围特征点的矩形框的坐标和长宽信息
		rect = boundingRect(c);
		//输出矩形框的坐标和面积
		cout << "位置：（" << rect.x << "，" << rect.y << "）面积：" << rect.width * rect.height << endl;
		//在原图上画出矩形框
		rectangle(src, rect, Scalar(0, 0, 255), 1);

		stringstream ss;
		ss << count;
		ss >> strCount;
		putText(src, strCount, Point(rect.x, rect.y), FONT_HERSHEY_PLAIN, 0.5, Scalar(0, 255, 0));

		imshow("src", src);
		waitKey(20);
	}
	return count;
}