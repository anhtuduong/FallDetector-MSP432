#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#define MAXNUM 1500
#define WINDOW_SAMPLE 3
#define MODEL_SIZE 16
#define CLASSES 3
#define COLUMNS 4
#define NOISE 0
#define WALK 1
#define FALL 2
#define TIME 0
#define X 1
#define Y 2
#define Z 3
#define SIGMAS 3


//Globals
FILE* freeFallTime;
FILE* freeFallX;
FILE* freeFallY;
FILE* freeFallZ;

FILE* noiseTime;
FILE* noiseX;
FILE* noiseY;
FILE* noiseZ;

FILE* walkTime;
FILE* walkX;
FILE* walkY;
FILE* walkZ;
FILE* test;

float timeValues[CLASSES][MAXNUM], xValues[CLASSES][MAXNUM], yValues[CLASSES][MAXNUM], zValues[CLASSES][MAXNUM];
float meanValue[CLASSES][COLUMNS], devValue[CLASSES][COLUMNS];
int sizeNoise = 0, sizeWalk = 0, sizeFall = 0;
int count = 0;
int fallsCounter = 0;
int walkCounter = 0;
float detectedFalls[MAXNUM];
float detectedWalk[MAXNUM];


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS HEADER
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void openFiles();
void closeFiles();
void getValues();
void detectFalls();
void getMeanDEV();
void printMeanDEV();
void selectClass();



void main(){   
    
    //////////////////////////////////////////////////////
    //open files
    /////////////////////////////////////////////////////
    openFiles();    
    //////////////////////////////////////////////////////
    //reading files
    /////////////////////////////////////////////////////
    getValues();  
    //////////////////////////////////////////////////////
    //getting Means and Standars Deviation
    /////////////////////////////////////////////////////
    getMeanDEV();
    printMeanDEV();
    //////////////////////////////////////////////////////
    //Deviding Classes
    /////////////////////////////////////////////////////
    selectClass();
    //////////////////////////////////////////////////////
    //closing files
    /////////////////////////////////////////////////////
    closeFiles();
}

void openFiles(){  
    freeFallTime = fopen("freeFall/Time.txt", "r");
    freeFallX = fopen("freeFall/X.txt", "r");
    freeFallY = fopen("freeFall/Y.txt", "r");
    freeFallZ = fopen("freeFall/Z.txt", "r");
    noiseTime = fopen("Noise/Time.txt", "r");
    noiseX = fopen("Noise/X.txt", "r");
    noiseY = fopen("Noise/Y.txt", "r");
    noiseZ = fopen("Noise/Z.txt", "r");
    walkTime = fopen("Walk/Time.txt", "r");
    walkX = fopen("Walk/X.txt", "r");
    walkY = fopen("Walk/Y.txt", "r");
    walkZ = fopen("Walk/Z.txt", "r");
    test = fopen("freeFall/test.txt", "r");
    printf("All files have been opened\n");
}
void closeFiles(){
    fclose(freeFallTime);
    fclose(freeFallX);
    fclose(freeFallY);
    fclose(freeFallZ);
    fclose(noiseTime);
    fclose(noiseX);
    fclose(noiseY);
    fclose(noiseZ);
    fclose(walkTime);
    fclose(walkX);
    fclose(walkY);
    fclose(walkZ);
    fclose(test);
}
void getValues(){
    while(fscanf(freeFallTime,"%f", &timeValues[FALL][count])!=EOF){
        fscanf(freeFallX, "%f", &xValues[FALL][count]);
        fscanf(freeFallY, "%f", &yValues[FALL][count]);
        fscanf(freeFallZ, "%f", &zValues[FALL][count]);
        count++;
    }
    sizeFall = count;
    count = 0;
     while(fscanf(noiseTime, "%f", &timeValues[NOISE][count])!=EOF){
        fscanf(noiseX, "%f", &xValues[NOISE][count]);
        fscanf(noiseY, "%f", &yValues[NOISE][count]);
        fscanf(noiseZ, "%f", &zValues[NOISE][count]);
        count++;        
    }
    sizeNoise = count;
    count = 0;
    while(fscanf(walkTime, "%f", &timeValues[WALK][count])!=EOF){
        fscanf(walkY, "%f", &yValues[WALK][count]);
        fscanf(walkZ, "%f", &zValues[WALK][count]);
        fscanf(walkX, "%f", &xValues[WALK][count]);        
        count++;
    }
    sizeWalk = count;
}

