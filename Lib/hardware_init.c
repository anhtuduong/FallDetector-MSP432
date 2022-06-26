//****************************************************************************
//
//                          FALL DETECTOR V1.0
//
//                          hardware_init.c
//
//                  Initialize hardware + wifi + sensors
//
//****************************************************************************

#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include <Lib/hardware_init.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <time.h>

extern Graphics_Context g_sContext;


//****************************************************************************

void _hwInit()
{
    // Halting WDT and disabling master interrupts
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    // Set the core voltage level to VCORE1
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    // Set 2 flash wait states for Flash bank 0 and 1
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    // Initializes Clock System
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

void _realTimeClockInit()
{
    time_t now;
    now = time(0);
    struct tm *local = localtime(&now);

    RTC_C_Calendar timeInit;
    timeInit.hours      = local->tm_hour;
    timeInit.minutes    = local->tm_min;
    timeInit.seconds    = local->tm_sec;
    timeInit.dayOfmonth = local->tm_mday;
    timeInit.month      = local->tm_mon;
    timeInit.year       = local->tm_year;
    
    MAP_RTC_C_initCalendar(&timeInit, RTC_C_FORMAT_BINARY);
    MAP_RTC_C_startClock();
}

void _ADCInit()
{
    _accelSensorInit();
    _joystickInit();

    // Initializing ADC (ADCOSC/64/8)
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    // Configuring ADC Memory (ADC_MEM0 - ADC_MEM4 with no repeat)
    // with internal 2.5v reference
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true);

    // Accelerometer X - MEM0 - A14
    ADC14_configureConversionMemory(ADC_MEM0,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);

    // Accelerometer Y - MEM1 - A13
    ADC14_configureConversionMemory(ADC_MEM1,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);

    // Accelerometer Z - MEM2 - A11
    ADC14_configureConversionMemory(ADC_MEM2,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);

    // Joystick X - MEM3 - A15
    ADC14_configureConversionMemory(ADC_MEM3,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    // Joystick Y - MEM4 - A9
    ADC14_configureConversionMemory(ADC_MEM4,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);


    // Setting up the sample timer to automatically step through the sequence
    // convert
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    // Triggering the start of the sample
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

void _ADC_Interrupt_enable_init(){
    // Enabling the interrupt when a conversion on channel 2 (end of sequence)
    // is complete and enabling conversions
    ADC14_enableInterrupt(ADC_INT4);

    // Enabling Interrupts
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();
}

void _accelSensorInit()
{
    // Pin 4.0, 4.2, 6.1 as ADC input
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN0 | GPIO_PIN2,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN1,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
}

void _joystickInit()
{
    // Pin 6.0, 4.4 as ADC input
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
}

void _graphicsInit()
{
    // Initializes display
    Crystalfontz128x128_Init();

    // Set default screen orientation
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initializes graphics context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

}

void _buzzerInit()
{
    // P2.7 to PM_TA0.4 for using Timer PWM to control LED
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                                                GPIO_PIN7,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
}

void _buttonInit()
{
    // // P5.1 for Button S1
    // GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    //                                            GPIO_PIN1,
    //                                            GPIO_PRIMARY_MODULE_FUNCTION);
    // // P3.5 for Button S2
    // GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
    //                                            GPIO_PIN5,
    //                                            GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);

}

