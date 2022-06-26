//****************************************************************************
//
//                          FALL DETECTOR V1.0
//
//                              utils.c
//
//                          utility functions
//
//****************************************************************************


#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "utils.h"

#define X 0
#define Y 1
#define Z 2

extern Graphics_Context g_sContext;


//****************************************************************************

void draw(char* text, int32_t x, int32_t y)
{
    Graphics_drawStringCentered(&g_sContext,
                                (int8_t *) text,
                                AUTO_STRING_LENGTH,
                                x,
                                y,
                                OPAQUE_TEXT);
}

void delay(int i)
{
    int k;
    for(k=i; k>=0; k--);
}

void delayWithCountDown(int i, int32_t x, int32_t y)
{
    int k;
    char string[3];
    for(k=i; k>=0; k--)
    {
        if(k%1000000 == 0)
        {
            sprintf(string, "%2d", k/1000000);
            draw(string, x, y);
        }
    }
}

void displayRealTimeClock()
{
    RTC_C_Calendar currentTime = MAP_RTC_C_getCalendarTime();
    char string[14];
    sprintf(string, "%02d / %02d / %04d",   (int)(currentTime.dayOfmonth),
                                            (int)(currentTime.month + 1),
                                            (int)(currentTime.year + 1900));
    draw(string, 64, 50);

    sprintf(string, "%02d : %02d : %02d",   (int)(currentTime.hours + 7),
                                            (int)(currentTime.minutes),
                                            (int)(currentTime.seconds));
    draw(string, 64, 70);
}

void lcd_AutoOrient()
{
    // ADC results buffer
        uint16_t accelBuffer[3]; 
        accelBuffer[X] = ADC14_getResult(ADC_MEM0);
        accelBuffer[Y] = ADC14_getResult(ADC_MEM1);
        accelBuffer[Z] = ADC14_getResult(ADC_MEM2);

        // LCD auto orientation
        if (accelBuffer[X] < 4900) {
            if (Lcd_Orientation != LCD_ORIENTATION_LEFT) {
                Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_LEFT);
                Graphics_clearDisplay(&g_sContext);
            }
        }
        else if (accelBuffer[X] > 11300) {
            if (Lcd_Orientation != LCD_ORIENTATION_RIGHT){
                Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_RIGHT);
                Graphics_clearDisplay(&g_sContext);
            }

        }
        else if (accelBuffer[Y] < 5000) {
            if (Lcd_Orientation != LCD_ORIENTATION_UP){
                Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
                Graphics_clearDisplay(&g_sContext);
            }
        }
        else if (accelBuffer[Y] > 11300) {
            if (Lcd_Orientation != LCD_ORIENTATION_DOWN){
                Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);
                Graphics_clearDisplay(&g_sContext);
            }
        }
}
