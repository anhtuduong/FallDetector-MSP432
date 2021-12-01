#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#define MAXNUM 4000
#define WINDOW_SAMPLE 3
#define MODEL_SIZE 16
#define CLASSES 5
#define COLUMNS 4
#define NOISE 0
#define WALK 1
#define FALL 2
#define CLOCK 3
#define PEND 4
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

FILE* clockTime;
FILE* clockX;
FILE* clockY;
FILE* clockZ;

FILE* pendTime;
FILE* pendX;
FILE* pendY;
FILE* pendZ;


float timeValues[CLASSES][MAXNUM], xValues[CLASSES][MAXNUM], yValues[CLASSES][MAXNUM], zValues[CLASSES][MAXNUM];
float meanValue[CLASSES][COLUMNS], devValue[CLASSES][COLUMNS];
int sizeNoise = 0, sizeWalk = 0, sizeFall = 0, sizeClock = 0, sizePend = 0;
int count = 0;
int fallsCounter = 0;
int walkCounter = 0;
float detectedFalls[MAXNUM];
float detectedWalk[MAXNUM];
int max[CLASSES][COLUMNS]={0};
int min[CLASSES][COLUMNS]={0};
int size=0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS HEADER
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void openFiles();
void closeFiles();
void getValues();
void detectFalls();
void getMeanDEV();
void printMeanDEV();
void maxMinValues();
void chooseSize(int i);

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
    //MAX MIN Values
    /////////////////////////////////////////////////////
    maxMinValues();
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

    clockTime = fopen("Clock/Time.txt", "r");
    clockX = fopen("Clock/X.txt", "r");
    clockY = fopen("Clock/Y.txt", "r");
    clockZ = fopen("Clock/Z.txt", "r");

    pendTime = fopen("Pendolum/Time.txt", "r");
    pendX = fopen("Pendolum/X.txt", "r");
    pendY = fopen("Pendolum/Y.txt", "r");
    pendZ = fopen("Pendolum/Z.txt", "r");

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
    fclose(clockTime);
    fclose(clockX);
    fclose(clockY);
    fclose(clockZ);
    fclose(pendTime);
    fclose(pendX);
    fclose(pendY);
    fclose(pendZ);
    
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
    count = 0;
    while(fscanf(clockTime, "%f", &timeValues[CLOCK][count])!=EOF){
        fscanf(clockY, "%f", &yValues[CLOCK][count]);
        fscanf(clockZ, "%f", &zValues[CLOCK][count]);
        fscanf(clockX, "%f", &xValues[CLOCK][count]);        
        count++;
    }
    sizeClock = count;
    count = 0;
    while(fscanf(pendTime, "%f", &timeValues[PEND][count])!=EOF){
        fscanf(pendY, "%f", &yValues[PEND][count]);
        fscanf(pendZ, "%f", &zValues[PEND][count]);
        fscanf(pendX, "%f", &xValues[PEND][count]);        
        count++;
    }
    sizePend = count;
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
    // fall mean values
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
        // Clock mean values
    sumXMean=0; sumYMean=0; sumZMean=0;
    sumXDEV = 0; sumYDEV = 0; sumZDEV = 0;
    for (int i = 0; i<sizeClock; i++){
        sumXMean += xValues[CLOCK][i];
        sumYMean += yValues[CLOCK][i];
        sumZMean += zValues[CLOCK][i];
    }
    meanValue[CLOCK][X] = sumXMean/sizeClock;
    meanValue[CLOCK][Y] = sumYMean/sizeClock;
    meanValue[CLOCK][Z] = sumZMean/sizeClock;

     //clock Dev values
    for (int i = 0; i<sizeClock; i++){
        sumXDEV += pow((xValues[CLOCK][i] - meanValue[CLOCK][X]), 2);
        sumYDEV += pow((yValues[CLOCK][i] - meanValue[CLOCK][Y]), 2);
        sumZDEV += pow((zValues[CLOCK][i] - meanValue[CLOCK][Z]), 2);
    }
    devValue[CLOCK][X]=sqrt(sumXDEV/(sizeClock-1));
    devValue[CLOCK][Y]=sqrt(sumYDEV/(sizeClock-1));
    devValue[CLOCK][Z]=sqrt(sumZDEV/(sizeClock-1));
        // Pend mean values
    sumXMean=0; sumYMean=0; sumZMean=0;
    sumXDEV = 0; sumYDEV = 0; sumZDEV = 0;
    for (int i = 0; i<sizePend; i++){
        sumXMean += xValues[PEND][i];
        sumYMean += yValues[PEND][i];
        sumZMean += zValues[PEND][i];
    }
    meanValue[PEND][X] = sumXMean/sizePend;
    meanValue[PEND][Y] = sumYMean/sizePend;
    meanValue[PEND][Z] = sumZMean/sizePend;

     //Pend Dev values
    for (int i = 0; i<sizePend; i++){
        sumXDEV += pow((xValues[PEND][i] - meanValue[PEND][X]), 2);
        sumYDEV += pow((yValues[PEND][i] - meanValue[PEND][Y]), 2);
        sumZDEV += pow((zValues[PEND][i] - meanValue[PEND][Z]), 2);
    }
    devValue[PEND][X]=sqrt(sumXDEV/(sizePend-1));
    devValue[PEND][Y]=sqrt(sumYDEV/(sizePend-1));
    devValue[PEND][Z]=sqrt(sumZDEV/(sizePend-1));

}

