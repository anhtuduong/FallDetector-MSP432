//****************************************************************************
// 
//                          FALL DETECTOR V1.0            
//
// main.c - MSP-EXP432P401R + Educational Boosterpack MkII
//          
//          Accelerometer   X   P6.1    A14         ADC
//                          Y   P4.0    A13         ADC
//                          Z   P4.2    A11         ADC
// 
//          Joystick        X   P6.0    A15         ADC
//                          Y   P4.4    A9          ADC
//                          SEL P4.1    A12         GPIO
// 
//          Buzzer              P2.7    PM_TA0.4    PWM
// 
//          Button S1           P5.1    A4          GPIO
// 
//          Button S2           P3.5    PM_UCB2CLK

//          ESP8266         TX  P3.2
//                          RX  P3.3
//                          EN  P6.1
//
//****************************************************************************

#define TESTING_WITH_A
// #define TESTING_WITH_J
#define BUZZER_ON

#include <stdio.h>
#include <stdbool.h>
#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <hardware_init.h>
#include <utils.h>
#include <wifi.h>


void startUp();
void buzzerPlay(int repeat);
int fallDetected();
void startFallAlert();
void startSOS();


#define X 0
#define Y 1
#define Z 2
#define N_SAMPLE 10000
#define SIGMAS 3
#define AX 3
#define MAX 15000
#define MIN 2000
#define NOISE_DEV 4

#define BUTTON_1_PRESSED !GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1)
#define BUTTON_2_PRESSED !GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5)

//Globals
const uint32_t uart_baudrate = 115200;
const uint32_t min = (MIN - 3*NOISE_DEV);
const uint32_t max = (MAX + 3*NOISE_DEV);

// Graphic library context
Graphics_Context g_sContext;

// Timer_A Compare Configuration Parameter (PWM)
Timer_A_CompareModeConfig compareConfig_PWM = {
TIMER_A_CAPTURECOMPARE_REGISTER_4,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        5000                                    // 10% Duty Cycle initially
        };

// Timer_A Up Configuration Parameter
const Timer_A_UpModeConfig upConfig = {
TIMER_A_CLOCKSOURCE_SMCLK,                      // SMCLK = 3 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_3,         // SMCLK/3 = 1000 KhZ
        10000,                                  // .. ms tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
        };


//****************************************************************************
//                              Main function
int main(void)
{
    _hwInit();
    _realTimeClockInit();
    _graphicsInit();
    
    startUp();

    _ADCInit();
    _buzzerInit();
    _buttonInit();

    _ADC_Interrupt_enable_init();


    while(1)
    {   
        displayRealTimeClock();

        if(fallDetected())
        {
            startFallAlert();
        }

        if(BUTTON_2_PRESSED)    // S2 button pressed
        {
            startSOS();
        }
    }
}
//****************************************************************************


void startUp()
{
    Graphics_clearDisplay(&g_sContext);
    draw("FALL DETECTOR", 64, 50);
    draw("v1.0", 64, 64);
    
    delayWithCountDown(5000000, 64, 80);

    Graphics_clearDisplay(&g_sContext);
}

void buzzerPlay(int repeat)
{
    while(repeat > 0)
    {
        /* Configuring Timer_A0 for Up Mode and starting */
        Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
        Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

        /* Initialize compare registers to generate PWM */
        Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM); // For P2.7

        delay(500000);

        Timer_A_stopTimer(TIMER_A0_BASE);

        delay(50000);

        repeat--;
    }
   
}

void startFallAlert()
{
    Graphics_clearDisplay(&g_sContext);
    draw("FALL DETECTED", 64, 30);
    draw("Press S1 if OK", 64, 50);
    draw("Press S2 to SOS", 64, 70);

    #ifdef BUZZER_ON
    buzzerPlay(5);
    #endif


    char string[3];
    int i;
    bool isOK = 0;
    for(i=10000000; i>=0; i--)
    {
        if(i%1000000 == 0)  // Countdown
        {
            sprintf(string, "%2d", i/1000000);
            draw(string, 64, 90);
        }

        if(BUTTON_1_PRESSED)    // S1 button pressed
        {
            isOK = 1;
            Graphics_clearDisplay(&g_sContext);
            draw("OK", 60, 64);
            delayWithCountDown(5000000, 70, 64);
            Graphics_clearDisplay(&g_sContext);
            break;
        }
        if(BUTTON_2_PRESSED)    // S2 button pressed
        {
            break;
        }
    }
    if(!isOK)
    {
        startSOS();
    }
    Graphics_clearDisplay(&g_sContext);
}

void startSOS()
{
    #ifdef BUZZER_ON
    buzzerPlay(1);
    #endif

    Graphics_clearDisplay(&g_sContext);
    draw("Sending SOS..", 64, 60);

    wifi_start();

    _graphicsInit();
    Graphics_clearDisplay(&g_sContext);
    draw("SOS sent!", 64, 60);
    delayWithCountDown(5000000, 64, 80);

    Graphics_clearDisplay(&g_sContext);
}

//****************************************************************************
//                          Interrupts 

// Get X, Y, Z from accelerometer
// 
void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();

    MAP_ADC14_clearInterruptFlag(status);

    /* ADC_MEM4 conversion completed */
    if(status & ADC_INT4)
    {
        lcd_AutoOrient();
    }
}

//****************************************************************************
//                  Algorithms for detecting the fall

#ifdef TESTING_WITH_J
int fallDetected()
{
    int detect;
    detect = 0;

    // Using joystick just to test functions after detected

    uint16_t joysBuffer[2];

    joysBuffer[X] = ADC14_getResult(ADC_MEM3);
    joysBuffer[Y] = ADC14_getResult(ADC_MEM4);

    if(joysBuffer[X] == 0 || joysBuffer[Y] == 0)
    {
        detect = 1;
    }

    return detect;
}
#endif  // TESTING_WITH_J

//****************************************************************************

#ifdef TESTING_WITH_A
int fallDetected(){
    int tmp_1[AX], tmp_2[AX], detect = 0, i;

    tmp_1[X] = ADC14_getResult(ADC_MEM0);
    tmp_1[Y] = ADC14_getResult(ADC_MEM1);
    tmp_1[Z] = ADC14_getResult(ADC_MEM2);
    delay(50);
    tmp_2[X] = ADC14_getResult(ADC_MEM0);
    tmp_2[Y] = ADC14_getResult(ADC_MEM1);
    tmp_2[Z] = ADC14_getResult(ADC_MEM2);

    for(i = 0; i < AX; i++){
        if(tmp_1[i] < min || tmp_1[i] > max){
            if(tmp_2[i] < min || tmp_2[i] > max){
                detect = 1;
            }
        }
    }

    return detect;
}
#endif  // TESTING_WITH_A

