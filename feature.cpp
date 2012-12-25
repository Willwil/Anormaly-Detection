#include "feature.h"
#include "featureIO.h"
#include <time.h>

VideoCapture vc;
vector<string> imgStrs;

//files: *.avi, *.mpg
void opticalFeature(Mat& feature, vector<string> files, string writer)
{
    time_t startTime = time(0);
    if(!feature.empty())
        feature.release();

    Mat frame;
    Mat current;    //the current frame
    Mat previous;   //the previous frame

    Mat v;
    Mat prevPts;
    Mat status;
    Mat err;

    int fileNumber = files.size();    // the number of videos
    bool directory = directoryAutoSet(files[0]);    //file or directory
    int readNumber = 0;  //record total number of images

    prevPts.create(1, FeatureRows*FeatureCols, CV_32FC2);
    setGrids(prevPts);
    v = Mat::zeros(1, FeatureRows*FeatureCols, CV_32FC2);

    for(int i = 0; i < fileNumber; i++)
    {
        int counts = 0;
        imgStrs.clear();
        if(!initialVideo(files[i], directory))
            continue;

        while(1)
        {
            if(!getFrame(counts, directory, frame))
                break;
			readNumber ++;
			 //printf("* ");
            printf("readNumber = %d\n", readNumber);
            counts ++;
            
            current = frame.clone();
            if(current.channels() > 1)
                cvtColor(current, current, CV_BGR2GRAY);
            //pyrDown(temp, current);
            if(counts == 1)
            {
                previous = current.clone();
                continue;
            }

            calcOpticalFlowPyrLK(previous, current, prevPts, v, status, err);
            current.copyTo(previous);
            //printf("tag\n");
            feature.push_back(v.reshape(1, 1));            
            
        }
	
        if(vc.isOpened())
            vc.release();
        writeFeatureToFile(feature.rowRange(feature.rows()-counts+1, feature.rows()), writer);
    }   
    
    if(feature.empty())
    {
        printf("optical feature extraction process may have some errors!\n");
        exit(1);
    }
    printf("readNumber: %d, feature rows: %d, feature cols: %d\n", readNumber, feature.rows, feature.cols);
    time_t endTime = time(0);
    int totalUsed = endTime - startTime;
    char usedTime[MaxStringLength];
    sprintf(usedTime, "%dh %dmin %dsec", totalUsed/3600, (totalUsed/60)%60, totalUsed%3600);
    printf("feature extration precess used %s time\n", usedTime);
}


void socialForceFeature(Mat& feature, vector<string> files, string writer)
{
    time_t startTime = time(0);
    if(!feature.empty())
        feature.release();
    bool directory = directoryAutoSet(files[0]);    //file or directory
    int fileNumber = files.size();    // the number of videos
    int readNumber = 0;  //record total number of images

    Mat frame;
    Mat current;
    Mat previous;
    Mat temp;

    Mat pV;     //previous v
    Mat cV;     //current v
    Mat aV;   //average velocity
    Mat f;
    float tao = 1.0/2;

    Mat prevPts;
    Mat curPts;
    Mat status;
    Mat err;

    prevPts.create(1, FeatureRows*FeatureCols, CV_32FC2);
    setGrids(prevPts);
    curPts = Mat::zeros(1, FeatureRows*FeatureCols, CV_32FC2);
    cV.create(1, FeatureRows*FeatureCols, CV_32FC2);
    aV.create(1, FeatureRows*FeatureCols, CV_32FC2);


    for(int i = 0; i < fileNumber; i++)
    {
        int countss = 0;
        imgStrs.clear();
        if(!initialVideo(files[i], directory))
        {
            printf("!!!!!!!!!!!\n");
            continue;
        }

        while(1)
        {
            if(!getFrame(countss, directory, frame))
                break;
            readNumber ++;
            countss ++;
            printf("* ");
            //printf("readNumber = %d\n", readNumber);
            current = frame.clone();
            if(temp.channels() > 1)
                cvtColor(current, current, CV_BGR2GRAY);
            //pyrDown(temp, current);

            if(countss == 1)
            {
                //printf("tag");
                previous = current.clone();
                continue;
            }

            calcOpticalFlowPyrLK(previous, current, prevPts, curPts, status, err);
            current.copyTo(previous);
            cV = curPts - prevPts;
            if(countss == 2)
            {
                pV = cV.clone();
                continue;
            }

            getAverageMat(cV, aV);

            f = tao*(aV - cV) + (pV - cV);
            feature.push_back(f.reshape(1, 1));
            cV.copyTo(pV);
        }

        if(vc.isOpened())
            vc.release();
        writeFeatureToFile(feature.rowRange(feature.rows()-countss+2, feature.rows()), writer);
    }
    if(feature.empty())
    {
        printf("social force feature extraction process may have some errors!\n");
        exit(1);
    }
    printf("readNumber: %d, feature rows: %d, feature cols: %d\n", readNumber, feature.rows, feature.cols);

    time_t endTime = time(0);
    int totalUsed = endTime - startTime;
    char usedTime[MaxStringLength];
    sprintf(usedTime, "%dh %dmin %dsec", totalUsed/3600, (totalUsed/60)%60, totalUsed%60);
    printf("feature extration precess used %s time\n", usedTime);
}