void getMeanDEV(){
    float sumXMean=0, sumYMean=0, sumZMean=0;
    float sumXDEV = 0, sumYDEV = 0, sumZDEV = 0;
    // noise mean values
    for (int i = 0; i<sizeNoise; i++){
        sumXMean += xValues[NOISE][i];
        sumYMean += yValues[NOISE][i];
        sumZMean += zValues[NOISE][i];
    }
    meanValue[NOISE][X] = sumXMean/sizeNoise;
    meanValue[NOISE][Y] = sumYMean/sizeNoise;
    meanValue[NOISE][Z] = sumZMean/sizeNoise;
    //noise Dev values
    for (int i = 0; i<sizeNoise; i++){
        sumXDEV += pow((xValues[NOISE][i] - meanValue[NOISE][X]), 2);
        sumYDEV += pow((yValues[NOISE][i] - meanValue[NOISE][Y]), 2);
        sumZDEV += pow((zValues[NOISE][i] - meanValue[NOISE][Z]), 2);
    }
    devValue[NOISE][X]=sqrt(sumXDEV/(sizeNoise-1));
    devValue[NOISE][Y]=sqrt(sumYDEV/(sizeNoise-1));
    devValue[NOISE][Z]=sqrt(sumZDEV/(sizeNoise-1));
    // walk mean values
    sumXMean=0; sumYMean=0; sumZMean=0;
    sumXDEV = 0; sumYDEV = 0; sumZDEV = 0;
    for (int i = 0; i<sizeWalk; i++){
        sumXMean += xValues[WALK][i];
        sumYMean += yValues[WALK][i];
        sumZMean += zValues[WALK][i];
    }
    meanValue[WALK][X] = sumXMean/sizeWalk;
    meanValue[WALK][Y] = sumYMean/sizeWalk;
    meanValue[WALK][Z] = sumZMean/sizeWalk;
    //walk Dev values
    for (int i = 0; i<sizeWalk; i++){
        sumXDEV += pow((xValues[WALK][i] - meanValue[WALK][X]), 2);
        sumYDEV += pow((yValues[WALK][i] - meanValue[WALK][Y]), 2);
        sumZDEV += pow((zValues[WALK][i] - meanValue[WALK][Z]), 2);
    }
    devValue[WALK][X]=sqrt(sumXDEV/(sizeWalk-1));
    devValue[WALK][Y]=sqrt(sumYDEV/(sizeWalk-1));
    devValue[WALK][Z]=sqrt(sumZDEV/(sizeWalk-1));
    // noise mean values
    sumXMean=0; sumYMean=0; sumZMean=0;
    sumXDEV = 0; sumYDEV = 0; sumZDEV = 0;
    for (int i = 0; i<sizeFall; i++){
        sumXMean += xValues[FALL][i];
        sumYMean += yValues[FALL][i];
        sumZMean += zValues[FALL][i];
    }
    meanValue[FALL][X] = sumXMean/sizeFall;
    meanValue[FALL][Y] = sumYMean/sizeFall;
    meanValue[FALL][Z] = sumZMean/sizeFall;

     //fall Dev values
    for (int i = 0; i<sizeFall; i++){
        sumXDEV += pow((xValues[FALL][i] - meanValue[FALL][X]), 2);
        sumYDEV += pow((yValues[FALL][i] - meanValue[FALL][Y]), 2);
        sumZDEV += pow((zValues[FALL][i] - meanValue[FALL][Z]), 2);
    }
    devValue[FALL][X]=sqrt(sumXDEV/(sizeFall-1));
    devValue[FALL][Y]=sqrt(sumYDEV/(sizeFall-1));
    devValue[FALL][Z]=sqrt(sumZDEV/(sizeFall-1));
}

void printMeanDEV(){
    printf("Noises Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n", meanValue[NOISE][X], devValue[NOISE][X], meanValue[NOISE][Y], devValue[NOISE][Y], meanValue[NOISE][Z], devValue[NOISE][Z]);
    printf("Walk Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n", meanValue[WALK][X], devValue[WALK][X], meanValue[WALK][Y], devValue[WALK][Y], meanValue[WALK][Z], devValue[WALK][Z]);
    printf("Falls Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n", meanValue[FALL][X], devValue[FALL][X], meanValue[FALL][Y], devValue[FALL][Y], meanValue[FALL][Z], devValue[FALL][Z]);
}

void selectClass(){
   for (int i = 0; i<sizeFall-1; i++){
        if((yValues[FALL][i]>(meanValue[WALK][Y]+SIGMAS*devValue[WALK][Y]) || yValues[FALL][i]<(meanValue[WALK][Y]-SIGMAS*devValue[WALK][Y]))
        && (yValues[FALL][i+1]>(meanValue[WALK][Y]+SIGMAS*devValue[WALK][Y]) || yValues[FALL][i+1]<(meanValue[WALK][Y]-SIGMAS*devValue[WALK][Y]))){
            fallsCounter++;
            detectedFalls[i]=yValues[FALL][i];
            //printf("%f\t", detectedFalls[i]);
        }
    }
    printf("\n"); 
    printf("Falls s=%d: %d out of %d\n",SIGMAS, fallsCounter, sizeFall);
    printf("\n");
    fallsCounter=0;  
    for (int i = 0; i<sizeWalk-1; i++){
        if(yValues[WALK][i]>(meanValue[WALK][Y]+SIGMAS*devValue[WALK][Y]) || yValues[WALK][i]<(meanValue[WALK][Y]-SIGMAS*devValue[WALK][Y])){
            fallsCounter++;
            detectedFalls[i]=yValues[FALL][i];
            //printf("%f\t", detectedFalls[i]);
        }
    }
    printf("\n"); 
    printf("Walk s=%d: %d out of %d\n",SIGMAS, fallsCounter, sizeWalk);
    printf("\n");   
}
