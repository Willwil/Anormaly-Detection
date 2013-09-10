#ifndef _LIAN_HIERARCHICAL
#define _LIAN_HIERARCHICAL

#include "globalInclude.h"

class Node
{
	public:
		int parent;
		int ancestor;//the root
		int nums;//the points in this subtree
};


class Hierarchical
{
	public:
		Hierarchical(string fileName);
		Hierarchical(const Mat& points, vector<int> nums, string fileName = "./output/hierar.model");
		~Hierarchical();
		double* collectScore(const Mat& indices);
		
	private:
		void initial(const Mat& points, vector<int> nums);
		bool stop(int minimal, double theta);
		double selectTwoMinimalCluster(int* si, int* sj);
		void unionCluster(int si, int sj);	
		void updateMinial(int si, int sj);
		void collect();
		void save(const char* fileName);
		void load(const char* fileName);
		void destroy();
		
	public:
		Node* forest; //2*N - 1;
		double** dist;
		int id;//node created, the top node can be placed
		int N;//the max possible node
		int psum;//the total points
		double theta;//stop threshod
		vector<int> ancestors;//
};


#endif
