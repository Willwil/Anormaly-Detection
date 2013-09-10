#ifndef GLOBALINCLUDE_H
#define GLOBALINCLUDE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
using namespace cv;

const int MaxStringLength = 500;
extern int GridRows, GridCols;
extern int frameWidth, frameHeight;

void printUsedTime(time_t startTime);
void programPause();
int countWords(const char* filename);
int countLines(const char* filename);

#endif // GLOBALINCLUDE_H
