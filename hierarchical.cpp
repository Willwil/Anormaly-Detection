#include "hierarchical.h"
#include "utils.h"
#include <stack>
#include <map>

#define UNREACHABLE -1
#define UNKNOWN -2

Hierarchical::Hierarchical(string fileName)
{
	load(fileName.c_str());
}


Hierarchical::Hierarchical(const Mat& points, vector<int> nums, string fileName)
{
	time_t startTime = time(0);
	
	initial(points, nums);
	
	while(1)
	{
		int si, sj;
		double minimal = selectTwoMinimalCluster(&si, &sj);
		printf("minimal: %f, theta: %f\n", minimal, theta);
		if(stop(minimal, theta))
			break;
		//printf("tag tag tag\n");
		unionCluster(si, sj);
		updateMinial(si, sj);
	}
	
	collect();
	printUsedTime(startTime);
	save(fileName.c_str());
}


Hierarchical::~Hierarchical()
{
	delete[] forest;
	int s = 2*N - 1;
	for(int i = 0; i < s; i ++)
		delete[] dist[i];
	delete[] dist;
}


void Hierarchical::initial(const Mat& points, vector<int> nums)
{
	int n = points.rows;
	N = n;
	int s = 2*N - 1;
	forest = new Node[s];
	dist = new double*[s];
	psum = 0;
	for(int i = 0; i < s; i ++)
	{
		forest[i].parent = 0;
		forest[i].ancestor = UNKNOWN;
		dist[i] = new double[s];
		for(int j = 0; j < s; j ++)
			dist[i][j] = UNREACHABLE;
	}
	
	for(int i = 0; i < n; i ++)
	{
		forest[i].nums = nums[i];
		psum += nums[i];
		for(int j = 0; j < i; j ++)
			dist[i][j] = l2norm(points, i, j);
			
	}
	for(int i = 0; i < n; i ++)
	{
		dist[i][i] = 0;
		for(int j = i+1; j < n; j ++)
			dist[i][j] = dist[j][i];
	}
	id = n;
	//printf("parent: %d\n", forest[0].parent);
	////////////////////////////////////
	//import config
	//the max distance value of the the neighbor to be 
	theta = points.cols;
}


bool Hierarchical::stop(int minimal, double theta)
{
	if(minimal > theta || minimal < 0)
		return true;
	return false;
}

double Hierarchical::selectTwoMinimalCluster(int* si, int* sj)
{
	double minimal = -1;
	bool breakFlag = false;
	for(int i = 0; i < id && !breakFlag; i++)
	{
		for(int k = 0; k < i && !breakFlag; k++)
		{
			if(forest[i].parent || forest[k].parent)
				continue;

			minimal = dist[i][k];
			*si = i;
			*sj = k;
			breakFlag = true;
			//printf("1: %f ", minimal);
		}
	}
	if(!breakFlag)
		return -1;
	
	for(int i = *si; i < id; i++)
	{
		for(int k = 0; k < i; k++)
		{
			if(forest[i].parent || forest[k].parent)
				continue;
			if(dist[i][k] < minimal)
			{
				minimal = dist[i][k];
				*si = i;
				*sj = k;
				printf("1: %f %d %d\n", minimal, i, k);
			}
		}
	}
	return minimal;
}


void Hierarchical::unionCluster(int si, int sj)
{
	forest[si].parent = forest[sj].parent = id;
	forest[id].nums = forest[si].nums + forest[sj].nums;
	id++;
}

void Hierarchical::updateMinial(int si, int sj)
{
	for(int k = 0; k < id; k ++)
	{
		if(forest[k].parent)
			continue;
		dist[id-1][k] = min(dist[si][k], dist[sj][k]);
		dist[k][id-1] = dist[id-1][k];
	}
}

void Hierarchical::collect()
{
	//printf("id: %d\n", id);
	for(int i = 0; i < id; i++)
	{
		//printf("%d\n", forest[i].parent);
		if(forest[i].parent == 0)
		{
			//printf("a: %d\n", i);
			ancestors.push_back(i);
			forest[i].ancestor = i;
		}
	}
	for(int i = 0; i < N; i ++)
	{
		int parent = forest[i].parent;
		int ancestor;
		std::stack<int> visit;
		visit.push(i);
		while(1)
		{
			if(parent && forest[parent].ancestor == UNKNOWN)
			{
				visit.push(parent);
				parent = forest[parent].parent;
			}
			else
			{
				if(parent)
					ancestor = forest[parent].ancestor;
				else
				{
					ancestor = visit.top();
					visit.pop();
					forest[ancestor].ancestor = ancestor;
				}
				break;
			}
		}
		while(!visit.empty())
		{
			int e = visit.top();
			visit.pop();
			forest[e].ancestor = ancestor;
		}
	}
}

void Hierarchical::save(const char* fileName)
{		
	ofstream ofs;
	ofs.open(fileName, ofstream::out);
	if(!ofs.is_open())
	{
		printf("error occured: can't open %s for write.\n", fileName);
        exit(1);
	}
	
	ofs << theta << endl;
	ofs << id << endl;
	ofs << N << endl;
	for(int i = 0; i < id; i ++)
	{
		ofs << forest[i].parent << forest[i].ancestor << forest[i].nums;
	}
	ofs << endl;
	for(int i = 0; i < id; i ++)
	{
		for(int j = 0; j < id; j ++)
		{
			ofs << dist[i][j];
		}
	}
	ofs << endl;
	
	ofs.close();
}


void Hierarchical::load(const char* fileName)
{
	ifstream ifs(fileName, ifstream::in);
	if(!ifs.is_open())
	{
		printf("error occured: can't open %s for read.\n", fileName);
        exit(1);
	}
	
	ifs >> theta;
	ifs >> id;
	ifs >> N;
	forest = new Node[id];
	for(int i = 0; i < id; i ++)
	{
		ifs >> forest[i].parent >> forest[i].ancestor >> forest[i].nums;
	}
	dist = new double*[id];
	for(int i = 0; i < id; i ++)
	{
		dist[i] = new double[id];
		for(int j = 0; j < id; j ++)
		{
			ifs >> dist[i][j];
		}
	}
	
	ifs.close();
}

double* Hierarchical::collectScore(const Mat& indices)
{
	int anc = ancestors.size();
	map<int, double> score;
	//printf("score: \n");
	for(int i = 0; i < anc; i ++)
	{
		int t = ancestors[i];
		double s = forest[t].nums * 1.0 / psum;
		//if(s > 1)
			//printf("%f %d %d\n", s, N, forest[t].nums);
		score.insert(pair<int, double>(t, s));
	}
	//printf("\n");
	//programPause();
	int points = indices.rows;
	double* allScore = new double[points];
	for(int i = 0; i < points; i ++)
	{
		int t = indices.at<int>(i, 0);
		int ancestor = forest[t].ancestor;
		allScore[i] = score[ancestor];
	}
	return allScore;
}
