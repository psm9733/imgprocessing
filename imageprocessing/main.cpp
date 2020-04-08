#include <core.hpp>
#include <highgui.hpp>
#include <cstdio>
#include <stdio.h>
using namespace cv;
using namespace std;

Mat BgrToGray(Mat img);
// Mat Padding(Mat img);
Mat ConvolutionCal(Mat img, int* kernel);
Mat ImageProcessing(Mat img, int mode);
int GainFunction(int inputValue, float gain, float offset);

float x_derivative_kernel[3][3] = { {-1.0, 0.0, 1.0},
								{-1.0, 0.0, 1.0},
								{-1.0, 0.0, 1.0} };

float y_derivative_kernel[3][3] = { {-1.0, -1.0, -1.0},
								{0.0, 0.0, 0.0},
								{1.0, 1.0, 1.0} };

float blur_kernel[3][3] = { {1.0 / 16, 2.0 / 16, 1.0 / 16},
						{2.0 / 16, 4.0 / 16, 2.0 / 16},
						{1.0 / 16, 2.0 / 16, 1.0 / 16} };

float sharpen_kernel[3][3] = { {-1.0, -1.0, -1.0},
							{-1.0, 9.0, -1.0},
							{-1.0, -1.0, -1.0} };

int main() {
	Mat img = imread("C:/Users/fs/Desktop/imgprocessing/image5.jpg", IMREAD_COLOR);
	if (img.empty()) {
		printf("image is not exist\n");
		return 0;
	}
	
	Mat grayImg = BgrToGray(img);
	Mat xDerImg = ImageProcessing(img, 1);
	Mat yDerImg = ImageProcessing(img, 2);
	Mat blurImg = ImageProcessing(img, 3);
	Mat sharpenImg = ImageProcessing(img, 4);
	
	imshow("origin image", img);
	imshow("gray image", grayImg);
	imshow("xDerImg", xDerImg);
	imshow("yDerImg", yDerImg);
	imshow("blurImg", blurImg);
	imshow("sharpenImg", sharpenImg);
	
	waitKey(0);
	return 0;
}

Mat BgrToGray(Mat img) {
	Mat targetImg = img.clone();
	int width = targetImg.cols;
	int height = targetImg.rows;
	Mat grayimg(height, width, CV_8UC1);
	for (int y = 0; y < height; y++) {
		uchar* inputPoint = targetImg.ptr<uchar>(y);
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

Mat ConvCalculator(Mat img, float *kernel) {
	Mat targetImg = img.clone();
	int height = targetImg.rows;
	int width = targetImg.cols;
	Mat resultImg = Mat::zeros(height, width, CV_8UC3);
	uchar* targetImgPointer = targetImg.data;
	uchar* resultImgPointer = resultImg.data;
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			int index = y * width + x;
			for (int z = 0; z < 3; z++) {
				resultImgPointer[3 * index + z] =
					(float)targetImgPointer[3 * (index - width - 1) + z] * (*(kernel + 0)) +
					(float)targetImgPointer[3 * (index - width + 0) + z] * (*(kernel + 1)) +
					(float)targetImgPointer[3 * (index - width + 1) + z] * (*(kernel + 2)) +
					(float)targetImgPointer[3 * (index - 1) + z] * (*(kernel + 3)) +
					(float)targetImgPointer[3 * (index + 0) + z] * (*(kernel + 4)) +
					(float)targetImgPointer[3 * (index + 1) + z] * (*(kernel + 5)) +
					(float)targetImgPointer[3 * (index + width - 1) + z] * (*(kernel + 6)) +
					(float)targetImgPointer[3 * (index + width + 0) + z] * (*(kernel + 7)) +
					(float)targetImgPointer[3 * (index + width + 1) + z] * (*(kernel + 8));
			}
		}
	}
	return resultImg;
}

Mat ImageProcessing(Mat img, int mode) {	//mode = 0 -> gain,	mode = 1 -> x_derivative,	mode = 2 -> y_derivative
	Mat targetImg = img.clone();				//mode = 3 -> blur, mode = 4 -> sharpen
	Mat result;
	int input_height = targetImg.rows;
	int input_width = targetImg.cols;
	Mat zeropaddingImg;
	copyMakeBorder(targetImg, zeropaddingImg, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0, 0, 0));	//padding
	if (mode == 0) {			//gain processing
		for (int y = 0; y < input_height; y++) {
			uchar* imgPoint = targetImg.ptr<uchar>(y);
			for (int x = 0; x < input_width; x++) {
				uchar* b = &imgPoint[x * 3 + 0];
				uchar* g = &imgPoint[x * 3 + 1];
				uchar* r = &imgPoint[x * 3 + 2];
				*b = GainFunction(*b, 1, 3);
				*g = GainFunction(*g, 1, 0);
				*r = GainFunction(*r, 5, 0);
			}
		}
	}
	else {
		if (mode == 1) {			//x_derivative
			result = ConvCalculator(targetImg, *x_derivative_kernel);
		}
		else if (mode == 2) {		//y_derivative
			result = ConvCalculator(targetImg, *y_derivative_kernel);
		}
		else if (mode == 3) {		//blur
			result = ConvCalculator(targetImg, *blur_kernel);
		}
		else if (mode == 4) {		//sharpen
			result = ConvCalculator(targetImg, *sharpen_kernel);
		}
	}
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