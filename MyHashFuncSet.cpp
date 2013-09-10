#include "MyHashFuncSet.h"
#include "utils.h"

#define HASH_FUNC_SIGMOID
#define choice 2

void HashAllItems(const Mat& feature, const Mat& proj, HashParam& p)
{
	printf("start to build the bucket info ...\n");
	int nl = feature.rows;
	int nc = feature.cols;

	//hash every items
	//core:compute value for hash, but the type is continus, float, and not normalised
	Mat projectValue;
	ProjectAll(p, feature, proj, projectValue);
	
	//change the range(minRange, maxRange) to {0, 1, ..., bucketLength-1}
	//hash process
	p.hashPoint.create(nl, p.bucketNumber, CV_32SC1);
	float mid = p.range /2;
	float pi = 3.14159265;
	
	#ifdef HASH_FUNC_ORIGINAL
		p.r = (p.bucketLength - 1)/p.range;
	#endif
	
	#ifdef HASH_FUNC_SIGMOID

		float y1 = 1.0;
		float x1 = 0;
		int acc = 0, wc = 0;
		int bins = p.bucketLength;
		float seg = p.range / bins;
		int delta = 4;
		
		vector<int> hh = calcHist(projectValue.ptr<float>(0), nl, bins);
		vector<int> shh = smooth(hh, bins);
		for(int i = 0; i < bins; i ++)
		{
			//printf("%d", shh[i]);
			if(shh[i] < delta)
			{
				acc += shh[i];
				wc ++;
			}
			else
				break;
		}
		printf("wc: %d, acc: %d, seg: %f\n", wc, acc, seg);
		y1 = acc * bins * 1.0/ nl * 2;
		x1 = wc * seg - p.range/2;
		
		p.a = p.bucketLength;
		p.b = 1.0;
		p.c = -log(p.a/y1 - p.b)/x1;
		printf("x1: %f, y1: %f, p.c: %f\n", x1, y1, p.c);
		
	#endif
	
	#ifdef HASH_FUNC_ATAN
		p.a = (p.bucketLength - 1)/pi;
		p.b = 2.0;
		p.c = (p.bucketLength-1)/2;
	#endif
			
	#ifdef HASH_FUNC_LOG
	
	
	#endif
	
	
	FILE* out = fopen("/home/administrator/data.dat", "w");
	for(int bucket = 0; bucket < p.bucketNumber; bucket ++)
	{
		for(int pp = 0; pp < nl; pp ++)
		{
			float y = projectValue.at<float>(bucket, pp) - p.minRange;
			
			if(bucket == 0)
			{
				fprintf(out, "%f ", y);
			}
			#ifdef HASH_FUNC_ORIGINAL
				y = y * r;
			#endif
			
			//x**3
			#ifdef HASH_FUNC_POWER_THREE
				printf("using hash function 2\n");
				y = 2 * y / p.range - 1;
				if(y < 0)
				{
					y = 0 - pow(0 - y, 1.0/3.0);
				}
				else
				{
					y = pow(y, 1.0/3.0);
				}
				y = (y + 1) * (p.bucketLength - 1) /2.0;
			#endif
			
			//sigmoid a/(1 + b * exp(-c*x))
			#ifdef HASH_FUNC_SIGMOID
				y = y - mid;
				y = p.a / (1 + p.b * exp(-p.c * y));
			#endif
			
			//atan a*atan(b*x)+c
			#ifdef HASH_FUNC_ATAN
				y = y - mid;
				y = p.a * atan(p.b * y) + p.c;
			#endif
			
		    // 
			#ifdef HASH_FUNC_LOG
				
			#endif
			
			int h = (int)(y);
			if(h < 0 || h >= p.bucketLength)
			{
				printf("y: %f, h: %d\n", y, h);
				programPause();
			}
			p.bucketInfo[bucket][h].push_back(pp);
			//p.hashPoint.at<int>(pp, bucket) = h;
		}
	}//end of hash every items
	fclose(out);
	
	printf("\nexit projection ...\n");
	
	/*int showPixels = 1;
	Mat img = Mat::zeros(nl/levels * 5, showPixels*levels, CV_8UC3);
	Scalar color(0, 255, 255);
	int showlevels = p.bucketInfo[0].size();
	//int showlevels = levels;
	for(int i = 0; i < showlevels; i ++)
	{
		int v = p.bucketInfo[0][i].size();
		printf("%d ", v);
		//rectangle(img, Point(i*showPixels, 0), Point(i*showPixels, min(v, img.rows-1)), color, CV_FILLED);
	}
	printf("\n");
	namedWindow("bins");
	imshow("bins", img);
	waitKey(0);*/
	
	programPause();
}



