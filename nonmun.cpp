#include <iostream>
 
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat NF1(Mat image);
Mat NF2(Mat image, int matX, int matY, int noise);
Mat NFY2(Mat image, int matX, int matY, int noise);
Mat NF4(Mat image, int edge);

Mat NoiseFiltering(Mat image);


int main() {

	Mat img = imread("cement_cracked (280).jpg", CV_8U);  

	//medianBlur(img, img, 3);

	if(img.empty()) {
		cout<<"Error reading image"<<endl;
		return 0;
	}

	imshow("original_img", img);

	NoiseFiltering(img);
	
	waitKey(0);

	return 0;

}

Mat NoiseFiltering(Mat image) {

	Mat athimg, dst;

	adaptiveThreshold(image, athimg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 91, 25);

	bitwise_not(athimg, athimg);   // 반전

	Mat img1 = athimg.clone(), img2 = athimg.clone(), img3 = athimg.clone();


	//NF1(athimg);

	NF2(img1, 10, 2, 7);

	NF2(img2, 4, 4, 9);

	NF4(img3, 0);

	//addWeighted(athimg , 1, img1, 1, 0.0, dst);
	addWeighted(img1, 1, img2, 1, 0.0, dst);
	addWeighted(dst, 1, img3, 1, 0.0, dst);   


	namedWindow("Threshold Image");
	imshow("Threshold Image", athimg);

	imshow("NF2", img1);

	imshow("NF3", img2);

	imshow("NF4", img3);

	imshow("dst", dst);

	return image;

}

Mat NF1(Mat image) {

	int i, j;

	for(i = 0; i < image.cols; i++) {                      
		for(j = 0; j < image.rows; j++) {

			if(image.at<uchar>(j, i) < 200) 
				image.at<uchar>(j, i) = 0;
			
		}
	}

	return image;
}


/*
Mat NF2(Mat image, int matX, int matY, int noise) {

	int nos, m, n;

	for(int x = 0; x < image.rows - matX; x++) {
		for(int y = 0; y < image.cols - matY; y++) {

			Mat ROI(image, Rect(y, x, matY, matX));       // 잡음은 0이 아니다. 17개 이하일 경우 노이즈로 간주하여 제거한다.

			nos = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if((ROI.at<uchar>(m, n)) > 130)        // 해당 픽셀에 값이 잡음일 경우 noise++
						nos++;
				}
			}

			if(nos < noise) {                             // noise가 3보다 작을 경우 모든 픽셀을 검정으로
				
				for(m =0; m < matX; m++) {
					for(n = 0; n < matY; n++) {
						ROI.at<uchar>(m, n) = 0;
					}
				}
			}		
		}
	}

	return image;
}
*/
Mat NF4(Mat image, int edge) {

	int x, y;

	for(x = 0; x < image.rows; x++) {
		for(y = 0; y < image.cols; y++) {
				  
			
			if(y + 2 < image.cols) {
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




