#pragma once
#include "Libraries/Headers/opencv2/core/core.hpp"
#include "Libraries/Headers/opencv2/highgui/highgui.hpp"
#include "Libraries/Headers/opencv2/imgproc/imgproc.hpp"
#include "Interface.h"
#include <fstream>
#include <vector>
using namespace cv;
using namespace std;

class HighPass{
	wstring path;
	wstring pathFilter;
	float kernel[128][128];
	int sizeKernel;
	int factor;
public:
	//set nMask from class interface
	void Set_Factor(float _factor){
		factor = _factor;
	}

	//set path form class interface
	void SetPath(wstring _path){
		path = _path;
	}

	//set path filter from class interface
	void SetPathFilter(wstring _path){
		pathFilter = _path;
	}
	//load filter form file txt
	void LoadFilter(){
		int n;
		fstream input(pathFilter, ios::in);
		if (!input.is_open()){
			return;
		}
		input >> n;
		sizeKernel = n;
		for (int i = 0; i < n; i++){

			for (int j = 0; j < n; j++){
				input >> kernel[i][j];
			}
		}
		input.close();

	}

	//convolution one kernel
	IplImage* Convolution_1_kernel(IplImage*imageRoot, float kernel[][128], int _sizeKernel){

		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		int m = (_sizeKernel - 1) / 2;
		for (int i = 0; i < imageRoot->height; i++)
		{
			for (int j = 0; j < imageRoot->width; j++)
			{
				int sum = 0;
				int temp = i*imageRoot->width + j;

				for (int ii = -m; ii <= m; ii++){
					for (int jj = -m; jj <= m; jj++){
						if (i - ii < 0)
							continue;
						if (i - ii > imageRoot->height - 1)
							continue;
						sum = sum + kernel[ii + m][jj + m] * static_cast<uchar>(imageRoot->imageData[(i - ii)*imageRoot->width + (j - jj)]);
					}
				}
				imgDis->imageData[temp] = saturate_cast<uchar>(sum);
			}
		}
		return imgDis;
	}

	//filter HighPass
	void FiltHighPass(){
		if (pathFilter == L""){
			MessageBox(0, L"Please choose Filter", L"Error", 0);
			return ;
		}
		LoadFilter();
		string s(path.begin(), path.end());
		IplImage* imageRoot = cvLoadImage(&s[0], 0);
		if (imageRoot == NULL)
			return ;

		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		imgDis = Convolution_1_kernel(imageRoot, kernel, sizeKernel);

		string save = "bm" + to_string(countSave++) + ".bmp";
		cvSaveImage(&save[0], imgDis);
	}

	//to use for sobel, robert, prewitt
	IplImage* Cal_2_Kernel(IplImage *imageRoot,float kernelX[][3],float kernelY[][3]){
		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		for (int i = 0; i < imageRoot->height; i++)
		{
			for (int j = 0; j < imageRoot->width; j++)
			{
				int x = 0, y = 0;

				int temp = i*imageRoot->width + j;

				for (int ii = -1; ii <= 1; ii++){
					for (int jj = -1; jj <= 1; jj++){
						if (i - ii< 0){
							continue;
						}
						if (i - ii> imageRoot->height - 1){
							continue;
						}
						x += kernelX[ii + 1][jj + 1] * static_cast<uchar>(imageRoot->imageData[(i - ii)*imageRoot->width + (j - jj)]);
						y += kernelY[ii + 1][jj + 1] * static_cast<uchar>(imageRoot->imageData[(i - ii)*imageRoot->width + (j - jj)]);

					}
				}
				imgDis->imageData[temp] = saturate_cast<uchar>(sqrt(x*x + y*y));
			}
		}
		return imgDis;
	}

	//filter Sobel with kernelX and kernelY
	void FilterSobel()
	{
		float kernelX[3][3] = {
			{ 1, 0, -1 },
			{ 2, 0, -2 },
			{ 1, 0, -1 }
		};
		float kernelY[3][3] = {
			{ -1, -2, -1 },
			{ 0, 0, 0 },
			{ 1, 2, 1 }
		};
		string s(path.begin(), path.end());
		IplImage* imageRoot = cvLoadImage(&s[0], 0);
		if (imageRoot == NULL)
			return ;

		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);

		imgDis=Cal_2_Kernel(imageRoot, kernelX, kernelY);

		string save = "bm" + to_string(countSave++) + ".bmp";
		cvSaveImage(&save[0], imgDis);
	}

	//filter Robert with kernelX and kernelY
	void FilterRobert()
	{
	
		float kernelX[3][3] = {
			{ 0, 0, -1 },
			{0, 1, 0 },
			{0 ,0 ,0}
		};
		float kernelY[3][3] = {
			{ -1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 0 }
		};
		string s(path.begin(), path.end());
		IplImage* imageRoot = cvLoadImage(&s[0], 0);
		if (imageRoot == NULL)
			return ;

		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		
		imgDis = Cal_2_Kernel(imageRoot, kernelX, kernelY);

		string save = "bm" + to_string(countSave++) + ".bmp";
		cvSaveImage(&save[0], imgDis);
	}

	//filter Prewitt with kernelX and kernelY
	void FilterPrewitt()
	{
		
		float kernelX[3][3] = {
			{ 1, 0, -1 },
			{ 1, 0, -1 },
			{ 1, 0, -1 }
		};
		float kernelY[3][3] = {
			{ -1, -1, -1 },
			{ 0, 0, 0 },
			{ 1, 1, 1 }
		};
		string s(path.begin(), path.end());
		IplImage* imageRoot = cvLoadImage(&s[0], 0);
		if (imageRoot == NULL)
			return ;
		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);

		imgDis = Cal_2_Kernel(imageRoot, kernelX, kernelY);

		string save = "bm" + to_string(countSave++) + ".bmp";
		cvSaveImage(&save[0], imgDis);
		return ;
	}

	//filter HighBoost with kernelX and kernelY
	void HighBoost(){
		if (factor <= 0)
			return ;
		float kernelLowpass[3][128] = {
			{ 1/9, 1/9, 1/9 },
			{ 1/9, 1/9, 1/9 },
			{ 1/9, 1/9, 1/9 }
		};
		string s(path.begin(), path.end());
		IplImage* imageRoot = cvLoadImage(&s[0], 0);
		if (imageRoot == NULL)
			return ;
		IplImage* lowpass = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		lowpass = Convolution_1_kernel(lowpass, kernelLowpass,3);
		for (int i = 0; i < imgDis->height; i++)
		{
			for (int j = 0; j < imgDis->width; j++)
			{
				int temp = i*imgDis->width + j;
				imgDis->imageData[temp] = factor*static_cast<uchar>(imageRoot->imageData[temp]) - static_cast<uchar>(lowpass->imageData[temp]);
			}
		}
		string save = "bm" + to_string(countSave++) + ".bmp";
		cvSaveImage(&save[0], imgDis);
	}
};