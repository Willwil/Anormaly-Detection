#ifndef _LIAN_FAST_SHIFT
#define _LIAN_FAST_SHIFT

#include "globalInclude.h"

class FastShift
{
	public:
	FastShift(){};
	FastShift(const Mat& data, Mat& convexPoints);
	
	//can be setted by external
	public:
	double radius;//the cell length
	int selectD;   //shift optional direction nums supported 
	Mat data;
	double acceptance;
	double stopCond;
	float step;
	
	private:
	void partitionGrid();
	void initial();
	void radomSelect();
	void whichGrid(vector<float> shiftPoint, vector<int> grid_pos);
	int findposition(vector<int>& grid_pos);
	
	void calForce(vector<float> shiftPoint, int pos, vector<float>& force);
	
	vector<float> maxDirectionOneSide(vector<float>  shiftPoint, vector<float>  shiftValue);
	vector<float> maxDirectionTwoSide(vector<float>  shiftPoint, vector<float>  shiftValue);
	vector<float> allDirections(vector<float>  shiftPoint, vector<float>  shiftValue);
	
	//only used internally
	private:
	int* minValue; //the minist value of every dimension, size: nc
	int* grids;  //partition nums of every dimension, size: nc
	
	float** C; //the cell centroid, size: grid nums * data dimension
	int* D; //the cell density, size: grid nums, grids[0]*...*grids[nc -1]
	float* delta;

	vector<float> selected;
	int n;
	int r;
	
	vector<int> factor;//
	int gridNum;
	
};

#endif
