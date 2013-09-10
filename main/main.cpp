#include "../excutableAlg.h"
#include "../DataSet.h"


/*struct ParamType
{
	int algType;
	char trainFile[MaxStringLength];
	char testFile[MaxStringLength];
	char videoFile[MaxStringLength];
	enum DataSet dataset;
};*/

void help()
{
    printf("AnormalyDetection [train|test] dataset videofiles\n"); 
    printf("dataset(UMN, BEHAVE, UCSD_PED1, or UCSD_PED2).\n");
}

void configDataset(char* data)
{
    DataSet dataset;
    if(strcmp(data, "UMN") == 0)
        dataset = UMN;
    else if(strcmp(data, "BEHAVE") == 0)
        dataset = BEHAVE;
    else if(strcmp(data, "UCSD_PED1") == 0)
        dataset = UCSD_PED1;
    else if(strcmp(data, "UCSD_PED2") == 0)
        dataset = UCSD_PED2;
    else
    {
        help();
        exit(1);
    }

    switch(dataset)
    {
        case UMN:
            frameWidth = 320;
            frameHeight = 240;
            break;
        case BEHAVE:
            frameWidth = 640;
            frameHeight = 480;
            break;
        case UCSD_PED1:
            frameWidth = 238;
            frameHeight = 158;
            break;
        case UCSD_PED2:
            frameWidth = 238;
            frameHeight = 158;
            break;
        default:
            break;
    }
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


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        help();
        exit(1);
    }
    vector<string> files;
    if(argc >= 3)
    {
        configDataset(argv[2]);
        readDataset(argv[3], files);
    }
    if(strcmp(argv[1] , "train") == 0)
		train(files);
    else if(strcmp(argv[1] , "test") == 0)
        predict(files);
}