
#include <iostream>
 
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat NF1(Mat image, int min);
Mat NF2(Mat image, int matX, int matY, int noise);

Mat NF4(Mat image, int edge);
Mat NFx4(Mat image, int edge);

Mat NoiseFiltering(Mat image);
Mat Histogram(Mat image);

Mat GRASSFIRE(Mat image, int matX, int matY, float f);


int main() {

	Mat img = imread("cement_cracked (590).jpg", CV_8U);
	Mat img1 = imread("cement_cracked (12).jpg", CV_8U); 

	if(img.empty()) {
		cout<<"Error reading image"<<endl;
		return 0;
	}

	int min = 255;

	
	for(int i = 0; i < img.cols; i++) {                      
		for(int j = 0; j < img.rows; j++) {

			if(img.at<uchar>(j, i) < min) 
				min = img.at<uchar>(j, i);
						
		}
	}

	int min1 = 255;

	for(int i = 0; i < img1.cols; i++) {                      
		for(int j = 0; j < img1.rows; j++) {

			if(img1.at<uchar>(j, i) < min) 
				min1 = img1.at<uchar>(j, i);
						
		}
	}
	//printf("min = %d\n", min);

	/*
	Mat img_sharpening;
	img_sharpening.create(img.size(), img.type());
	const int nChannels = img.channels();

	for(int j = 1; j < img.rows - 1; ++j)
	{
		const uchar* previous = img.ptr<uchar>(j - 1);
		const uchar* current = img.ptr<uchar>(j);
		const uchar* next = img.ptr<uchar>(j + 1);

		uchar* output = img_sharpening.ptr<uchar>(j);
		for(int i = nChannels; i < nChannels*(img.cols - 1); ++i)
		{
			*output++ = saturate_cast<uchar>(5 * current[i] - current[i - nChannels]  - current[i+nChannels] - previous[i] - next[i]);
		}
	}

	img_sharpening.row(0).setTo(Scalar(0));
	img_sharpening.row(img_sharpening.rows - 1).setTo(Scalar(0));
	img_sharpening.col(0).setTo(Scalar(0));
	img_sharpening.col(img_sharpening.cols - 1).setTo(Scalar(0));
	imshow("Sharpening", img_sharpening);

	*/
	
	NF1(img, min);
	NF1(img1, min1);

	medianBlur(img1, img1, 3);
	medianBlur(img, img,  3);

	img = 255 - img;
	img1 = 255 - img1;

	NFx4(img1, 0);
	NFx4(img, 0);

	imshow("img", img);
	imshow("img1", img1);
	
	NoiseFiltering(img);
	//img1 = NoiseFiltering(img1);

	//imshow("dst", img);
	//imshow("dst1", img1);
	
	waitKey(0);

	return 0;

}

