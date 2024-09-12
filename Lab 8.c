 //--------------------------------------------------------------------
// Name: Grant Barbone
// Date: Fall 2020
// Purp: Lab08
//
// Assisted: The entire class of EENG 383
// Assisted by: Microchips 18F26K22 Tech Docs //-
//- Academic Integrity Statement: I certify that, while others may have
//- assisted me in brain storming, debugging and validating this program,
//- the program itself is my own work. I understand that submitting code
//- which is the work of other individuals is a violation of the course
//- Academic Integrity Policy and may result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board. I also understand that if I knowingly give my original work to
//- another individual that it could also result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board. //------------------------------------------------------------------------ #include <pic18f25k22.h>
#include "mcc_generated_files/mcc.h"
#pragma warning disable 520 #pragma warning disable 1498
#define PERIOD 25
#define FREQUENCY 10 #define ISR_COUNTS
#define NUM_SAMPLE 512 #define THRESHOLD_MAX 138 #define THRESHOLD_MIN 118
static uint16_t bufferIndex=0; static bool fillBuffer= false;
// warning: (520) function "xyz" is never called 3 // fputc.c:16:: warning: (1498) pointer (unknown)
uint16_t periodSamples;
uint16_t crossingCount=0;
uint8_t adc_reading[NUM_SAMPLE]; uint8_t threshold =128;
uint8_t NEW_SAMPLE = false;

 void INIT_PIC(void); void myTMR0ISR(void);
//---------------------------------------------- // Main "function" //----------------------------------------------
void main(void) 
uint8_t i;
char cmd;
uint16_t pastReading; bool direction=0; uint16_t crossIndex=0; uint16_t sum=0; uint16_t period=0;
SYSTEM_Initialize();
TMR0_WriteTimer(0x0000); INTCONbits.TMR0IF = 0;
while (INTCONbits.TMR0IF == 0);
TMR0_SetInterruptHandler(myTMR0ISR); INTERRUPT_GlobalInterruptEnable(); INTERRUPT_PeripheralInterruptEnable();
uint16_t indicesCrossed[NUM_SAMPLE>>3]; printf("Lab 08\r\n");
printf("Microphone experiments\r\n"); printf("Dev'21\r\n");
printf("> ");
for (;;) {
if (EUSART1_DataReady) { // wait for incoming data on USART cmd = EUSART1_Read();
switch (cmd) { // and do what it tells you to do
case '?': printf("------------------------------\r\n");

 printf("?: Help menu\r\n");
printf("o: k\r\n");
printf("Z: Reset processor\r\n");
printf("z: Clear the terminal\r\n");
printf("T/t: Increase/decrease threshold 138 - 118\r\n");
printf("f: gather 512 samples from the microphone and calculate the
frequency\r\n"); printf("------------------------------\r\n");
break;
//--------------------------------------------
// Reply with "k", used for PC to PIC test //--------------------------------------------
case 'o':
printf(" k\r\n>");
break;
//--------------------------------------------
// Reset the processor after clearing the terminal //--------------------------------------------
case 'Z':
for (i = 0; i < 40; i++) printf("\n");
RESET();
break;
//-------------------------------------------- // Clear the terminal //-------------------------------------------- case 'z':
for (i = 0; i < 40; i++) printf("\n"); break;
//--------------------------------------------
// Continue to collect samples until the user
// presses a key on the keyboard //--------------------------------------------
case'T':
if (threshold < THRESHOLD_MAX){
threshold+=5;
printf("The current frequency is at %d", threshold );
printf("\r\n");
}
else {
printf("You have reached the maximum threshold frequency.\r\n");

 }
break;
case 't':
if (threshold > THRESHOLD_MIN){
threshold-=5;
printf("The current frequency is at %d", threshold); printf("\r\n");
}
else {
printf("You have reached the min threshold frequency.\r\n"); }
break;
case 'f':
fillBuffer=true;
while (fillBuffer);
printf("The lest 512 ADC samples fro the microphone are :\r\n");
for (int i =0 ; i < NUM_SAMPLE; i ++){
if (i%16==0){ printf("\r\nS[%d] ", i);
}
printf("%d ", adc_reading[i]);
}
printf("r\n");
printf("\r\nThe sound wave crossed at the following indicies\r\n"); pastReading=adc_reading[0];
//crossIndex=0;
for (int i =1; i < NUM_SAMPLE; i++){
if (adc_reading[i]>pastReading){ direction=1;
} else {
direction=0; }
if (pastReading>128 && direction ==0){ if (adc_reading[i]<=128){
indicesCrossed[crossIndex]=i; printf("%d ",i );

 crossIndex++;
} }
else if (pastReading<128 && direction ==1){ if (adc_reading[i]>=128){
indicesCrossed[crossIndex]=i; printf("%d ",i );
crossIndex++;
}
}
pastReading=adc_reading[i];
}
printf("\r\n\r\nThe sound wave had %d periods\r\n", crossIndex); for (int i =0; i < crossIndex-1; i++){ sum+=indicesCrossed[i+1]-indicesCrossed[i];
printf("%d - %d= %d\r\n",
indicesCrossed[i],indicesCrossed[i+1],indicesCrossed[i+1]-indicesCrossed[i]); }
period=sum/(crossIndex-1);
printf("average period = %d us \r\n", period);
printf("average frequency = %d Hz\r\n", ((crossIndex-1)*FREQUENCY)/sum); break;
} // end switch } // end if
} // end while
} // end main
typedef enum {MIC_IDLE, MIC_WAIT_FOR_TRIGGER, MIC_ACQUIRE} myTMR0states_t; void myTMR0ISR(void) {
static myTMR0states_t myTMR0state=MIC_IDLE; static uint16_t bufferIndex=0;
uint8_t microphone=ADRESH; ADCON0bits.GO_NOT_DONE = 1;
switch (myTMR0state){ case MIC_IDLE:

 if (fillBuffer==true){
myTMR0state=MIC_WAIT_FOR_TRIGGER; }
break;
case MIC_ACQUIRE:
bufferIndex++; adc_reading[bufferIndex]=microphone; if (bufferIndex==NUM_SAMPLE){
myTMR0state=MIC_IDLE;
fillBuffer=false; }
break;
case MIC_WAIT_FOR_TRIGGER:
if (microphone> THRESHOLD_MAX || microphone< THRESHOLD_MIN ){ myTMR0state=MIC_ACQUIRE;
bufferIndex=0;
} break; default:
myTMR0state=MIC_IDLE;
break;
}
TEST_PIN_SetHigh(); // Set high when every we start a new conversion ADCON0bits.GO_NOT_DONE = 1; // start a new conversion NEW_SAMPLE = true; // tell main that we have a new value TMR0_WriteTimer(0x10000 + 111);
TEST_PIN_SetLow(); // Monitor pulse width to determine how long we are in ISR
} // end myTMR0ISR
