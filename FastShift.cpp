#include "FastShift.h"
#include "utils.h"
#include "MatrixDataIo.h"



/*int shiftTime;
vector<int> iters;
vector<float> convexTrend;*/


FastShift::FastShift(const Mat& feature, Mat& convexPoints)
{
	printf("start to meanshift point...\n");
	
	time_t startTime = time(0);
	data = feature;
	initial();
	printf("start to compute the convex point for every point ...\n");
	convexPoints.create(r, n, CV_32FC1);

	srand(time(0));
	vector<float> shiftPoint(n, 0);
	vector<float> shiftValue(n, 0);
	double w1, w2;
	vector<float> total_force, current_force, external_force;
	vector<int> grid_pos;
	int maxIterations = 50;

	
	for(int counts = 0; counts < r; counts ++)
	{
		printf("the %d-th iterations ...\n", counts);
		int iterations = maxIterations;
		copy((float*)data.row(counts).data, shiftPoint, n);
		while(iterations > 0)
		{
			iterations --;
			//choose the best direction
			radomSelect();
			whichGrid(shiftPoint, grid_pos);
			int pos = findposition(grid_pos);
			//so many cases to get the external forces
			#ifdef one_direction
				external_force = maxDirectionOneSide(shiftPoint, shiftValue);
			#else 
				#ifdef composite_line
					external_force = maxDirectionTwoSide(shiftPoint, shiftValue);
				#else 
					external_force = allDirections(shiftPoint, shiftValue);
				#endif
			#endif
			
			//shift the feature vector
			calForce(shiftPoint, pos, current_force);
			total_force = current_force + external_force;
			
			if(l2norm(external_force) < acceptance)
				continue;
			if(l2norm(total_force) < stopCond)
				break;
			shiftPoint = shiftPoint + step * total_force;
			/*w1 = l2norm(current_force)/l2norm(total_force);
			w2 = external_force/total_force;
			shiftPoint = w1 * shiftPoint + w2 * shiftValue;
			if(w2 < epsilon)
				break;*/
			
		}
		copy(shiftPoint, (float*)convexPoints.row(counts).data, n);
		//iters.push_back(counts);
		
	}
	string writer = "./output/convex.dat";
	MatrixDataIo mdi(writer.c_str(), true, convexPoints);
	
	printUsedTime(startTime);
	programPause();
	printf("exit meanshift clustering...\n");
}

void FastShift::partitionGrid()
{ 
	grids = new int[n];
	minValue = new int[n];
	
	for(int j = 0; j < n; j ++)
	{
		minValue[j] = data.at<float>(0, j);
		grids[j] = minValue[j];
	}
	for(int i = 1; i < r; i ++)
	{
		for(int j = 0; j < n; j ++)
		{
			float v = data.at<float>(i, j);
			if(v < minValue[j])
				minValue[j] = v;
			else if(v > grids[j])
				grids[j] = v;
		}
	}
	for(int j = 0; j < n; j ++)
		grids[j] = (grids[j] - minValue[j])/radius;
}


void FastShift::initial()
{
	r = data.rows;
	n = data.cols;
	
	partitionGrid();
	selectD = 2;
	selected.resize(selectD, 0);
	
	factor.resize(n, 1);
	for(int i = n - 2; i >= 0; i --)
		factor[i] *= grids[i+1];
	gridNum = factor[0] * grids[0];
	
	D = new int[gridNum];
	C = new float*[gridNum];
	for(int i = 0; i < gridNum; i ++)
		C[i] = new float[n];
	
	vector<int> grid_pos(n,  0);
	for(int i = 0; i < n; i ++)
	{
		whichGrid(data.row(i), grid_pos);
		int pos = findposition(grid_pos);
		D[pos] ++;
		add(C[pos], (float*)(data.row(i).data), n);
	}
	for(int i = 0; i < gridNum; i ++)
	{
		for(int j = 0; j < n; j ++)
		{
			C[i][j] /= D[i];
		}
	}
}


void FastShift::radomSelect()
{
	for(int i = 0; i < selectD; i ++)
		selected[i] = rand() % n;
}


void FastShift::whichGrid(vector<float> shiftPoint, vector<int> grid_pos)
{
	for(int i = 0; i < n; i ++)
	{
		grid_pos[i] = (shiftPoint[i] - minValue[i])/grids[i];
	}
}


int FastShift::findposition(vector<int>& grid_pos)
{
	int pos = 0;
	for(int j = 0; j < n; j ++)
	{
		pos +=  grid_pos[j] * factor[j];
	}
	return pos;
}

//core: D/(d[i] * delta)
//some problem
void FastShift::calForce(vector<float> shiftPoint, int pos, vector<float>& force)
{
	for(int i = 0; i < n; i ++)
	{
		force[i] = (shiftPoint[i] - C[pos][i]) * delta[pos];
		force[i] = D[pos] / force[i] ;
	}
}

vector<float> FastShift::maxDirectionOneSide(vector<float>  shiftPoint, vector<float>  shiftValue)
{
	vector<float>  external_force(n, 0);
	
	vector<int> grid_pos(n,  0);
	whichGrid(shiftPoint, grid_pos);
	int pos = findposition(grid_pos);
	
	vector<float>  force(n, 0);
	bool unset = true;
	float max;
	int shift;
	
	for(int i = 0; i < selectD; i ++)
	{
		for(int j = -1; j <= 1; j += 2)
		{
			calForce(shiftPoint, pos + j * factor[selected[i]], force);
			float l1 = l2norm(force);
			if(unset)
			{
				max = l1;
				external_force = force;
				shift = pos + j * factor[selected[i]];
				unset = false;
				continue;
			}
			if(l1 > max)
			{
				max = l1;
				external_force = force;
				shift = pos + j * factor[selected[i]];
			}
		}
	}
	//copy(C[shift], shiftValue, n);
	return external_force;
}


vector<float> FastShift::maxDirectionTwoSide(vector<float>  shiftPoint, vector<float>  shiftValue)
{
	vector<float>  external_force(n, 0);
	vector<float>  force(n, 0);
	vector<float>  force1(n, 0);
	bool unset = true;
	float max;
	int shift;
	
	vector<int> grid_pos(n,  0);
	whichGrid(shiftPoint, grid_pos);
	int pos = findposition(grid_pos);
	
	for(int i = 0; i < selectD; i ++)
	{
		for(int j = -1; j <= 1; j += 2)
		{
			calForce(shiftPoint, pos + j * factor[selected[i]], force1);
			force += force1;
		}
		float l = l2norm(force);
		if(unset)
		{
			max = l;
			external_force = force;
			shift = pos - factor[selected[i]];
			unset = false;
			continue;
		}
		if(l > max)
		{
			max = l;
			external_force = force;
		}
	}
	//copy(C[shift], shiftValue, n);
	return external_force;
}

vector<float> FastShift::allDirections(vector<float>  shiftPoint, vector<float>  shiftValue)
{
	vector<int> grid_pos(n,  0);
	whichGrid(shiftPoint, grid_pos);
	int pos = findposition(grid_pos);
	
	vector<float>  external_force(n, 0);
	vector<float>  force1(n, 0);
	for(int i = 0; i < selectD; i ++)
	{
		for(int j = -1; j <= 1; j += 2)
		{
			calForce(shiftPoint, pos + j * factor[selected[i]], force1);
			external_force += force1;
		}
	}
	return external_force;
}


