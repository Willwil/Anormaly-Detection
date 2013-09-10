#include "globalInclude.h"
#include "utils.h"


//libSVM format
//lable index1:value2 index2:value2
/*bool writeInSVM(string writer, string reader, vector<int> label)
{
    FILE* rfile = fopen(reader.c_str(), "r");
    FILE* wfile = fopen(writer.c_str(), "w");
    if(!rfile || !wfile)
    {
        printf("error occured: can't open for read and write.\n");
        exit(1);
    }
    char c;
    int i = 1;
    int j = 0;
    while(fscanf(rfile, "%c", &c) != EOF)
    {
    		if(c == ' ')
    		{
    			fprintf(wfile, " %d:", i);
    			i ++;
    		}else if(c == ';' || c == '\n')
    		{
    			fprintf(wfile, "\n%d ", label[j]);
    			j ++;
    		}
    		else
    			fprintf(wfile, "%c", c);	
	}
	fclose(rfile);
	fclose(wfile);
	return true;
}*/


//matrix format
//every feature occupy a line
/*bool writeInNormal(string writer)
{
	printf("write the feature from file in matrix format ...\n");
    FILE* file = fopen(writer.c_str(), "r+");
    if(!file)
    {
        printf("error occured: can't open %s for read and write.\n", writer.c_str());
        exit(1);
    }
    char c;
    while(fscanf(file, "%c", &c) != EOF)
    {
    		if(c == ';')
    		{
    			fseek(file, -1, SEEK_CUR);
    			fputc('\n', file);
    		}	
	}
	fclose(file);
	return true;
}

//video extraction format
//every video occupy a line; Internal video the feature are seperated by ';'
bool writeFeatureToFile(const Mat& feature, string writer)
{
    if(feature.empty())
    {
        printf("feature extraction process may have some errors!\n");
        exit(1);
    }
    //check
    uint nl = feature.rows; 
    uint nc = feature.cols;  
    printf("rows: %d cols: %d\n", nl, nc);
    FILE* file = fopen(writer.c_str(), "a+");
    if(!file)
    {
        printf("error occured: can't open %s for write.\n", writer.c_str());
        exit(1);
    }
    for(int i =0 ; i < nl; i++)
    {
    	if(i > 0)
        	fprintf(file, ";");
        const float* data = feature.ptr<float>(i);
        for(int j = 0;  j < nc; j++)
        {
            fprintf(file, "%f ", data[j]);
        }
    }
    fprintf(file, "\n");
    fclose(file);
    return true;
}*/

void writeFeatureToFile(const Mat& data, string writer)
{
	if(data.empty())
    {
        printf("feature is null!!!\n");
        exit(1);
    }
    ofstream ofs;
	ofs.open(writer.c_str(), ofstream::app);
	if(!ofs.is_open())
	{
		printf("error occured: can't open %s for write.\n", writer.c_str());
        exit(1);
	}
	uint nl = data.rows; 
	uint nc = data.cols; 
	for(int i = 0 ; i < nl; i++)
	{
		if(i > 0)
			ofs << endl;
		for(int j = 0 ; j < nc; j++)
			ofs << data.at<float>(i, j) << " ";
	}
	ofs << endl;
	ofs.close();
}


/*void saveConvexPoint(const Mat& convexPoints, string writer)
{
	printf("start to save convex point to file ...\n");
	FILE* out;
	out = fopen(writer.c_str(), "w");
	if(!out)
		exit(1);
	int nl = convexPoints.rows;
	int nc = convexPoints.cols;
	for(int i = 0; i < nl; i ++)
	{
		for(int j = 0; j < nc; j ++)
			fprintf(out, "%f ", convexPoints.at<float>(i, j));
		fprintf(out, "\n");
	}
	fclose(out);
}

void loadConvexPoint(Mat& convexPoints, string reader)
{
	printf("start to load convex point from file ...\n");
	if(!convexPoints.empty())
		convexPoints.release();
	ifstream ifs(reader.c_str(), ifstream::in);
	if(!ifs.is_open())
		exit(1);
	string buffer;
	getline(ifs, buffer);
	int cols = find(buffer, ' ') + 1;
	Mat t;
	t.create(1, cols, CV_32FC1);
	float* data = (float*)t.data;
	float value;
	int i = 0;
	ifs.seekg(0, ios::beg);
	while(!ifs.eof())
	{
		for(i = 0; i < cols; i ++)
			ifs >> data[i];
		if(i < cols)
		{
			printf("%s : data error\n", reader.c_str());
			printf("i: %d, rows: %d\n", i, convexPoints.rows);
			exit(1);
		}
		convexPoints.push_back(t);
	}
	ifs.close();
}*/

