/*
 * set_accelerometer.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Erik Nielsen
 */
#include <set_accelerometer.h>
#include  <hardware_init.h>
#include <math.h>

void calibration(int *mean, int *dev, int n){
    int i, tmp = 0, sum = 0, tmp_sum = 0;
    for(i = 0; i < n; i++){
        tmp = ADC14_getResult(ADC_MEM1);
        sum += tmp;
    }
    *mean = sum/n;
    sum = 0;
    for(i = 0; i < n; i++){
        tmp = ADC14_getResult(ADC_MEM1);
        tmp_sum = (*mean - tmp) * (*mean -tmp);
        sum += tmp_sum;
    }
    *dev = sqrt(sum/n -1);
}



