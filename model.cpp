#include "model.h"


int trainSVMModel()
{
    printf("start to train SVM model...\n");
    char cmd[MaxStringLength];
    //sprintf(cmd, "./svm-train -n 0.8 -v 5 train.model svmmodel");
    sprintf(cmd, "./svm-train -s 2 svm_train.model svmmodel");
    shellResultParse(system(cmd));
    return 0;
}

int predictSVM(int i)
{
    //printf("tag\n");
    printf("start to predict...\n");
    char cmd[MaxStringLength];
    char outputFile[MaxStringLength];
    sprintf(outputFile, "predict%d", i+1);
    sprintf(cmd, "./svm-predict svm_test.model svmmodel %s", outputFile);
    shellResultParse(system(cmd));
    //showResult(outputFile, labels[2*i], labels[2*i+1]);
    return 0;
}

void LDAEstimation()
{
	char cmd[MaxStringLength];
    sprintf(cmd, "R CMD BATCH lda_R");
    shellResultParse(system(cmd));
    
    double* p  =  readLdaResult("lda_result");
    int nl  =  feature.rows;
	double probality  =  1;
	for(int i  =  0; i < nl; i ++)
	{
        probality * =  p[indices[i]];
	}
	double threshod  =  0.2;
	if(probality > threshod)
		printf("normal\n");
	else
		printf("abnormal\n");
    
}

void trainDictionary()
{
	char cmd[MaxStringLength];
    sprintf(cmd, "matlab -nodisplay -nojvm -r sparseTrain");
    shellResultParse(system(cmd));
    
	/*module load matlab;
	matlab -nojvm -nodisplay -r test;*/
	//mcc -m mex, mcc -I ./1.m 2.m;
	
}

void predictSparseCoeffient()
{
	//Mat feature;
	//string writer  =  "X_test.mat";
	//featureExtrator(feature, file, writer);
	//writeInNormal(writer);
	
	char cmd[MaxStringLength];
    sprintf(cmd, "matlab -nodisplay -nojvm -r sparsePredict");
    shellResultParse(system(cmd));
    
    /////not end
}

void showResult(string outputFile, int start, int end)
{
    FILE* file  =  fopen(outputFile.c_str(), "r");
    int predictLabel;
    int count  =  0;
    int accuracy  =  0;
    int TP, TN, FP, FN;
    TP  =  0; TN  =  0; FP  =  0; FN  =  0;
    while(fscanf(file, "%d", &predictLabel) ! =  EOF)
    {
        count ++;
        if(predictLabel  =  =  1)
        {
            if(count > =  start && count < =  end)
            {
                accuracy ++;
                TP ++;
            }
            else
                FP ++;
        }
        if(predictLabel  =  =  -1)
        {
            if(count > =  start && count < =  end)
                FN ++;
            else
            {
                accuracy ++;
                TN ++;
            }
        }
    }
    printf("the true predict rate is %d.\n ", accuracy);
    printf("the false alarm rate is %d.\n ", FP);
    printf("the missing alarm rate is %d.\n ", FN);
}


void shellResultParse(int status)
{
	if(status  =  =  -1)
    {
        printf("system error!\n");
    }
    else
    {
         if(WIFEXITED(status) && WEXITSTATUS(status)  =  =  0)
         {
               printf("run shell script successfully.\n");
         }
         else
         {
            printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
         }
    }
}
	
		
		
