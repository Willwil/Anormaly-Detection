#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string.h>

using namespace std;
using namespace cv;

Mat img;
float op[4][2], tp[4][2];
int pneed = 0;
Point prev;

void on_mouse(int event, int x, int y, int flags, void* dr)
{
	if(pneed == 4)
		return;
    //左键点击 按下
    if (event == CV_EVENT_LBUTTONDOWN)
    {    
          printf("x: %d, y: %d\n", x, y);
          op[pneed][0] = x;
          op[pneed][1] = y;
          Point p1, p2, prev;
          p1.x = x;
          p1.y = y;
          
          
          if(pneed != 0)
          {
          	  prev.x = op[pneed-1][0];
          	  prev.y = op[pneed-1][1];
          	  line(img, prev, p1, Scalar(0, 0, 255));
          }
          else
          {
          		p2.x = x;
          		p2.y = y;
          	 	rectangle(img, p1, p2, Scalar(0, 0, 255));
          }
          imshow("original image", img);
          pneed++;
    }
    if (event ==  CV_EVENT_MOUSEMOVE)
    {
    	if(pneed == 0)
    		return;
    	Point prev;
    	prev.x = op[pneed-1][0];
        prev.y = op[pneed-1][1];
    	Point p2;
        p2.x = x;
        p2.y = y;
        Mat cpy = img.clone();
    	line(cpy, prev, p2, Scalar(0, 0, 255));
    	imshow("original image", cpy);
    }
}


int main()
{
	img = imread("/host/Video/UCSD_Anomaly_Dataset/UCSDped1/Train/Train001/001.tif");
	namedWindow("original image");
	int nl = img.rows*3;
	int nc = img.cols*3;
	resize(img, img, Size(nc, nl));
	imshow("original image", img);
	waitKey(0);
	//setMouseCallback("original image", on_mouse, 0);
	// while(pneed != 4)
	// {
	// 	imshow("original image", img);
	// 	waitKey(0);
	// }
	

	float k1[4][2] = {
		100, 160, 
		160, 160,
		160, 220,
		100, 220
	};
	float k2[4][2] = {
		125, 59, 
		222, 65, 
		145, 138,
		12, 128
	};
	
	//memcpy(k2, op, sizeof(float)*8);
	// Mat persM;
 //    Mat basePoint(1, 4, CV_32FC2, k1);
 //    Mat imagePoint(1, 4, CV_32FC2, k2);
	// persM = getPerspectiveTransform(imagePoint, basePoint);
	// Mat transImg;
	// warpPerspective(img, transImg, persM, Size(600, 400));

	// namedWindow("transform image");
	// imshow("transform image", transImg);
	// waitKey(0);
	return 0;
}