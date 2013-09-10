#include "Kmeans.h"
#include "utils.h"

Kmeans::Kmeans(string fileName)
{
	load(fileName.c_str());
}

Kmeans::Kmeans(const Mat& points, int k, string fileName)
{
	this->k = k;
	n = points.rows;
	m = points.cols;
	k_means(points);
	collect();
	updateMeasure();
	save(fileName.c_str());
}

void Kmeans::k_means(const Mat& feature)
{
	int try_again  =  5;
	TermCriteria criteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0);
	kmeans(feature, k, indices, criteria, try_again, KMEANS_PP_CENTERS, centers);
}

void Kmeans::collect()
{
	nums.resize(k, 0);
	for(int i = 0; i < n; i++)
		nums[indices.at<int>(i, 0)] ++;
	/*printf("nums: \n");
	for(int i = 0; i < k; i ++)
		printf("%d ", nums[i]);
	printf("\n");*/
}

//unfinished
void Kmeans::updateMeasure()
{
	/*cohency = new double[k];
	sepration = new double[k];
	for(int i = 0; i < n; i++)
	{
		for(int i = 0; i < n; i++)
		{
			int t = indices.at<int>(i, 0);
			double s = norm(centers.row(t) - centers.row(i));
		}
	}*/
}


void Kmeans::save(const char* fileName)
{
	ofstream ofs;
	ofs.open(fileName, ofstream::out);
	if(!ofs.is_open())
	{
		printf("error occured: can't open %s for write.\n", fileName);
        exit(1);
	}
	
	ofs << k << " " << m << endl;
	for(int i = 0; i < k; i ++)
	{
		for(int j = 0; j < m; j ++)
			ofs << centers.at<float>(i, j);
		ofs << endl;
	}
	for(int i = 0; i < k; i ++)
		ofs << nums[i] << " ";
	ofs << endl;
	ofs.close();
}

void Kmeans::load(const char* fileName)
{
	ifstream ifs(fileName, ifstream::in);
	if(!ifs.is_open())
	{
		printf("error occured: can't open %s for read.\n", fileName);
        exit(1);
	}
	
	ifs >> k >> m;
	centers.create(k, m, CV_32FC1);
	nums.resize(k, 0);
	for(int i = 0; i < k; i ++)
		for(int j = 0; j < m; j ++)
			ifs >> centers.at<float>(i, j);
	for(int i = 0; i < k; i ++)
		ifs >> nums[i];
	ifs.close();
}

double* Kmeans::collectScore()
{
	double* score = new double[k];
	for(int i = 0; i < k; i ++)
	{
		double s = nums[i] * 1.0 / n;
		score[i] = s;
	}
	double* allScore = new double[n];
	for(int i = 0; i < n; i ++)
		allScore[i] = score[indices.at<int>(i, 0)];
	delete[] score;
	return allScore;
}

double* Kmeans::collectScore(const Mat& data)
{
	int tn = data.rows;
	double* score = new double[tn];
	double* distance = new double[k];
	double totalScore = 0;
	double* weight = new double[k];
	/*int sum_nums = 0;
	for(int j = 0; j < k; j++)
		sum_nums += nums[j];*/
	for(int j = 0; j < k; j++)
		weight[j] = nums[j];
	for (int i = 0; i < tn; i++)
	{
		double sum = 0.0;
		for(int j = 0; j < k; j++)
		{
			distance[j] = norm(data.row(i) - centers.row(j));
			sum += distance[j];
		}
		score[i] = 0;
		for(int j = 0; j < k; j++)
			score[i] += weight[j]/(distance[j]*distance[j]);
		totalScore += score[i];
	}
	//covert to [0, 1]
	totalScore = totalScore/tn;
	for (int i = 0; i < tn; i++)
	{
		score[i] = score[i]/totalScore;
		//if(score[i] < 1)
			//printf("%d %lf\n", i, score[i]);
	}
		
	delete[] distance;
	return score;
}