void gradientHistomFeature(Mat& feature, vector<string> files, string writer)
{
    time_t startTime = time(0);
    if(!feature.empty())
        feature.release();
    bool directory = directoryAutoSet(files[0]);    //file or directory
    int fileNumber = files.size();    // the number of videos
    int readNumber = 0;  //record total number of images

    Mat frame;
    Mat current;
    Mat previous;
    Mat temp;
    Mat f;

    for(int i = 0; i < fileNumber; i++)
    {
        int countss = 0;
        imgStrs.clear();
        if(!initialVideo(files[i], directory))
        {
            printf("!!!!!!!!!!!\n");
            continue;
        }
        while(1)
        {
            if(!getFrame(countss, directory, frame))
                break;
            countss ++;
            readNumber ++;
            //printf("* ");
            printf("readNumber = %d\n", readNumber);
            temp = frame.clone();
            if(temp.channels() > 1)
                cvtColor(temp, temp, CV_BGR2GRAY);
            //pyrDown(temp, current);
            if(readNumber == 1)
            {
                previous = current.clone();
                continue;
            }
            int nl = current.rows;
            int nc = current.cols;
            if(!f.empty())
            {
                if(nl != f.rows || nc != f.cols || f.type() != CV_32FC3)
                    f.release();
            }
            if(f.empty())
                f.create(nl, nc, CV_32SC3);
            for(int i = 0; i < nl; i ++)
            {
                int tt = 0;
                float* data = f.ptr<float>(i);
                uchar* cdata = current.ptr<uchar>(i);
                uchar* pdata = current.ptr<uchar>(i-1);
                uchar* ppdata = previous.ptr<uchar>(i);
                for(int j = 0; j < nc; j ++)
                {
                    data[tt] = ((j > 0)? (cdata[j] -cdata[j-1]): 0);
                    tt ++;
                    data[tt] = ((i > 0)? (cdata[j] - pdata[j]): 0);
                    tt ++;
                    data[tt] = cdata[j] - ppdata[j];
                    tt ++;
                }
            }
            current.copyTo(previous);
            feature.push_back(f.reshape(1, 1));
        }
        
        if(vc.isOpened())
            vc.release();
        writeFeatureToFile(feature.rowRange(feature.rows()-countss, feature.rows()), writer);
    }
    
    time_t endTime = time(0);
    int totalUsed = endTime - startTime;
    char usedTime[MaxStringLength];
    sprintf(usedTime, "%dh %dmin %dsec", totalUsed/3600, (totalUsed/60)%60, totalUsed%3600);
    printf("feature extration precess used %s time\n", usedTime);
}


void myNewFeature(Mat& feature, vector<string> files, string writer)
{

}

void gridImage(const Mat& src, Mat& dest, int rows, int cols)
{
    if(src.empty())
    {
        printf("src image error!\n");
        exit(1);
    }
    /*Mat dest1;
    dest1.create(rows, cols, src.type());*/
    //printf("type: %d, float: %d\n", src.elemSize1(), sizeof(float));
    if(!dest.empty())
    {
        if(src.rows != rows || src.cols != cols)
        {
            dest.release();
            dest.create(rows, cols, src.type());
        }
    }
    else
    {
        dest.create(rows, cols, src.type());
    }
    int sx = src.rows / rows;
    int sy = src.cols / cols;
    int channels = src.channels();
    for(int i = 0; i < rows; i ++)
    {
        float* data = dest.ptr<float>(i);
        const float* data2 = src.ptr<float>(i * sx + sx/2);
        //int t = i * sx + sx/2;
        int m = sy/2;
        for(int j = 0; j < cols; j ++)
        {
            for(int k = 0; k < channels; k ++)
            {
                data[j*channels + k] = data2[(m+sy*j)*channels + k];
            }
        }
        //printf("\n");
    }
    //printf("tag\n");
}


