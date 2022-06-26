//****************************************************************************
// 
//                          FALL DETECTOR V1.0            
//
//                          hardware_init.h
//          
//
//****************************************************************************

#ifndef HARDWARE_INIT_H_
#define HARDWARE_INIT_H_

#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>

void _hwInit();
void _realTimeClockInit();
void _ADCInit();
void _ADC_Interrupt_enable_init();
void _graphicsInit();
void _accelSensorInit();
void _joystickInit();
void _buzzerInit();
void _buttonInit();


#endif  // HARDWARE_INIT_H_
