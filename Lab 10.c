 //--------------------------------------------------------------------
// Name: Grant Barbone and Ethan Roberts
// Date: Fall 2020
// Purp: Lab10
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
//- Board. //------------------------------------------------------------------------ #include "mcc_generated_files/mcc.h"
#include "sdCard.h" 
#pragma warning disable 520
#pragma warning disable 1498
void myTMR0ISR(void);
#define SINE_WAVE 26 #define MAX_BLOCKS 128 #define RED 0
#define BLUE 1
#define RANDOM_INDEX 3 #define FORWARD 0 #define BACKWARDS 1 #define BLOCK_SIZE 512 #define RATE 1600
typedef enum {
BEING_FILLED, FILLED, BEING_EMPTIED, EMPTY
} myBufferState_t;
typedef struct { myBufferState_t state;
uint8_t buffer[BLOCK_SIZE];
// warning: (520) function "xyz" is never called 3 // fputc.c:16:: warning: (1498) pointer (unknown)

 } sdCardBlock_t;
sdCardBlock_t redBuffer, blueBuffer; uint16_t volumeAdjust=400;
uint8_t direction = FORWARD; uint8_t writeFlag=false;
uint8_t readFlag= false;
// Large arrays need to be defined as global even though you may only need to // use them in main. This quirk will be important in the next two assignments.
//---------------------------------------------- // Main "function" //----------------------------------------------
void main(void) {
const uint8_t sin[SINE_WAVE] = {128, 159, 187, 213, 233, 248, 255, 255,
248, 233, 213, 187, 159, 128, 97, 69, 43, 23, 8, 1, 1, 8, 23, 43, 69, 97};
uint8_t status;
uint16_t index;
uint16_t blockCounter;
uint16_t setBlocks= 0;
uint32_t sdCardAddress = 0x00000000; uint16_t sinCounter=0;
uint32_t loopSDcardAddress=0;
char cmd;
SYSTEM_Initialize(); CS_SetHigh();
// Provide Baud rate generator time to stabilize before splash screen TMR0_WriteTimer(0x0000);
INTCONbits.TMR0IF = 0;
while (INTCONbits.TMR0IF == 0);
TMR0_SetInterruptHandler(myTMR0ISR);

 INTERRUPT_GlobalInterruptEnable(); INTERRUPT_PeripheralInterruptEnable();
printf("Lab 10\r\n");
printf("Dev'21\r\n");
printf("Attached jumper over RC2/LPFin header.\r\n"); printf("Attached jumper over LPFout/AMPin header.\r\n"); printf("Attached speaker to green screw terminal.\r\n");
for (;;) {
if (EUSART1_DataReady) { // wait for incoming data on USART cmd = EUSART1_Read();
switch (cmd) { // and do what it tells you to do
//--------------------------------------------
// Reply with help menu //--------------------------------------------
case '?': printf("\r\n-------------------------------------------------\r\n"); printf("SD card address: ");
printf("%04x", sdCardAddress >> 16);
printf(":");
printf("%04x", sdCardAddress & 0X0000FFFF); printf("\r\n"); printf("-------------------------------------------------\r\n"); printf("?: help menu\r\n");
printf("o: k\r\n");
printf("Z: Reset processor\r\n");
printf("z: Clear the terminal\r\n");
printf("----------------SPI TEST-------------------------\r\n"); printf("--------------SD CARD TESTS----------------------\r\n"); printf("i: Initialize SD card\r\n");
printf("a/A decrease/increase read address\r\n");
printf("r: read a block of 512 bytes from SD card\r\n"); printf("1: write perfect 26 value sine wave to 128 blocks\r\n"); printf("s: spool memory to a csv file");
printf("+/-: Increase/Decrease the sample rate by 10 us"); printf("P: Play from SD card to PWM -> LPF -> Audio"); printf("W: Write microphone => SD card "); printf("-------------------------------------------------\r\n");

 break;
//--------------------------------------------
// Reply with "k", used for PC to PIC test //--------------------------------------------
case 'o':
printf("o: ok\r\n");
break;
//--------------------------------------------
// Reset the processor after clearing the terminal //--------------------------------------------
case 'Z':
for (int i = 0; i < 40; i++) printf("\n");
RESET();
break;
//-------------------------------------------- // Clear the terminal //-------------------------------------------- case 'z':
for (int i = 0; i < 40; i++) printf("\n"); break;
//-------------------------------------------- // Clear the terminal //--------------------------------------------
case 'i':
readFlag=false; writeFlag=false; SPI2_Close(); SPI2_Open(SPI2_DEFAULT);
communication SDCARD_Initialize(true);
break;
// Reset the SPI channel for SD card
//--------------------------------------------
// Increase or decrease block address //-------------------------------------------- case 'A':
case 'a':
if (cmd == 'a') {

 sdCardAddress -= BLOCK_SIZE;
if (sdCardAddress >= 0x04000000) {
printf("Underflowed to high address\r\n");
sdCardAddress = 0x04000000 - BLOCK_SIZE; } else {
printf("Decreased address\r\n"); }
} else {
sdCardAddress += BLOCK_SIZE;
if (sdCardAddress >= 0x04000000) {
printf("Overflowed to low address\r\n");
sdCardAddress = 0x00000000; } else {
printf("Increased address\r\n"); }
}
// 32-bit integers need printed as a pair of 16-bit integers printf("SD card address: ");
printf("%04x", sdCardAddress >> 16);
printf(":");
printf("%04x", sdCardAddress & 0X0000FFFF); printf("\r\n");
break;
//--------------------------------------------
// r: read a block of BLOCK_SIZE bytes from SD card //--------------------------------------------
case 'r':
READ_TIME_PIN_SetHigh(); SDCARD_ReadBlock(sdCardAddress, redBuffer.buffer); READ_TIME_PIN_SetLow();
printf("Read block: \r\n");
printf(" Address: ");
printf("%04x", sdCardAddress >> 16);
printf(":");
printf("%04x", sdCardAddress & 0X0000FFFF);
printf("\r\n");
hexDumpBuffer(redBuffer.buffer);
break;
case '1':
loopSDcardAddress=0;
sinCounter=0;
for (blockCounter = 0; blockCounter < MAX_BLOCKS; blockCounter++) { //printf("its is working");

 for (int i =0; i< BLOCK_SIZE; i++){ redBuffer.buffer[i]=sin[sinCounter]; sinCounter++;
if (sinCounter==SINE_WAVE){ sinCounter=0;
} }
SDCARD_WriteBlock(loopSDcardAddress, redBuffer.buffer);
while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE); loopSDcardAddress+=BLOCK_SIZE;
TMR1_WriteTimer(0x0000);
PIR1bits.TMR1IF = 0;
while (PIR1bits.TMR1IF == 0);
}
printf("The program wrote %d to the SdCard", MAX_BLOCKS);
// end for blockCount break;
name\r\n");
case 's':
printf("You may terminate spooling at anytime with a keypress.\r\n"); printf("To spool terminal contents into a file follow these instructions:\r\n"); printf("Right mouse click on the upper left of the PuTTY window\r\n"); printf("Select: Change settings...\r\n");
printf("Select: Logging\r\n");
printf("Select: Session logging: All session output\r\n");
printf("Log file name: Browse and provide a .csv extension to your file
printf("Select: Apply\r\n");
printf("Press any key to start\r\n");
while(EUSART1_DataReady==0);
(void) EUSART1_Read();
blockCounter=0;
loopSDcardAddress = 0;
while (EUSART1_DataReady ==0 && blockCounter <= MAX_BLOCKS ) { SDCARD_WriteBlock(loopSDcardAddress, redBuffer.buffer); loopSDcardAddress+=BLOCK_SIZE;
blockCounter+=1;
for (int i =0; i < BLOCK_SIZE; i ++){
printf("%d\r\n", redBuffer.buffer[i]); }
}
(void) EUSART1_Read();

 printf("Spooled %d out of the 512 blocks.", blockCounter); printf("To close the file follow these instructions:");
printf("Right mouse click on the upper left of the PuTTY window"); printf("Select: Change settings...");
printf("Select: Logging");
printf("Select: Session logging: None");
printf("Select: Apply"); break;
case '+':
volumeAdjust -= 16; break;
case '-':
volumeAdjust += 16; break;
case 'P': direction=FORWARD; loopSDcardAddress=0;
SDCARD_ReadBlock(loopSDcardAddress, redBuffer.buffer); loopSDcardAddress+=512;
readFlag=true;
break;
case 'W':
printf("Press any key to start recording of audio\r\n"); while (EUSART1_DataReady == false);
(void) EUSART1_Read();
loopSDcardAddress = 0;
//Added TMR1 with 1:1 prescaler and no interrupts to // create the needed delays.
uint16_t redIndex = 0;
uint16_t blueIndex = 0;
printf("Press any key to stop recording of audio\r\n"); TMR1_WriteTimer(0x0000);
while (EUSART1_DataReady == false) {
for (redIndex = 0; redIndex < BLOCK_SIZE; redIndex++) { ADCON0bits.GO_NOT_DONE = 1;
TMR1_WriteTimer(0x10000 - volumeAdjust + TMR1_ReadTimer());

(setBlocks);
PIR1bits.TMR1IF = 0;
while (!TMR1_HasOverflowOccured()); redBuffer.buffer[redIndex] = ADRESH;
}
SDCARD_WriteBlock(loopSDcardAddress, redBuffer.buffer);
while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE); loopSDcardAddress += BLOCK_SIZE;
for (blueIndex = 0; blueIndex < BLOCK_SIZE; blueIndex++) { TMR1_WriteTimer(0x10000 - volumeAdjust + TMR1_ReadTimer()); PIR1bits.TMR1IF = 0;
while (!TMR1_HasOverflowOccured());
blueBuffer.buffer[blueIndex] = ADRESH; }
SDCARD_WriteBlock(loopSDcardAddress, blueBuffer.buffer);
while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE); loopSDcardAddress += BLOCK_SIZE;
}
(void) EUSART1_Read();
setBlocks = loopSDcardAddress >> BLOCK_SIZE;
printf("Just wrote %d blocks to SD card, each page filled with audio data.\r\n", )
break; //--------------------------------------------
// If something unknown is hit, tell user //-------------------------------------------- default:
printf("Unknown key %c\r\n", cmd); break;
} // end switch
} // end if
} // end while } // end main
//----------------------------------------------
// As configured, we are hoping to get a toggle
// every 100us - this will require some work.
//
// You will be starting an ADC conversion here and

 // storing the results (when you reenter) into a global
