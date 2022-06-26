/*
 * data_collector.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Erik Nielsen
 */
#include <data_collector.h>
#include <uart_driver.h>
#include <hardware_init.h>

#define X 0
#define Y 1
#define Z 2

void collect(){
    int tmp[3];
    int counter = 0;
    char msg[25];
    while(1){
        tmp[X] = ADC14_getResult(ADC_MEM0);
        tmp[Y] = ADC14_getResult(ADC_MEM1);
        tmp[Z] = ADC14_getResult(ADC_MEM2);
        counter++;
        sprintf(msg , "%d,%d,%d", tmp[X], tmp[Y], tmp[Z]);
        uart_println(msg);
    }
}
