 /*
* File: Lab5.c
* Author: grantbarbone *
* Created on October 2, 2023, 1:31 PM */
#include "mcc_generated_files/mcc.h" #include <inttypes.h>
#pragma warning disable 520 #pragma warning disable 1498
#define DUTY_INC 0x10 #define NUM_COLOR 6 uint8_t colorTour=false; //flag
// warning: (520) function "xyz" is never called
// fputc.c:16:: warning: (1498) pointer (unknown)
uint16_t deltaRed[] = {0x0000, 0x0000, 0x0001, 0x0000,0x0000, 0xFFFF}; uint16_t deltaGreen[] = {0x0001, 0x0000, 0x0000, 0xFFFF,0x0000, 0x0000}; uint16_t deltaBlue[] = {0x0000, 0xFFFF, 0x0000, 0x0000,0x0001, 0x0000};
// Y-> r-> f-> b-> t-> g->
uint16_t initRED[] = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF}; uint16_t initGREEN[] = {0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00}; uint16_t initBLUE[] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF}; uint16_t duration= 900;
void myTMR0ISR(void); // color cycle
void main(void) SYSTEM_Initialize(); char cmd;
uint16_t redDuty=0x00FF; uint16_t greenDuty = 0x00FF; uint16_t blueDuty=0x00FF; uint8_t i;
//TMR2_WriteTimer(0x0000); PIR1bits.TMR2IF = 0; while(PIR1bits.TMR1IF == false);
TMR0_SetInterruptHandler(myTMR0ISR);

 INTERRUPT_GlobalInterruptEnable(); // ISR not working? - you probably INTERRUPT_PeripheralInterruptEnable(); // forgot to add these 2 line printf("------------------------------\r\n");
printf(" Red: 0x3f\r\n");
printf(" Green: 0x3f\r\n");
printf(" Blue: 0x3f\r\n"); printf("------------------------------\r\n"); EPWM1_LoadDutyValue(redDuty); EPWM2_LoadDutyValue(greenDuty); EPWM3_LoadDutyValue(blueDuty); for (;;){
if (EUSART1_DataReady){
cmd = EUSART1_Read();
switch (cmd) { // and do what it tells you to do case '?':
printf("------------------------------\r\n");
printf("?: Help menu\r\n");
printf("Z: Reset processor\r\n");
printf("z: Clear the terminal\r\n");
printf("R/r: increase/decrease Red intensity \r\n"); printf("G/g: increase/decrease Green intensity \r\n"); printf("B/b: increase/decrease Blue intensity \r\n"); printf("C/c: start/stop color cycle \r\n");
printf("a: All LEDs off \r\n");
printf("+/-: increase/decrease the color tour speed."); printf("------------------------------\r\n");
break;
case 'R':
if (redDuty <=(0xFF- DUTY_INC)){ redDuty += DUTY_INC;
}
colorTour=false; EPWM1_LoadDutyValue(redDuty); break;
case 'r':
if (redDuty >=(0xFF-DUTY_INC)){ redDuty -=DUTY_INC;
}
colorTour=false; EPWM1_LoadDutyValue(redDuty); break;
case'G':
if (greenDuty <=(0xFF-DUTY_INC)){ greenDuty +=DUTY_INC;

 }
colorTour=false; EPWM2_LoadDutyValue(greenDuty); break;
case'g':
if (greenDuty >=(0xFF-DUTY_INC)){ greenDuty -=DUTY_INC;
}
colorTour=false; EPWM2_LoadDutyValue(greenDuty); break;
case'B':
if (blueDuty <=(0xFF-DUTY_INC)){ blueDuty +=DUTY_INC;
}
colorTour=false; EPWM3_LoadDutyValue(blueDuty); break;
case'b':
if (blueDuty >=(0xFF-DUTY_INC)){ blueDuty -=DUTY_INC;
}
colorTour=false; EPWM3_LoadDutyValue(blueDuty); break;
case 'C': colorTour=true;
break; case 'c':
colorTour=false;
break; case 'a':
redDuty=0xFF;
greenDuty=0xFF;
blueDuty=0xFF; EPWM1_LoadDutyValue(redDuty); EPWM2_LoadDutyValue(greenDuty); EPWM3_LoadDutyValue(blueDuty); break;
case '+':
for(int i =0; i< 200; i++){
duration=12+duration; }

 break;
//--------------------------------------------
// Reset the processor after clearing the terminal //--------------------------------------------
case '-':
for(int i =0; i< 200; i++){
duration=-12+duration; }
break; case 'Z':
for (int i = 0; i < 40; i++) printf("\n"); RESET();
break;
//-------------------------------------------- // Clear the terminal //--------------------------------------------
case 'z':
for (int i = 0; i < 40; i++) printf("\n"); break;
//--------------------------------------------
// If something unknown is hit, tell user //--------------------------------------------
default:
printf("Unknown key %c\r\n", cmd); break;
//local variables }
}
}
}
void myTMR0ISR(void){
static uint8_t index=0;
static uint8_t step=0; //0-255
static uint16_t redDuty, greenDuty, blueDuty;
redDuty=initRED[index]; greenDuty=initGREEN[index]; blueDuty=initBLUE[index];

 if (colorTour==true){
redDuty += deltaRed[index]; greenDuty += deltaGreen[index]; blueDuty += deltaBlue[index]; EPWM1_LoadDutyValue(redDuty); EPWM2_LoadDutyValue(greenDuty); EPWM3_LoadDutyValue(blueDuty); step++;
if (step==255){ step=0;
index+=1;
if (index >= NUM_COLOR) { index=0;
}
redDuty=initRED[index]; greenDuty=initGREEN[index]; blueDuty=initBLUE[index];
} }
else{ //colorTour= false index=0;
step=0; redDuty=initRED[index]; greenDuty=initGREEN[index]; blueDuty=initBLUE[index];
}
TMR0_WriteTimer(0x10000-duration); INTCONbits.TMR0IF = 0;
}
