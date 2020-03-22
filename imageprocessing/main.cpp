#include <core.hpp>
#include <highgui.hpp>
#include <cstdio>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat BgrToGray(Mat img);
Mat ImageProcessing(Mat img);
int GainFunction(int inputValue, float gain, float offset);

int main() {
	Mat img = imread("C:/Users/sangmin/Desktop/Works/images.jpg", IMREAD_COLOR);
	if (img.empty()) {
		printf("image is not exist\n");
		return 0;
	}
	Mat grayImg = BgrToGray(img);
	Mat imageProcessingImg = ImageProcessing(img);
	imshow("origin image", img);
	imshow("gray image", grayImg);
	imshow("image processing result", imageProcessingImg);
	waitKey(0);
	return 0;
}

Mat BgrToGray(Mat img) {
	Mat inputImg = img.clone();
	int width = inputImg.cols;
	int height = inputImg.rows;
	Mat grayimg(height, width, CV_8UC1);
	for (int y = 0; y < height; y++) {
		uchar* inputPoint = inputImg.ptr<uchar>(y);
		uchar* grayPoint = grayimg.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			uchar b = inputPoint[x * 3 + 0];
			uchar g = inputPoint[x * 3 + 1];
			uchar r = inputPoint[x * 3 + 2];
			grayPoint[x] = (b + g + r) / 3;
		}
	}
	return grayimg;
}

Mat ImageProcessing(Mat img) {
	Mat inputImg = img.clone();
	Mat result;
	int width = inputImg.cols;
	int height = inputImg.rows;
	for (int y = 0; y < height; y++) {
		uchar* inputPoint = inputImg.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			uchar* b = &inputPoint[x * 3 + 0];
			uchar* g = &inputPoint[x * 3 + 1];
			uchar* r = &inputPoint[x * 3 + 2];
			*b = GainFunction(*b, 1, 3);
			*g = GainFunction(*g, 1, 0);
			*r = GainFunction(*r, 5, 0);
		}
	}
	result = inputImg.clone();
	return result;
}

int GainFunction(int inputValue, float gain, float offset) {
	float result = inputValue * gain + offset;
	if (result > 255)
		result = 255;
	else if (result < 0)
		result = 0;
	return (int)result;
}