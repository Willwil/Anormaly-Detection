#include "globalInclude.h"
#include "model.h"

void readDataset(char* location, vector<string>& files);
void readLabels(char* file, vector<int>& labels);
bool options(int argc, char** argv, char** output, int& n);

int main(int argc, char** argv)
{
    /////////////////////////////////////////////////////
    ///usage:
    ///AnormalyDetection -t [0 1] [ [-f] or [-d [0 1 2 3...] [-l lable] location] ]
    //AnormalyDetection -t [0 1] -f
    //AnormalyDetection -t [0 1] -d [0 1 2 3...] [-l lable] location
    ///////////////////////////////////////////////////////

    /*printf("argc = %d\n", argc);
    for(int i  = 1; i < argc; i ++)
        printf("%d %s\n", i, argv[i]);*/

    char **output = new char*[4];
    for(int i = 0; i < 4; i ++)
    {
        output[i] = new char[MaxStringLength];
        memcpy(output[i], "\0", MaxStringLength);
    }
    int n = 0;
    if(!options(argc, argv, output, n))
    {
        printf("error usage!\n");
        printf("usage:program options location\n");
        printf("options:-t, -f, -d , -l\n");
        return 1;
    }
    /*for(int i = 0; i < 4; i ++)
    {
        printf("%d: %s\n", i+1, output[i]);
    }*/
    int type = atoi(output[0]);
    if(n == 1)
    {
        if(type == 0)
		{
            trainSVMModel();
		}
		if(type == 1)
		{
		    //printf("entering detection...\n");
            predictSVM();
		}
        return 0;
    }
    
    
    int dataset = atoi(output[1]);
    switch(dataset)
    {
            case 1://UMN
                frameWidth = 320;
    			frameHeight = 240;
    			break;
            case 2://BEHAVE
                frameWidth = 640;
                frameHeight = 480;
    			break;
            case 3://UCSD_PED1
                frameWidth = 238;
                frameHeight = 158;
    			break;
            case 4://UCSD_PED2
                frameWidth = 238;
                frameHeight = 158;
                break;
    		default:
    			break;
    }
    
    char* location = output[2];
	vector<string> files;
    readDataset(location, files);
    
    if(type == 0)
    {
        trainSVMModel(files);
    }
    if(type == 1)
    {
        //printf("entering detection...\n");
        vector<int> labels;
        readLabels(output[3], labels);
        predictSVM(files, labels);
    }

    return 0;
}

bool options(int argc, char** argv, char** output, int& n)
{
    int leastParams = 3;
    if(argc-1 < leastParams)
        return false;
    bool modeled = false;
    for(int i = 1; i < argc; i ++)
    {
        if(strcmp(argv[i], "-t") == 0)
        {
            i++;
            strcpy(output[0], argv[i]);
            n ++;
        }
        else if(strcmp(argv[i], "-f") == 0)
        {
            modeled = true;
        }
        else if(strcmp(argv[i], "-d") == 0)
        {
            i ++;
            strcpy(output[1], argv[i]);
            n ++;
        }
        else if(strcmp(argv[i], "-l") == 0)
        {
            i ++;
            strcpy(output[3], argv[i]);
            n ++;
        }
        else if(i == argc - 1)
        {
            strcpy(output[2], argv[i]);
            n ++;
        }
        else
            return false;
    }

    if(n < 0 || n > 4)
        return false;
    if(modeled)
    {
        if(n != 1)
            return false;
        else
            return true;
    }
    //printf("%d", n);
    if(n < 3)
        return false;
    if(n < 4 && strcmp(output[0], "1") == 0)
        return false;
    if(strcmp(output[0], "0") != 0 && strcmp(output[0], "1") != 0)
        return false;
    if(atoi(output[1]) <= 0 || atoi(output[1]) >= 5)
        return false;
    return true;
}

void readDataset(char* location, vector<string>& files)
{
    FILE* file;
    file = fopen(location, "r");
    if(!file)
    {
        printf("read file failed: %s\n", location);
        exit(1);
    }
    char buffer[MaxStringLength];
    while(fscanf(file, "%s", buffer) != EOF)
        files.push_back(buffer);
    fclose(file);
}

void readLabels(char* labelFile, vector<int>& labels)
{
    FILE* file;
    file = fopen(labelFile, "r");
    if(!file)
    {
        printf("read file failed: %s\n", labelFile);
        exit(1);
    }
    int start, end;
    while(fscanf(file, "%d:%d", &start, &end) != EOF)
    {
        if(start > end)
        {
            printf("error data!\n");
            exit(1);
        }
        labels.push_back(start);
        labels.push_back(end);
    }
    fclose(file);
}
