#include "featureIO.h"
#include "globalInclude.h"



bool writeInSVM(string writer, string reader, vector<int> label)
{
    FILE* rfile = fopen(writer.c_str(), "r");
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
}

bool writeInNormal(string writer)
{
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

bool writeFeatureToFile(const Mat& feature, string writer)
{
    if(feature.empty())
    {
        printf("feature extraction process may have some errors!\n");
        exit(1);
    }
    //check
    uint nl = feature.rows; //样例的个数
    uint nc = feature.cols;  //样例的维数
    printf("rows: %d cols: %d\n", nl, nc);
    FILE* file = fopen(writer.c_str(), "a+");
    if(!file)
    {
        printf("error occured: can't open %s for write.\n", writer.c_str());
        exit(1);
    }
    for(int i =0 ; i < nl; i++)
    {
        const float* data = feature.ptr<float>(i);
        for(int j = 0;  j < nc; j++)
        {
            fprintf(file, "%f ", data[j]);
        }
        fprintf(file, ";");
    }
    fprintf(file, "\n");
    fclose(file);
    return true;
}

double* readLdaResult(string fileName)
{
    return 0;
}