Mat NoiseFiltering(Mat image) {

	Mat dst, dst1;

	dst.create(image.size(), CV_8U);

	dst = 0;

	Mat img1 = image.clone(), img2 = image.clone(), img3 = image.clone();

	//NF2(img1, 10, 3, 12);

	int num1 = 0, num2 = 0;
	NF2(img2, 4, 4, 8); // NF3

	for(int i = 0; i < img2.cols; i++) {                      
		for(int j = 0; j < img2.rows; j++) {

			if(img2.at<uchar>(j, i) != 0 && img2.at<uchar>(j, i) != 255)  // 77
				num1++;			
		}
	}

	printf("num = %d\n", num1);
	if(num1 > 2000) {
		for(int i = 0; i < img2.cols; i++) {                      
			for(int j = 0; j < img2.rows; j++) {

			if(img2.at<uchar>(j, i) != 0 && img2.at<uchar>(j, i) != 255)  // 77
				img2.at<uchar>(j, i) = 0;			
			}
		}
	}

	NF4(img3, 0);

	for(int i = 0; i < img3.cols; i++) {                      
		for(int j = 0; j < img3.rows; j++) {

			if(img3.at<uchar>(j, i) != 0 && img3.at<uchar>(j, i) != 255)  // 77
				num1++;			
		}
	}

	printf("num = %d\n", num1);
	if(num1 > 2000) {
		for(int i = 0; i < img3.cols; i++) {                      
			for(int j = 0; j < img3.rows; j++) {

			if(img3.at<uchar>(j, i) != 0 && img3.at<uchar>(j, i) != 255)  // 77
				img3.at<uchar>(j, i) = 0;			
			}
		}
	}


	dst = img2 + img3;

	imshow("dst befor", dst);
	
	//adaptiveThreshold(dst, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 91, 49);
	////////////////////////////////////////////////////////////////////////////////
	
	Mat binary = dst.clone();

	// 외곽선을 포함하는 벡터
	vector<vector<Point>> contours;

	// 연결 성분의 외곽선 얻기
	findContours (binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  
	// 모든 외관선에 대해
	vector<vector<Point>>::iterator it = contours.begin();

	// POLY
	vector<Point> poly;

	while (it != contours.end())
	{
		poly.clear();

		// 다각형으로 외곽선 근사화
		approxPolyDP(*it, poly, 10, true);

		// 사각형을 갖는가?
		if (poly.size() == 1) 
			// 사각형을 그린다
			polylines(dst, poly, true, 0, 10);
   
		++it;
	}
	
	////////////////////////////////////////////////////////////////////////////////
	
	medianBlur(dst, dst, 3);

	//NF4(dst, 0);

	////////////////////////////////////////////////////////////////////////////////
	
	Mat binary1 = dst.clone();

	// 외곽선을 포함하는 벡터
	vector<vector<Point>> contours1;

	// 연결 성분의 외곽선 얻기
	findContours (binary1, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  
	// 모든 외관선에 대해
	vector<vector<Point>>::iterator it1 = contours1.begin();

	// POLY
	vector<Point> poly1;

	while (it1 != contours1.end())
	{
		poly1.clear();

		// 다각형으로 외곽선 근사화
		approxPolyDP(*it1, poly1, 10, true);

		// 사각형을 갖는가?
		if (poly1.size() == 1) 
			// 사각형을 그린다
			polylines(dst, poly1, true, 0, 10);
   
		++it1;
	}
	
	////////////////////////////////////////////////////////////////////////////////


	//imshow("NF2", img1);
	imshow("NF3", img2);
	imshow("NF4", img3);
	imshow("dst", dst);
	//imwrite("dst.jpg", dst);

	return dst;

}

Mat NF1(Mat image, int min) {

	for(int i = 0; i < image.cols; i++) {                      
		for(int j = 0; j < image.rows; j++) {

			if(image.at<uchar>(j, i) < min + 97)  // 97
				image.at<uchar>(j, i) = 0;
			else if(image.at<uchar>(j, i) > min + 124) //124
				image.at<uchar>(j, i) = 255;			
		}
	}

	return image;
}

Mat NF4(Mat image, int edge) {

	int x, y;

	for(x = 0; x < image.rows; x++) {
		for(y = 0; y < image.cols; y++) {
				  
			
			if(y + 2 < image.cols && image.at<uchar>(x, y) > edge) {
				if(image.at<uchar>(x, y) > edge && 
				   image.at<uchar>(x, y + 1) > edge &&
				   image.at<uchar>(x, y + 2) > edge) {
				
					   y += 2;
				}
				else {
					image.at<uchar>(x, y) = 0;
					image.at<uchar>(x, y + 1) = 0;
					image.at<uchar>(x, y + 2) = 0;

					y += 2;
				}
			}
			
		}
	}

	return image;
}


Mat NFx4(Mat image, int edge) {

	int x, y;

	for(y = 0; y < image.cols; y++) {
		for(x = 0; x < image.rows; x++) {
				  
			/*
			if(x + 3 < image.rows && image.at<uchar>(x, y) > edge) {
				if(image.at<uchar>(x+1, y) == 255 ||
				   image.at<uchar>(x+2, y) == 255 ||
				   image.at<uchar>(x+3, y) == 255) {
				
					   x += 3;
				}
				else if(image.at<uchar>(x+1, y) > edge &&
				   image.at<uchar>(x+2, y) > edge &&
				   image.at<uchar>(x+3, y) > edge) {
					   x += 3;
				}
				else {
					image.at<uchar>(x, y) = 0;
					image.at<uchar>(x+1, y) = 0;
					image.at<uchar>(x+2, y) = 0;
					image.at<uchar>(x+3, y) = 0;

					x += 3;
				}
			}*/
			if(x + 2 < image.rows && image.at<uchar>(x, y) > edge) {
				if(image.at<uchar>(x+1, y) == 255 ||
				   image.at<uchar>(x+2, y) == 255 ) {
				
					   x += 2;
				}
				else if(image.at<uchar>(x+1, y) > edge &&
				   image.at<uchar>(x+2, y) > edge ) {
					   x += 2;
				}
				else {
					image.at<uchar>(x, y) = 0;
					image.at<uchar>(x+1, y) = 0;
					image.at<uchar>(x+2, y) = 0;

					x += 2;
				}
			}
			
		}
	}

	return image;
}


Mat NF2(Mat image, int matX, int matY, int noise) {

	int nos, m, n;

	for(int x = 0; x <= image.rows - matX; x++) {
		for(int y = 0; y <= image.cols - matY; y++) {

			Mat ROI(image, Rect(y, x, matY, matX));

			nos = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if(ROI.at<uchar>(m, n) > 0)        // 해당 픽셀에 값이 잡음일 경우 noise++
						nos++;
				}
			}

			if(nos < noise) {                             
				
				for(m =0; m < matX; m++) {
					for(n = 0; n < matY; n++) {
						ROI.at<uchar>(m, n) = 0;
					}
				}
			}

			y += (matY - 1); 

		}
		x += (matX - 1);
	}

	return image;
}









