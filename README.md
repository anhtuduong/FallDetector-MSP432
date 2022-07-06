# FallDetector-MSP432

![Screenshot from 2022-01-27 02-28-46](https://user-images.githubusercontent.com/76017474/177648660-b7e837ab-c056-496d-a303-1f7cd7498074.png)

## Basic Working Scheme

![Screenshot from 2022-07-06 23-44-09](https://user-images.githubusercontent.com/76017474/177648785-d7fc712d-d68c-4f50-9678-50dcbad3f447.png)

### Sensors used:
- Texas Instruments **MSP432-P401R**
- Texas Instruments **BOOSTXL-EDUMKII EDUCATIONAL BOOSTERPACK**
- Texas Instruments **CC3100**

### IDE:
- Code Composer Studio
---

## Software Architecture

### Fall Detection

        3-Axis Analog Accelerometer 
        Interrupts ADC14, default clocksource (5MHz) with clock divider 8

![Screenshot from 2022-07-06 23-55-16](https://user-images.githubusercontent.com/76017474/177650223-22c84a8f-0763-4bae-8f10-9b69b37e89d0.png)
![Screenshot from 2022-07-06 23-55-59](https://user-images.githubusercontent.com/76017474/177650309-621c16d9-6b72-4150-9193-294306ca8809.png)
![Screenshot from 2022-07-06 23-56-14](https://user-images.githubusercontent.com/76017474/177650326-5136f5fd-5982-4467-ac84-d1de58e6dbe5.png)

### User Interface

        RTC_C_Calendar to get Calendar time
        ADC14 Interrupt to get LCD auto-oriented
        Timer_A0_BASE used for the buzzer
        2 buttons from Boosterpack MkII

### Wifi

        CC3100 to connect to wifi
        Services from: ifttt.com + Webhooks

---

## Testing
https://youtu.be/mXc96FsT1cM