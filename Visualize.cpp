#include "Visualize.h"
#include "utils.h"

void Visualize::visualSorcialForce(const Mat& img, Mat pts, Mat f)
{
	if(pts.rows > 1)	
		exit(1);
	if(f.rows > 1 || f.cols != pts.cols)	
		exit(1);
	int h = img.rows;
	int w = img.cols;
	int points = pts.cols;

    Mat copyMat = Mat::zeros(h, w, CV_8UC3);
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            int temp = img.at<uchar>(i, j);
            //printf("%d\n", temp);
            copyMat.at<Vec3b>(i, j)[0] = temp;
            copyMat.at<Vec3b>(i, j)[1] = temp;
            copyMat.at<Vec3b>(i, j)[2] = temp;
        }
    }
    Scalar color(0, 255, 255);
    for (int i = 0; i < points; ++i)
    {
        Point p1(pts.at<Vec2f>(0, i)[0], pts.at<Vec2f>(0, i)[1]);
        int chenzi = 5;
        //Point p2(p1.x+0.6, p1.y+0.6);
        Point p2(p1.x + chenzi* f.at<Vec2f>(0, i)[0], p1.y+ chenzi * f.at<Vec2f>(0, i)[1]);
        line(copyMat, p1, p2, color, 1);
    }
	//printf("\n");
	namedWindow("force image");
	imshow("force image", copyMat);
	waitKey(0);
	//programPause();
}



void Visualize::visualTrajectory(const Mat& img, Mat pts)
{
    if(pts.rows > 1)    
        exit(1);
    int h = img.rows;
    int w = img.cols;
    int points = pts.cols;
    Mat copyMat = Mat::zeros(h, w, CV_8UC3);
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            int temp = img.at<uchar>(i, j);
            //printf("%d\n", temp);
            copyMat.at<Vec3b>(i, j)[0] = temp;
            copyMat.at<Vec3b>(i, j)[1] = temp;
            copyMat.at<Vec3b>(i, j)[2] = temp;
        }
    }
    Scalar color(0, 255, 255);
    for(int i = 0; i < points; i ++)
    {
        Point p1(pts.at<Vec2f>(0, i)[0], pts.at<Vec2f>(0, i)[1]);
        Point p2(p1.x+1, p1.y+1);
        rectangle(copyMat, p1, p2, color, 1);
    }
    //printf("\n");
    namedWindow("particle image");
    imshow("particle image", copyMat);
    waitKey(0);
}

//display [0, 1] range
void Visualize::visualPointsLine(const double* x, const double* y, int n)
{
    int width = 400;
    int height = 400;

    //double minx = *min_element(x, x+n);
    //double miny = *min_element(y, y+n);
    //double rangex = *max_element(x, x+n) - minx;
    //double rangey = *max_element(y, y+n) - miny;
    double* xx = new double[n];
    double* yy = new double[n];
    for (int i = 0; i < n; ++i)
    {
        //xx[i] = (x[i] - minx)/rangex * width;
        //yy[i] = (y[i] - miny)/rangey * height;
        xx[i] = x[i] * width;
        yy[i] = height - y[i] * height;
    }
    
    Mat img = Mat::zeros(height, width, CV_32FC3);
    Scalar color(0, 255, 255); 
    for (int i = 1; i < n; ++i)
    {
        Point p1(xx[i-1], yy[i-1]);
        Point p2(xx[i], yy[i]);
        line(img, p1, p2, color, 1);
    }
    Point p1(0, height);
    Point p2(width, 0);
    line(img, p1, p2, color, 1);

    namedWindow("roc curve");
    imshow("roc curve", img);
    waitKey(0);
}

//unfinished
void Visualize::visualFeature(const Mat& feature, int width, int height, int delay)
{
    int nl = feature.rows;
    int nc = feature.cols;
    Mat img = Mat::zeros(height, width, CV_32FC1);

    Scalar color(0, 255, 255); 
    namedWindow("feature plot");
    for (int i = 0; i < nl; ++i)
    {
        img = Scalar(0);
        for (int j = 1; j < nc; ++j)
        {
            int chenzi = 5;
            Point p1((j-1)*chenzi, feature.at<float>(i, j-1));
            Point p2(j*chenzi, feature.at<float>(i, j));
            line(img, p1, p2, color, 1);
        }
        imshow("feature plot", img);
        waitKey(delay);
    } 
}

//unfinished
void showDensity(const Mat& densityData, int imgWidth, int imgHeight)
{
    if(densityData.empty())
        return;
    if(densityData.cols != 2)
        return;
    int nl = densityData.rows;

    float min ,max, range;
    Mat normData;
    for(int j = 0;  j < 2; j++)
    {
        pair<int, int> s;
        minArray(densityData.col(j), min, s);
        maxArray(densityData.col(j), max, s);
        range = max - min;
        double alpha = imgHeight/range;
        double beta = -alpha * min;
        //problem
        densityData.col(j).convertTo(normData, CV_32F, alpha, beta);
    }
    
    /**************************/
    Mat showMat;
    for(int i = 0;  i < nl; i ++)
    {
        int xp, yp;
        if(xp >= imgHeight || yp >= imgWidth || xp < 0 || yp < 0)
        {
            printf("xp:%d, yp: %d\n", xp, yp);
            return;
        }
        if(showMat.at<uchar>(xp, yp) > 0)
        {
            showMat.at<uchar>(xp, yp)++;
        }
    }
    namedWindow("density plot");
    imshow("density plot", showMat);
    waitKey(0);
}


