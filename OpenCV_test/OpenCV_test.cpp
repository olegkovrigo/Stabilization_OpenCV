#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/mat.hpp>
#include <limits>
#include <stdio.h>
#include "windows.h"
#include <cassert>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>  

using namespace cv;
using namespace std;
//////////////////////////////////////////////////Адрес/////////////////////////////////////////////
const String image_PATH = "C:/unity_3d/Unity_BLA/";
///////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{   
	setlocale(LC_ALL, "Russian");
	int sumx = 0;
	int sumy = 0;
	int& sumxf = sumx;
	int& sumyf = sumy;

	String image_path1 = image_PATH + "image_1.jpg";
	///////////////////�������� � ����� ����////////////////////////
	Mat image1 = imread(image_path1);
	const int block_size = round(480 / 5);
	const int range = 10;
	const int X = round(image1.cols / 2 - block_size / 2);//��������� �������� �� ��� X
	const int Y = round(image1.rows / 2 - block_size / 2);//��������� �������� �� ��� Y
	VideoWriter writer;
	int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)                   
	string filename = image_PATH + "GS_VisualSt.avi";// name of the output video file
	CvSize size = cvSize(image1.cols, image1.rows);
	writer.open(filename, codec, 25, size);

	//clock_t start = clock();
	for (int k = 0; k < 200; k++)
	{
		clock_t start = clock();
		//cout << k << endl;
		/////////////////////����� ����������///////////////////////////
		String image_path1 = image_PATH + "image_" + to_string(k) + ".jpg";
		String image_path2 = image_PATH + "image_" + to_string(k + 1) + ".jpg";
		///////////////////�������� � ����� ����////////////////////////
		Mat image1 = imread(image_path1);
		Mat image1_gray;
		cvtColor(image1, image1_gray, COLOR_RGB2GRAY);
		Mat image2 = imread(image_path2);
		Mat image2_gray;
		cvtColor(image2, image2_gray, COLOR_RGB2GRAY);
		Mat Out_frame = Mat::zeros(image2.size(), image2.type());

		////////////////////////////////////////////////////////////////
		int dy = 0;
		int dx = 0;
		Rect reg_image2(X, Y, block_size, block_size);//RECT - ��������� ������ � �������� ������� : x, y, width, height.]
		Mat image1_find(Size(block_size, block_size), CV_8UC3);//��� ������ ����� ����������� 
		Mat image2_find(Size(block_size, block_size), CV_8UC3);//� ����������� ������
		image2_gray(reg_image2).copyTo(image2_find);
		Mat REZ(Size(block_size, block_size), CV_8UC3);//�������� �������
		int MAD_min = INT_MAX;

		for (int m = -range; m < range + 1; m++)
		{
			for (int n = -range; n < range + 1; n++)
			{
				int refBlkVer = Y + m;//row Vert co - ordinate for ref block
				int	refBlkHor = X + n;//col Horizontal co - ordinate
				//////////////////////////////////////////////////////////////////////////////////
				Rect reg_image1(refBlkHor, refBlkVer, block_size, block_size);
				///////////////////// ����������� ������� � ��������� �����������/////////////////
				image1_gray(reg_image1).copyTo(image1_find);
				//REZ = abs(image2_find - image1_find);

				absdiff(image2_find, image1_find, REZ);
				int MAD = 0;
				for (int i = 0; i < REZ.rows; i++)
				{
					for (int j = 0; j < REZ.cols; j++)
					{
						MAD += REZ.ptr<uchar>(i)[j];
						//cout << REZ.ptr<uchar>(i)[j] << ' ' ;
					}
					//cout << '\n';
				}
				//
				if (MAD < MAD_min)
				{
					MAD_min = MAD;
					dy = refBlkVer;
					dx = refBlkHor;
				}
			}
		}
		/////////////////////////////���������� ���//////////////////////////////////////////////////
		sumxf = round(sumxf * 0.9 + dx - X);
		sumyf = round(sumyf * 0.9 + dy - Y);
		//cout << "M_vx=" << sumxf << " M_vy=" << sumyf << endl;
		/////////////////����� ���������� �� �����//////////////////////

		Rect source = Rect(max(0, -sumxf), max(0, -sumyf), image2.cols - abs(sumxf), image2.rows - abs(sumyf));
		Rect target = Rect(max(0, sumxf), max(0, sumyf), image2.cols - abs(sumxf), image2.rows - abs(sumyf));

		image2(source).copyTo(Out_frame(target));
		///////////////////////////////////////���������� � ����////////////////////////////////////////////////
	    //imshow("Stabilization_Video", Out_frame);	waitKey(1);
		writer.write(Out_frame);
		clock_t end = clock();
		double seconds = (double)(end - start) / CLOCKS_PER_SEC;
		printf("The time: %f seconds\n", seconds);
		//cout << "t=" << search_time << " ms" << endl;
	}
	/*clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds);*/
	return 0;
}


