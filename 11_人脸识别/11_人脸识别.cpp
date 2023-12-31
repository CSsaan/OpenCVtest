#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>

using namespace std;
using namespace cv;

// 检测 & 显示
void detectAndDisplay(Mat frameResult);

String face_cascade_name = "../resources/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "../resources/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Face detection";
Mat srcframe, frameResult;

int main(void)
{
	// 加载xml
	if (!face_cascade.load(face_cascade_name))
	{
		cout << "加载face_cascade_name失败！" << endl;
	}
	if (!eyes_cascade.load(eyes_cascade_name))
	{
		cout << "加载eyes_cascade_name失败！" << endl;
	}

	// 逐帧检测识别
	VideoCapture capture;
	capture.open("../resources/people.mp4");
	while (1)
	{
		capture >> srcframe;
		if (srcframe.empty()) // 播放结束
		{
			break;
		}
		// 开始检测
		pyrDown(srcframe, srcframe);
		pyrDown(srcframe, srcframe);
		srcframe.copyTo(frameResult);
		detectAndDisplay(frameResult);
	}
	return 0;
}


void detectAndDisplay(Mat frameResult)
{
	vector<Rect> faces; //存放所以人脸矩形框
	Mat frame_grey;

	// 先灰度直方图均衡化
	cvtColor(frameResult, frame_grey, COLOR_BGR2GRAY);
	equalizeHist(frame_grey, frame_grey);

	// 人脸检测
	face_cascade.detectMultiScale(frame_grey, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frameResult, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 2, 8, 0);
		// 裁剪人脸部分
		Mat faceROI = frame_grey(faces[i]);
		std::vector<Rect> eyes;

		//在脸中检测眼睛
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(frameResult, eye_center, radius, Scalar(255, 0, 0), 3, 8, 0);
		}
	}

	////图像拼接
	//vector<Mat>vImage;
	//vImage.push_back(srcframe);
	//vImage.push_back(frameResult);
	//Mat result;
	//hconcat(vImage, result);

	////下采样缩小
	//pyrDown(result, result);

	//// 显示
	//imshow(window_name, result);
	imshow(window_name, frameResult);

}