void printMeanDEV(){
    printf("Noises Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n\n", meanValue[NOISE][X], devValue[NOISE][X], meanValue[NOISE][Y], devValue[NOISE][Y], meanValue[NOISE][Z], devValue[NOISE][Z]);
    printf("Walk Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n\n", meanValue[WALK][X], devValue[WALK][X], meanValue[WALK][Y], devValue[WALK][Y], meanValue[WALK][Z], devValue[WALK][Z]);
    printf("Falls Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n\n", meanValue[FALL][X], devValue[FALL][X], meanValue[FALL][Y], devValue[FALL][Y], meanValue[FALL][Z], devValue[FALL][Z]);
    printf("Noises Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n\n", meanValue[NOISE][X], devValue[NOISE][X], meanValue[NOISE][Y], devValue[NOISE][Y], meanValue[NOISE][Z], devValue[NOISE][Z]);
    printf("Clock Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n\n", meanValue[CLOCK][X], devValue[CLOCK][X], meanValue[CLOCK][Y], devValue[CLOCK][Y], meanValue[CLOCK][Z], devValue[CLOCK][Z]);
    printf("Pend Means (x, y, z) = (X= %f +- %f, Y= %f +- %f, Z= %f +- %f)\n\n", meanValue[PEND][X], devValue[PEND][X], meanValue[PEND][Y], devValue[PEND][Y], meanValue[PEND][Z], devValue[PEND][Z]);
}
void chooseSize(int i){
    if( i == NOISE){
        size = sizeNoise;
    }
    if( i == WALK){
        size = sizeWalk;
    }
    if( i == CLOCK){
        size = sizeClock;
    }
    if( i == FALL){
        size = sizeFall;
    }
    if( i == PEND){
        size = sizePend;
    }
}
void maxMinValues(){
    //Max min noise
    for (int i = 0; i<CLASSES; i++){
        for(int k = 0; k<COLUMNS; k++){
            max[i][k]=meanValue[i][k];
            min[i][k]=meanValue[i][k];
        }
    }

    for(int k = 0; k<CLASSES; k++){
        chooseSize(k);   
        for(int i = 0; i<size; i++){
                if(xValues[i] <= min[k][X]){
                    min[k][X]=xValues[i];
                }
                if(xValues[i] >= max[k][X]){
                    max[k][X]=xValues[i];
                }
                if(yValues[i] <= min[k][Y]){
                    min[k][Y]=yValues[i];
                }
                if(yValues[i] >= max[k][Y]){
                    max[k][Y]=yValues[i];
                }
                if(zValues[i] <= min[k][Z]){
                    min[k][X]=zValues[i];
                }
                if(zValues[i] >= max[k][Z]){
                    max[k][Z]=zValues[i];
                }
            }           
    }
    printf("Noises MAX (X x, Y y, Z z) = (X= %d x= %d, Y= %d y= %d, Z= %d z= %d)\n\n", max[NOISE][X], min[NOISE][X], max[NOISE][Y], min[NOISE][Y], max[NOISE][Z], min[NOISE][Z]);
    printf("Walk MAX (X x, Y y, Z z) = (X= %d x= %d, Y= %d y= %d, Z= %d z= %d)\n\n", max[WALK][X], min[WALK][X], max[WALK][Y], min[WALK][Y], max[WALK][Z], min[WALK][Z]);
    printf("Clock MAX (X x, Y y, Z z) = (X= %d x= %d, Y= %d y= %d, Z= %d z= %d)\n\n", max[CLOCK][X], min[CLOCK][X], max[CLOCK][Y], min[CLOCK][Y], max[CLOCK][Z], min[CLOCK][Z]);
    printf("Fall MAX (X x, Y y, Z z) = (X= %d x= %d, Y= %d y= %d, Z= %d z= %d)\n\n", max[FALL][X], min[FALL][X], max[FALL][Y], min[FALL][Y], max[FALL][Z], min[FALL][Z]);
    printf("Pend MAX (X x, Y y, Z z) = (X= %d x= %d, Y= %d y= %d, Z= %d z= %d)\n\n", max[PEND][X], min[PEND][X], max[PEND][Y], min[PEND][Y], max[PEND][Z], min[PEND][Z]);
                 
}