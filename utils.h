#ifndef MY_UTILS_H
#define MY_UTILS_H

#include "globalInclude.h"

template <typename T>
vector<T> operator +(vector<T> v1, vector<T> v2);
template <typename T>
vector<T> operator *(vector<T> v, T f);
template <typename T>
vector<T> operator *(T f, vector<T> v);
template <typename T>
vector<T> operator +=(vector<T> v1, vector<T> v2);

template <typename T>
void copy(T* c, vector<T> v, int n);
template <typename T>
void copy(vector<T> v, T* c, int n);
template <typename T>
void add(T* c1, T* c2, int n);


bool contains(Mat parent, Mat child);
void printMatInfo(const Mat& m);
int areaFromIntegral(const Mat& sum, int x1, int y1, int x2, int y2);

void minArray(const Mat& m, float& min, pair<int, int>& s);
void maxArray(const Mat& m, float& max, pair<int, int>& s);
float meanArray(const Mat& m);

float l2norm(const vector<float>& v);
float l2norm(const vector<float>& v1, const vector<float>& v2);
float l2norm(const Mat& z, int i, int j);
float l2norm(const vector<float>& v, const Mat& m);
float* l2norm(const Mat& z, bool DATA_AS_ROW=true);

int find(string s, char c);
string trim(string& s, char c);
void split(string s, char c, vector<string>& v);
string replace(string&s, char newc, char oldc);

void programPause();
void printUsedTime(time_t startTime);


template <typename T>
vector<T> operator +(vector<T> v1, vector<T> v2)
{
	int n = v1.size();
	if(n != v2.size())
		exit(1);
	vector<T> v3(n, 0);
	for(int i = 0;  i < n; i ++)
	{
		v3[i] = v1[i] + v2[i];
	}
	return v3;
}

template <typename T>
vector<T> operator *(vector<T> v, T f)
{
	int n = v.size();
	vector<T> v3(n, 0);
	for(int i = 0;  i < n; i ++)
	{
		v3[i] = v[i] * f;
	}
	return v3;
}


template <typename T>
vector<T> operator *(T f, vector<T> v)
{
	int n = v.size();
	vector<T> v3(n, 0);
	for(int i = 0;  i < n; i ++)
	{
		v3[i] = v[i] * f;
	}
	return v3;
}


template <typename T>
vector<T> operator +=(vector<T> v1, vector<T> v2)
{
	int n = v1.size();
	if(n != v2.size())
		exit(1);
	for(int i = 0;  i < n; i ++)
	{
		v1[i] = v1[i] + v2[i];
	}
	return v1;
}

//v = c;
//c: src, v: dest
template <typename T>
void copy(T* c, vector<T> v, int n)
{
	int vn = v.size();
	int m = min(n, vn);
	for(int i = 0; i < m; i ++)
	{
		v[i] = c[i];
	}
	for(int i = m; i < n; i ++)
	{
		v.push_back(c[i]);
	}
}

//c = v;
//c: src, v: dest
template <typename T>
void copy(vector<T> v, T* c, int n)
{
	for(int i = 0; i < n; i ++)
	{
		c[i] = v[i];
	}
}

template <typename T>
void add(T* c1, T* c2, int n)
{
	for(int i = 0; i < n; i ++)
	{
		c1[i] += c2[i];
	}
}

template <typename T>
T min(T* v, int n, int& item = 0)
{
	T ret = v[0];
	item = 0;
	for(int i = 1; i < n; i ++)
	{
		if(v[i] < ret)
		{
			ret = v[i];
			item = i;
		}
	}
	return ret;
}


template <typename T>
T max(T* v, int n, int& item = 0)
{
	T ret = v[0];
	item = 0;
	for(int i = 1; i < n; i ++)
	{
		if(v[i] > ret)
		{
			ret = v[i];
			item = i;
		}
	}
	return ret;
}

template <typename T>
T sum(T* v, int n)
{
	T ret = v[0];
	for(int i = 1; i < n; i ++)
		ret += v[i];
	return ret;
}




#endif
