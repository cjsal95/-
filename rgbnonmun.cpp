#include <iostream>
 
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat NF1(Mat image);
Mat NF2(Mat image, int matX, int matY, int noise);

Mat NF4(Mat image, int edge);
Mat NFx4(Mat image, int edge);

Mat NoiseFiltering(Mat image);
Mat Histogram(Mat img_input);

Mat GRASSFIRE(Mat image, int matX, int matY, float f);


int main() {

	//Mat img = imread("cement_cracked (596).jpg", CV_LOAD_IMAGE_COLOR);  
	Mat img = imread("cement_cracked (22).jpg", CV_8U);  

	if(img.empty()) {
		cout<<"Error reading image"<<endl;
		return 0;
	}
	/*
	Mat Rimg, Gimg, Bimg;

	vector<Mat> channels;
	split(img, channels);    // 채널 분리

	Bimg = channels[0];
	Gimg = channels[1];
	Rimg = channels[2];

	adaptiveThreshold(Bimg, Bimg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 91, 49); // 69, 65
	adaptiveThreshold(Gimg, Gimg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 91, 49);
	adaptiveThreshold(Rimg, Rimg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 91, 49);

	medianBlur(Bimg, Bimg, 5);
	medianBlur(Gimg, Gimg, 5);
	medianBlur(Rimg, Rimg, 5);


	//imshow("Bimg", Bimg);
	//imshow("Gimg", Gimg);
	//imshow("Rimg", Rimg);

	img = channels[0] + channels[1] + channels[2];   // 채널 합체!
	*/
	medianBlur(img, img, 3);
	adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 91, 43);
	medianBlur(img, img, 3);

	NoiseFiltering(img);
	
	waitKey(0);

	return 0;

}

Mat NoiseFiltering(Mat image) {

	Mat dst, dst1;

	bitwise_not(image, image);   // 반전
	
	////////////////////////////////////////////////////////////////////////////////
	
	Mat binary = image.clone();

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
		if (poly.size() < 2) 
			// 사각형을 그린다
			polylines(image, poly, true, 0, 4);
   
		++it;
	}
	
	////////////////////////////////////////////////////////////////////////////////
	imshow("image", image);

	Mat img1 = image.clone(), img2 = image.clone(), img3 = image.clone(), img4 = image.clone(), img5 = image.clone();

	NF2(img1, 8, 2, 13);

	NF2(img2, 4, 4, 12);

	NF4(img3, 0);

	//NFx4(img4, 0);
	/*
	for(int i = 0; i < img3.rows; i++) {                      
		for(int j = 0; j < img3.cols; j++) {

			if(img3.at<uchar>(i, j) != img4.at<uchar>(i, j)) 
				img4.at<uchar>(i, j) = 0;
		}
	}
	*/
	medianBlur(img3, img3, 3);

	//NF2(img4, 5, 5, 24);

	//addWeighted(athimg , 1, img1, 1, 0.0, dst);
	addWeighted(img1, 1, img2, 1, 0.0, dst);
	addWeighted(dst, 1, img3, 1, 0.0, dst1);

	imshow("NF2 + NF3", dst);

	imshow("NF2", img1);

	imshow("NF3", img2);

	imshow("NF4", img3);

	//imshow("NFx4", img4);

	imshow("dst", dst1);

	imwrite("dst.jpg", dst);

	return dst;

}

Mat NF1(Mat image) {

	int i, j;

	for(i = 0; i < image.cols; i++) {                      
		for(j = 0; j < image.rows; j++) {

			if(image.at<uchar>(j, i) < 90) 
				image.at<uchar>(j, i) = 0;
			else
				image.at<uchar>(j, i) = 255;
			
		}
	}

	return image;
}



Mat NF4(Mat image, int edge) {

	int x, y;

	for(x = 0; x < image.rows; x++) {
		for(y = 0; y < image.cols; y++) {
				  
			
			if(y + 3 < image.cols && image.at<uchar>(x, y) > edge) {
				if(image.at<uchar>(x, y) > edge && 
				   image.at<uchar>(x, y + 1) > edge &&
				   image.at<uchar>(x, y + 2) > edge &&
				   image.at<uchar>(x, y + 3)) {
				
					   y += 3;
				}
				else {
					image.at<uchar>(x, y) = 0;
					image.at<uchar>(x, y + 1) = 0;
					image.at<uchar>(x, y + 2) = 0;
					image.at<uchar>(x, y + 3) = 0;

					y += 3;
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
				  
			
			if(x + 3 < image.rows && image.at<uchar>(x, y) > edge) {
				if(image.at<uchar>(x, y) > edge && 
				   image.at<uchar>(x+1, y) > edge &&
				   image.at<uchar>(x+2, y) > edge &&
				   image.at<uchar>(x+3, y)) {
				
					   x += 3;
				}
				else {
					image.at<uchar>(x, y) = 0;
					image.at<uchar>(x+1, y) = 0;
					image.at<uchar>(x+2, y) = 0;
					image.at<uchar>(x+3, y) = 0;

					x += 3;
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









Mat GRASSFIRE(Mat image, int matX, int matY, float f) {

	int noise, m, n;
	float vote;

	for(int x = 0; x <= image.rows - matX; x++) {
		for(int y = 0; y <= image.cols - matY; y++) {

			Mat ROI(image, Rect(y, x, matY, matX));

			noise = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if(ROI.at<uchar>(m, n) > 0)        // 해당 픽셀에 값이 잡음일 경우 noise++
						noise++;
				}
			}

			vote = noise / (matX * matY);


			if(f < vote) {                             
				
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