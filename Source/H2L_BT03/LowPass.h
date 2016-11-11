#pragma once
#include "Libraries/Headers/opencv2/core/core.hpp"
#include "Libraries/Headers/opencv2/highgui/highgui.hpp"
#include "Libraries/Headers/opencv2/imgproc/imgproc.hpp"
#include "Interface.h"
#include <fstream>
#include <vector>
using namespace cv;
using namespace std;
extern int countSave=0;
class LowPass{
	wstring path;
	wstring pathFilter;
	float kernel[128][128];
	int sizeKernel;
	int nMask;
public:

	//set nMask from class interface
	void Set_N_Mask(int _nMask){
		nMask = _nMask;
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

	//call to get Median
	int CalcMed(int a[], int n){
		int med;
		for (int i = 0; i < n - 1; i++){
			for (int j = 0; j < n; j++){
				if (a[i] > a[j]){
					int temp = a[i];
					a[i] = a[j];
					a[j] = temp;
				}
			}
		}
		if (n%2==1)
			med = a[n / 2 + 1];
		else 
			med = (a[n / 2 + 1] + a[n / 2]) / 2;
		return med;

	}

	//filter median
	void FiltMedian(){
		int t;
		if (nMask <= 0)
			return ;
		
		int m = (nMask - 1) / 2;
		t = m;
		if (nMask % 2 == 0){
			m = m + 1;
		}
		string s(path.begin(), path.end());
		IplImage* imageRoot = cvLoadImage(&s[0], 0);
		if (imageRoot == NULL)
			return ;
		IplImage* imgDis = cvCreateImage(cvGetSize(imageRoot), 8, 1);
		int sort[1280];

		for (int i = 0; i < nMask*nMask; i++)
			sort[i] = 0;

		for (int i = 0; i < imageRoot->height; i++)
		{
			for (int j = 0; j < imageRoot->width; j++)
			{
				int temp = i*imageRoot->width + j;
				for (int ii = -m; ii <=t; ii++){
					for (int jj = -m; jj <=t; jj++){
						if (i - ii < 0 ){
							continue;
						}
						if (i - ii > imageRoot->height-1){
							continue;
						}
						sort[(ii+m) * nMask + jj+m] = static_cast<uchar>(imageRoot->imageData[(i - ii)*imageRoot->width + (j - jj)]);
					}
				}
				int med = CalcMed(sort, nMask*nMask);
				imgDis->imageData[temp] = saturate_cast<uchar>(med);
			}
		}
		string save = "bm" + to_string(countSave++)+ ".bmp";
		if (nMask==1)
			cvSaveImage(&save[0], imageRoot);
		else 
			cvSaveImage(&save[0], imgDis);

	}

	//filter average
	void FiltAverage(){
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
		int m = (sizeKernel-1) / 2;
		for (int i = 0; i < imageRoot->height; i++)
		{
			for (int j = 0; j < imageRoot->width; j++)
			{
				int sum = 0;
				int temp = i*imageRoot->width + j;

				for (int ii = -m; ii <= m; ii++){
					for (int jj = -m; jj <= m; jj++){
						if (i - ii < 0 )
							continue;
						if (i - ii > imageRoot->height - 1)
							continue;
						sum = sum + kernel[ii+m][jj+m] * static_cast<uchar>(imageRoot->imageData[(i-ii)*imageRoot->width + (j-jj)]);
					}
				}
				imgDis->imageData[temp] = saturate_cast<uchar>(sum);
			}
		}

		string save = "bm" + to_string(countSave++) + ".bmp";
		cvSaveImage(&save[0], imgDis);
	}

};