void setGrids(Mat& pts)
{
    if(pts.empty())
        pts.create(FeatureRows, FeatureCols, CV_32FC2);
    if(pts.channels() != 2)
        return;
    int nl = pts.rows;
    int nc = pts.cols;
    int celx = frameWidth/nc;
    int cely = frameWidth/nc;
    for(int i = 0; i < nl; i ++)
    {
        uchar* data = pts.ptr<uchar>(i);
        for(int j = 0; j < nc*2; j ++)
        {
            data[2*j+1] = celx * j;
            data[2*j] = cely * i;
        }
    }
}

void getImageStrings(string foldName, vector<string>& imgStrs)
{
    DIR* dir = 0;
    struct dirent* s_dir = 0;
    struct stat st;
    char path[MaxStringLength] = "";

    dir = opendir(foldName.c_str());
    if(dir == NULL)
        return;

    while(s_dir = readdir(dir))
    {
        if( strcmp(s_dir->d_name, ".")==0 || strcmp(s_dir->d_name, "..")==0)
            continue;
        //printf("d_name: %s\n", s_dir->d_name);
        if(s_dir->d_name[0] == '.')
            continue;
        stat(s_dir->d_name, &st);
        if(S_ISDIR(st.st_mode))
            continue;
        if(!isImageFormat(s_dir->d_name))
            continue;
        sprintf(path, "%s/%s", foldName.c_str(), s_dir->d_name);
        imgStrs.push_back(path);
    }
    if(dir)
        closedir(dir);
}


bool initialVideo(string folder, bool directory)
{
    //printf("folder: %s\n", folder.c_str());
    if(directory)
    {
        getImageStrings(folder, imgStrs);
        if(imgStrs.size() == 0)
            return false;
    }
    else
    {
        vc.open(folder);
        if(!vc.isOpened())
        {
            printf("error occures: video file can't open!\n");
            return false;
        }
    }
    return true;
}


bool getFrame(int counts, bool directory, Mat& frame)
{
    if(directory)
    {
        if(counts >= imgStrs.size())
            return false;
        //printf("path: %s\n", path);
        frame = imread(imgStrs[counts]);
        if(frame.empty())
        {
            printf("??????????????????\n");
            return false;
        }
    }
    else
    {
        if(!vc.read(frame))
            return false;
    }
    return true;
}

void getAverageMat(const Mat& in, Mat& out)
{
    if(in.empty())
        exit(1);
    if(in.total() != in.checkVector(2, CV_32F, true))
        exit(1);
    if(out.empty())
        out.create(1, in.cols, CV_32FC2);

    int tempNumber;
    float ebsuleng = frameWidth*frameHeight* 1.0 / (FeatureRows*FeatureCols);
    const float* in_data = in.ptr<float>(0);
    float* out_data = out.ptr<float>(0);

    for(int j = 0;  j < in.cols; j ++)
    {
        out_data[2*j] = in_data[2*j];
        out_data[2*j+1] = in_data[2*j+1];
        tempNumber = 1;
        for(int k = 0;  k < in.cols; k ++)
        {
            double s = (in_data[2*j] - in_data[2*k]) * (in_data[2*j] - in_data[2*k]);
            if(s > ebsuleng)
                continue;
            s += (in_data[2*j+1] - in_data[2*k+1]) * (in_data[2*j+1] - in_data[2*k+1]);

            if(s <= ebsuleng)
            {
                out_data[2*j] += in_data[2*k];
                out_data[2*j+1] += in_data[2*k+1];
                tempNumber ++;
            }
        }

        if(tempNumber > 0)
        {
            out_data[2*j] /= tempNumber;
            out_data[2*j+1] /= tempNumber;
        }
    }
}


