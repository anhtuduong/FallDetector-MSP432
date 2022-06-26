//****************************************************************************
// 
//                          FALL DETECTOR V1.0            
//
//                              utils.h
//          
//
//****************************************************************************

#ifndef UTILS_H_
#define UTILS_H_

#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>

void draw(char* text, int32_t x, int32_t y);
void delay(int seconds);
void delayWithCountDown(int i, int32_t x, int32_t y);
void displayRealTimeClock();
void lcd_AutoOrient();


#endif  // UTILS_H_
