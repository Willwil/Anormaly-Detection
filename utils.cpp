#include "utils.h"


bool contains(Mat parent, Mat child)
{
	
	return true;
}


void printMatInfo(const Mat& m)
{
	int nl = m.rows;
	int nc = m.cols;
	for(int i = 0; i < nl; i++)
	{
		for(int j = 0; j < nc; j++)
		{
			int t = m.at<int>(i, j);
			if(t != 0)
				printf("%d ", t);
		}
		if((i * nc + 1) % 100 == 0)
			printf("\n");
	}
}

void minArray(const Mat& m, float& min, pair<int, int>& s)
{
	min = m.at<float>(0, 0);
	int nl = m.rows;
	int nc = m.cols;
	for(int i = 0; i < nl; i++)
	{
		for(int j = 0; j < nc; j++)
		{
			if(min > m.at<float>(i, j))
			{
				min = m.at<float>(i, j);
				s.first = i;
				s.second = j;
			}
		}
	}
}

void maxArray(const Mat& m, float& max, pair<int, int>& s)
{
	max = m.at<float>(0, 0);
	int nl = m.rows;
	int nc = m.cols;
	for(int i = 0; i < nl; i++)
	{
		for(int j = 0; j < nc; j++)
		{
			if(max < m.at<float>(i, j))
			{
				max = m.at<float>(i, j);
				s.first = i;
				s.second = j;
			}
		}
	}
}


float meanArray(const Mat& m)
{
	float mean = 0;
	int nl = m.rows;
	int nc = m.cols;
	for(int i = 0; i < nl; i++)
	{
		for(int j = 0; j < nc; j++)
		{
			mean += m.at<float>(i, j);
		}
	}
	mean = mean/(nl*nc);
	return mean;
}


float l2norm(const vector<float>& v)
{
	float s = 0.0;
	int n = v.size();
	for(int i = 0;  i < n; i++)
	{
		s += v[i] * v[i];
	}
	
	return s;
}


float l2norm(const vector<float>& v1, const vector<float>& v2)
{
	float ret = 0;
	int n = v1.size();
	if(n != v2.size())
		exit(1);
	for(int i = 0;  i < n; i++)	
		ret += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	return ret;
}

float l2norm(const Mat& z, int i, int j)
{
	float ret = 0.0;
	int nc = z.cols;
	if(i >= z.rows || j >= z.rows)
		exit(1);
	for(int k = 0; k < nc; k ++)
		ret += (z.at<float>(i, k) - z.at<float>(j, k)) * (z.at<float>(i, k) - z.at<float>(j, k));
	return ret;
}

float l2norm(const vector<float>& v, const Mat& m)
{
	float ret;
	if(m.rows > 1 || v.size() != m.cols)
		exit(1);
	int n = v.size();
	float* m_data = (float*)m.data;
	for(int i = 0;  i < n; i ++)	
		ret += (v[i] - m_data[i]) * (v[i] - m_data[i]);	
	return ret;
}

float* l2norm(const Mat& z, bool DATA_AS_ROW)
{
	int nl = z.rows;
	int nc = z.cols;
	float* ret = new float[nl];
	for (int i = 0; i < nl; ++i)
	{
		ret[i] = 0;
		for(int k = 0; k < nc; k++)
			ret[i] += z.at<float>(i, k) * z.at<float>(i, k);
	}
	return ret;
}


int find(string s, char c)
{
	int ret = 0;
	
	trim(s, c);
	int n = s.size();
	for(int i = 0; i < n; i++)
		if(s[i] == c)
			ret ++;
	return ret;
}

string trim(string& s, char c)
{
	int eraseN1 = 0;
	int eraseN2 = 0;
	
	int n = s.size();
	for(int i = 0; i < n; i++, eraseN1++)
		if(s[i] != c)
			break;
	s.erase(0, eraseN1);
			
	for(int i = n - 1; i > 0; i--, eraseN2++)
		if(s[i] != c)
			break;
	s.erase(n-eraseN2, eraseN2);
	return s;	
}

void split(string s, char c, vector<string>& v)
{
	if(!v.empty())
		v.clear();
	
	int f;
	while((f = s.find(c)) != string::npos)
	{
		v.push_back(s.substr(0, f));
		s.erase(0, f+1);
	}
}

string replace(string&s, char newc, char oldc)
{
	int n = s.size();
	for(int i = 0; i < n; i++)
		if(s[i] == oldc)
			s[i] = newc;
	return s;
}


int areaFromIntegral(const Mat& sum, int x1, int y1, int x2, int y2)
{
	assert(x2 >= x1 && y2 >= y1);
	int area = sum.at<uchar>(x2, y2) + sum.at<uchar>(x1, y1) - sum.at<uchar>(x1, y2) - sum.at<uchar>(x2, y1);
	return area;
}