// variable and setting a flag, alerting main that
// it can read a new value. //
// !!!MAKE SURE THAT TMR0 has 0 TIMER PERIOD in MCC!!!! //----------------------------------------------
void myTMR0ISR(void) { static uint8_t index=0;
static uint8_t color;
uint8_t adcValue; adcValue=ADRESH; ADCON0bits.GO_NOT_DONE=1; if (readFlag==true){
if ((color==RED) && redBuffer.state==BEING_EMPTIED){ EPWM1_LoadDutyValue(redBuffer.buffer);
if (direction==FORWARD){
index+=1;
}
if (direction==BACKWARDS){
index-=1; }
}
if(index>=BLOCK_SIZE){
redBuffer.state=EMPTY;
color=BLUE;
index=0;
}
}
if (writeFlag==true){
if ((color==RED) && redBuffer.state==BEING_FILLED){
redBuffer.buffer[index]=adcValue;
index+=1; }
if(index>=BLOCK_SIZE){ redBuffer.state=FILLED;
color=BLUE;
index=0; }
} TEST_PIN_SetHigh();

 TMR0_WriteTimer(TMR0_ReadTimer() + (0x10000 - RATE)); // More accurate
INTCONbits.TMR0IF = 0; TEST_PIN_SetLow();
}
/* end of file */
