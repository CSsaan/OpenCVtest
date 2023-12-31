#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main()
{
	// 捕获视频（0）
	VideoCapture capture;
	
	capture.open(0); // 视频地址为0，即为打开摄像头
	if (!capture.isOpened())
	{
		cout << "不能初始化摄像头\n";
		return 0;
	}

	// 循环所有帧
	while (1)
	{
		// 加载每帧
		Mat frame;
		capture >> frame;
		
		//// 如果摄像头错误
		//try 
		//{
		//	
		//}
		//catch(exception &e)
		//{
		//	cout << e.what() << endl;
		//	cout << "打开摄像头失败！" << endl;
		//	break;
		//}
		// 显示
		imshow("开启摄像头", frame);
		waitKey(30); // 两帧间隔（约为33fps）
		
	}
	destroyAllWindows();
	return 0;
}