/*void socialForceFeature(vector<string> files, Mat& feature)
{
    time_t startTime = time(0);
    if(!feature.empty())
        feature.release();
    bool directory = directoryAutoSet(files[0]);    //file or directory
    int fileNumber = files.size();    // the number of videos
    int readNumber = 0;  //record total number of images

    Mat frame;
    Mat current;
    Mat previous;
    Mat temp;
    Mat pV;     //previous v
    Mat cV;     //current v
    Mat aV;   //average velocity
    Mat f;
    double tao = 1.0/2;
    int avRadius = 1;

    Mat prevPts;
    Mat curPts;
    Mat status;
    Mat err;
    prevPts.create(1, FeatureRows*FeatureCols, CV_32FC2);
    setGrids(prevPts);
    curPts = Mat::zeros(1, FeatureRows*FeatureCols, CV_32FC2);


    for(int i = 0; i < fileNumber; i++)
    {
        VideoCapture vc;
        DIR* dir = 0;
        struct dirent* s_dir = 0;
        struct stat st;
        char path[MaxStringLength] = "";
        if(directory)
        {
            dir = opendir(files[i].c_str());
        }
        else
        {
            vc.open(files[i]);
            if(!vc.isOpened())
            {
                printf("error occures: video file can't open!\n");
                exit(1);
            }
            int frameCount = vc.get(CV_CAP_PROP_FRAME_COUNT);
            if(!feature.empty())
                feature.reserve(readNumber+frameCount);
        }
        while(1)
        {
            if(directory)
            {
                if((s_dir = readdir(dir)) == 0)
                    break;
                if( strcmp(s_dir->d_name, ".")==0 || strcmp(s_dir->d_name, "..")==0)
                    continue;
                //printf("d_name: %s\n", s_dir->d_name);
                if(s_dir->d_name[0] == '.')
                    continue;
                stat(s_dir->d_name, &st);
                if(S_ISDIR(st.st_mode))
                    continue;
                if(!isImageFormat(s_dir->d_name))
                    continue;
                sprintf(path, "%s/%s", files[i].c_str(), s_dir->d_name);
                frame = imread(path);
                if(frame.empty())
                {
                    printf("%s??????\n", path);
                    exit(1);
                }
            }
            else
            {
                if(!vc.read(frame))
                    break;
            }
            readNumber ++;
            //printf("* ");
            printf("readNumber = %d\n", readNumber);
            temp = frame.clone();
            if(temp.channels() > 1)
                cvtColor(temp, temp, CV_BGR2GRAY);
            pyrDown(temp, current);
            if(readNumber == 1)
            {
                previous = current.clone();
                continue;
            }

            calcOpticalFlowPyrLK(previous, current, prevPts, curPts, status, err);
            current.copyTo(previous);
            if(readNumber == 2)
            {
                pV = cV.clone();
                continue;
            }
            if(aV.empty())
            {
                aV.create(cV.rows, cV.cols, cV.type());
            }
            //printf("tags\n");
            int top, down, left, right;
            for(int i = 0; i < cV.rows; i ++)
            {
                float* a_data = aV.ptr<float>(i);
                for(int j = 0;  j < cV.cols; j ++)
                {
                    a_data[j] = 0;
                    left = j - avRadius;
                    right = j + avRadius;
                    top = i - avRadius;
                    down = i + avRadius;
                    if(left < 0)
                    {
                        left = 0;
                    }
                    if(right >= cV.cols)
                    {
                        right = cV.cols-1;
                    }
                    if(top < 0)
                    {
                        top = 0;
                    }
                    if(down >= cV.rows)
                    {
                        down = cV.rows-1;
                    }
                    //printf("left:%d, right:%d, top:%d, down:%d\n", left, right, top, down);
                    //printf("rows:%d, cols:%d\n", cV.rows, cV.cols);
                    Mat tt = cV.rowRange(top, down+1).colRange(left, right+1);
                    //printf("tt cols:%d\n",tt.cols);
                    for(int k = 0; k < tt.rows; k ++)
                    {
                        for(int t = 0; t < tt.cols; t ++)
                        {
                             for(int m = 0; m < tt.channels(); m ++)
                                a_data[j*tt.channels() + m] += tt.at<Vec2f>(k, t)[m];
                        }
                    }
                    for(int m = 0; m < tt.channels(); m ++)
                        a_data[j*tt.channels() + m] /= (tt.rows*tt.cols);
                }
            }

            f = tao*(aV - cV) + (pV - cV);
            feature.push_back(f.reshape(1, 1));
            cV.copyTo(pV);
        }
    }
    time_t endTime = time(0);
    int totalUsed = endTime - startTime;
    char usedTime[MaxStringLength];
    sprintf(usedTime, "%dh %dmin %dsec", totalUsed/3600, (totalUsed/60)%60, totalUsed%60);
    printf("feature extration precess used %s time\n", usedTime);
}*/