int hashFunction(HashParam p, vector<float> shiftVector, const Mat& subProj)
{
	vector<float> p3(p.projectionSize, 0);
	int pr_size = subProj.cols;
	int nc = shiftVector.size();
	//printf("p.projectionSize: %d,  pr_size: %d\n", p.projectionSize, pr_size);
	for(int tt = 0; tt < p.projectionSize; tt ++)
	{
		int index = subProj.at<int>(0, tt);
		if(index < 0 || index >= nc)
			printf("index: %d, col: %d\n", index, tt);
		p3[tt] = shiftVector[index];
	}
	float y = getProjectValue(p, p3);
	y = y - p.minRange;
	
	float mid = p.range /2;
	#ifdef HASH_FUNC_ORIGINAL
		y = y * p.r;
	#endif
	
	#ifdef HASH_FUNC_POWER_THREE
		y = 2 * y / p.range - 1;
		if(y < 0)
		{
			y = 0 - pow(0 - y, 1.0/3.0);
		}
		else
		{
			y = pow(y, 1.0/3.0);
		}
		y = (y + 1) * (p.bucketLength - 1) /2.0;
	#endif
	
	#ifdef HASH_FUNC_SIGMOID
		
		y = y - mid;
		y = p.a / (1 + p.b * exp(-p.c * y));
	#endif
	
	#ifdef HASH_FUNC_ATAN

		y = y - mid;
		y = p.c * atan(p.t * y) + (p.bucketLength-1)/2;
	#endif
	

	#ifdef HASH_FUNC_LOG
		
	#endif
	
	int h = (int)(y);
	return h;
}


void ProjectAll(HashParam& p, const Mat& feature, const Mat& proj, Mat& projectValue)
{	
	printf("start to project  ...\n");
	int nl = feature.rows;
	int nc = feature.cols;

	//hash every items
	//core:compute value for hash, but the type is continus, float, and not normalised
	projectValue.create(p.bucketNumber, nl, CV_32FC1);
	
	bool initMinMax = false;
	vector<float> p3(p.projectionSize, 0);
	
	for(int bucket = 0; bucket < p.bucketNumber; bucket ++)
	{
		for(int pp = 0; pp < nl; pp ++)
		{
			for(int tt = 0; tt < p.projectionSize; tt ++)
			{
				int cc = proj.at<int>(bucket, tt);
				p3[tt] = feature.at<float>(pp, cc);
			}
			//float k = getProjectValue(p, p3);
			float k = 0.0;
			float dtemp = 0.0;
			float variance = 0.0;
			vector<float> v1(p.projectionSize, 0), v2(p.projectionSize, 0);
			for(int tt = 0; tt < p.projectionSize; tt ++)
			{
				v1[tt] = p3[tt] - p.p1[tt];
				v2[tt] = p.p2[tt] - p.p1[tt];
				k += v1[tt] * v2[tt];
				dtemp += v2[tt] * v2[tt];
				variance += v1[tt] * v1[tt];
			}
			dtemp = sqrt(dtemp);
			k = k /dtemp;
			variance = sqrt(variance);
			float w = 1.0/variance;
			variance = acos(k/variance);
			//variance = w * variance;
			k = p.we * k + (1 - p.we) * variance;
			//k = (1 - w) * k + w * variance;
			projectValue.at<float>(bucket, pp) = k;
			//printf("%f ", k);
			
			if(initMinMax)
			{
				if(k > p.maxRange)
					p.maxRange = k;
				if(k < p.minRange)
					p.minRange = k;
			}
			else
			{
				p.maxRange = p.minRange = k;
				initMinMax = true;
			}
		}
		//printf("\n");
	}
	p.range = p.maxRange - p.minRange;
	printf("maxRange: %f, minRange: %f, range: %f.\n", p.maxRange, p.minRange, p.range);
}



float getProjectValue(HashParam p, vector<float> p3)
{
	float k = 0.0;
	float dtemp = 0.0;
	float variance = 0.0;
	vector<float> v1(p.projectionSize, 0), v2(p.projectionSize, 0);
	for(int tt = 0; tt < p.projectionSize; tt ++)
	{
		v1[tt] = p3[tt] - p.p1[tt];
		v2[tt] = p.p2[tt] - p.p1[tt];
		k += v1[tt] * v2[tt];
		dtemp += v2[tt] * v2[tt];
		variance += v1[tt] * v1[tt];
	}
	dtemp = sqrt(dtemp);
	k = k /dtemp;
	variance = sqrt(variance);
	float w = 1.0/variance;
	variance = acos(k/variance);
	//variance = w * variance;
	k = p.we * k + (1 - p.we) * variance;
	//k = (1 - w) * k + w * variance;
	return k;
}

vector<int> calcHist(float* v, int n, int bins)
{
	vector<int> histograms(bins, 0);
	float MM = *max_element(v, v+n);
	float mm = *min_element(v, v + n);
	float seg = (MM - mm) * 1.0/ bins;
	for(int pp = 0; pp < n; pp ++)
	{
		histograms[int((v[pp] - mm)/seg)]  ++;
	}
	return histograms;
}

vector<int> smooth(vector<int> hists, int bins)
{
	vector<int> sh(bins, 0);
	sh[0] = (hists[0] + hists[1]) /2;
	sh[bins - 1] = (hists[bins - 1] + hists[bins -2]) / 2;
	for(int pp = 1; pp < bins - 1; pp ++)
	{
		sh[pp] = (hists[pp-1] + hists[pp] + hists[pp+1])/3;
		//printf("%d ", h[pp])
	}
	return sh;
}
