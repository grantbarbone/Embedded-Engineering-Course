 /**
Generated Main Source File
Company:
Microchip Technology Inc.
Name: Grant Barbone File Name:
main.c
Summary:
This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs
Description:
This header file provides implementations for driver APIs for all modules selected in the
GUI.
Generation Information :
Product Revision : PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7 Device : PIC18F25K22
Driver Version : 2.00
*/
/*
(c) 2018 Microchip Technology Inc. and its subsidiaries.
Subject to your compliance with these terms, you may use Microchip software and any
derivatives exclusively with Microchip products. It is your responsibility to comply with third party
license terms applicable to your use of third party software (including open source software) that
may accompany Microchip software.
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
FOR A PARTICULAR PURPOSE.
IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND

 WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
SOFTWARE. */
#include "mcc_generated_files/mcc.h" /*
Main application
*/
#define MAX_BUFFER_SIZE 32 #define LED_ON 25 #define LED_OFF 0 #pragma warning disable 520 #pragma warning disable 1498
uint8_t i, checkSum;
typedef enum event2State_t{RX_IDLE, RX_DATA_BYTES, RX_CS}; char IRTransmitBuffer[MAX_BUFFER_SIZE] = "";
char IRrecieveBuffer[MAX_BUFFER_SIZE] = "";
uint16_t bitPeriod[6] = {53333, 13333, 6666, 3333, 1666, 833};
bool recieveNewMessage;
//flags
void myEUSART2ISR(void);
uint8_t baudRateSelected = 1;
uint8_t receiveBusy = false;
uint8_t receiveNewMessage = false;
uint8_t mask;
void myTMR1ISR(void);
uint8_t transmitStart=false;
uint8_t transmitBusy=false;
uint16_t bitPeriodInTMR1Counts=26666; //1200 Baud default uint8_t transmitSourceAddress=1;
uint8_t transmitDestAddress=255;
char letter=' ';
char cmd;

 uint8_t a=0;
uint8_t userEnter8bit(void);
void decodeIntoASCII(char msg[]); /*
Main application
*/
void main(void) {
SYSTEM_Initialize();
EUSART2_SetRxInterruptHandler(myEUSART2ISR); TMR1_SetInterruptHandler(myTMR1ISR); INTERRUPT_GlobalInterruptEnable(); INTERRUPT_PeripheralInterruptEnable();
//TMR0_WriteTimer(0xD000);
while(1){
if (EUSART1_DataReady){ cmd=EUSART1_Read(); switch(cmd){
case '?':
printf("-------------------------------------------------\r\n"); printf("TX msg: This is a test\r\n");
printf("My identity: 1");
printf("TX address: ff"); printf("receiveNewMessage = false"); printf("2400 Baud"); printf("-------------------------------------------------\r\n"); printf("?: help menu\r\n");
printf("o: k\r\n");
printf("Z: Reset processor\r\n");
printf("z: Clear the terminal\r\n");
printf("b: set the Baud rate\r\n");
printf("m: create a NULL terminated message with SRC and DEST prefix\r\n"); printf("s: set Source transmit identity\r\n");
printf("d: set Destination transmit target\r\n");
printf("S: Send message in TM1 ISR\r\n");
printf("R: Receive message using EUSART2 via IR decoder\r\n");
printf("M: Monitor all IR traffic\r\n.");

 printf("x/X: decode tx/RX message.\r\n"); printf("-------------------------------------------------\r\n"); break;
case 'o':
printf("o: ok\r\n");
break; case 'Z':
for (i = 0; i < 40; i++) printf("\n"); RESET();
break;
case 'z':
for (i = 0; i < 40; i++) printf("\n"); break;
case 'b': //ask about baud rate
printf("Choose the index of the target baud rate\r\n"); printf("0: 300 baud\r\n");
printf("1: 1200 baud\r\n");
printf("2: 2400 baud\r\n");
printf("3: 4800 baud\r\n");
printf("4: 9600 baud\r\n");
printf("5: 19200 baud\r\n");
while (EUSART1_DataReady);
baudRateSelected = EUSART1_Read() - '0';
switch (baudRateSelected) {
case 0: SPBRGH2 = 0xD0;
SPBRG2 = 0x54;
break;
case 1: SPBRGH2 = 0x34;
SPBRG2 = 0x14;
break;
case 2: SPBRGH2 = 0x1A;
SPBRG2 = 0x0A;
break;
case 3: SPBRGH2 = 0x0D;
SPBRG2 = 0x05;
break;
case 4: SPBRGH2 = 0x06;
SPBRG2 = 0x82;
break;
case 5: SPBRGH2 = 0x03;
SPBRG2 = 0x40;
break;
default: SPBRGH2 = 0x34;
SPBRG2 = 0x14;

 break;
}
printf("Baud rate assigned %02x:%02x\r\n", SPBRGH2, SPBRG2); bitPeriodInTMR1Counts=bitPeriod[baudRateSelected];
break;
case 's':
printf("input the an address: ");
transmitSourceAddress=userEnter8bit();
printf("\r\nEnter the transmitSourceAddress: %d", transmitSourceAddress); printf("\r\n");
break;
case 'd' :
printf("input the an address: ");
transmitSourceAddress=userEnter8bit();
printf("\r\nEnter the transmitDestinationAddress: %d", transmitSourceAddress); printf("\r\n");
break;
case 'S': transmitStart=true;
//myTMR1ISR();
//printf("check one");
while (transmitStart==true);
//printf("check two");
while (transmitBusy==true);
//printf("check three");
// Since EUSART2 is double buffered, clear out any garbage
transmitStart = true; // Tell the TMR1 ISR to TX uint8_t rxBufferIndex = 0;
// do{
// while (PIR3bits.RC2IF == 0);
// IRrecieveBuffer[rxBufferIndex] = RCREG2;
// } while (IRrecieveBuffer[rxBufferIndex++] != '\0');
// while (PIR3bits.RC2IF == 0);
// IRrecieveBuffer[rxBufferIndex] = RCREG2;
// receiveNewMessage = true;
printf("Transmitted\r\n");
printf(" Message: %s\r\n", IRTransmitBuffer + 2); printf(" Checksum computed: %d\r\n", checkSum);
printf(" Sender address: %d\r\n", transmitSourceAddress); printf(" Target address: %d\r\n", transmitDestAddress);

 break; case 'R':
a=0;
if (receiveNewMessage==true){
printf("Received\r\n");
printf(" Message: ");
while(IRrecieveBuffer[a++]!= '\0' ) {
printf("%c", IRrecieveBuffer[a]); checkSum+=IRrecieveBuffer[a];
}
printf("Checksum computed: %d\r\n" );
printf("\r\n Checksum: %d\r\n", checkSum); printf("Checksum received: %d\r\n" );
printf(" Sender address: %d\r\n", transmitSourceAddress); printf(" Target address: %d\r\n", transmitDestAddress); receiveNewMessage=false;
}
else{
printf("No message, recieveNewMessage = false");
}
break;
case 'm':
IRTransmitBuffer[0]=transmitSourceAddress; checkSum=transmitSourceAddress; IRTransmitBuffer[1]=transmitSourceAddress; checkSum+=transmitSourceAddress;
i=2;
checkSum =0;
letter=' ';
printf("Enter message, hit return when done.\r\n");
while(EUSART1_DataReady);
while((i < MAX_BUFFER_SIZE-1) && ((cmd=EUSART1_Read())!='\r')){ IRTransmitBuffer[i++]=cmd;
checkSum+=cmd;
printf("%c", cmd);
}
printf("\r\nCreated\r\n");
printf(" Message: %s", IRTransmitBuffer); for (int z=2;IRTransmitBuffer[z]=='\0'; i++ ){ printf("%c", IRTransmitBuffer[z]);

 }
printf("\r\nChecksum: %d \r\n", checkSum); IRTransmitBuffer[i+1]='\0'; IRTransmitBuffer[i+2]= checkSum;
break;
case 'M':
printf("Hit any key to exit\r\n"); printf("SRC DEST CHECK MESSAGE"); while(!(EUSART1_DataReady)){ if(receiveNewMessage==true){ recieveNewMessage=false;
i=2;
printf("%d", IRrecieveBuffer[0] ); printf("%d", IRrecieveBuffer[1]); while(IRrecieveBuffer[i]!='\0'){
i++; }
printf(" %d ", IRrecieveBuffer[i+1]); i=2;
while(IRrecieveBuffer[i]!='\0'){
printf("%c",IRrecieveBuffer[i++]); }
printf("\r\n"); }
}
break;
case 'x':
printf("TX Buffer Contents:\r\n"); decodeIntoASCII(IRTransmitBuffer); break;
case 'X':
printf("RX Buffer Contents:\r\n"); decodeIntoASCII(IRrecieveBuffer);
} //switch statement } //is statement
} //while loop
} //end of function
void myEUSART2ISR(void){
static enum event2State_t event2State = RX_IDLE; static uint8_t index=0;

 switch (event2State) {
case RX_IDLE :
index=0; IRrecieveBuffer[index++]=RCREG2; receiveBusy=true;
event2State = RX_DATA_BYTES;
break;
case RX_DATA_BYTES: if(((IRrecieveBuffer[index++]=RCREG2))=='\0'){
event2State = RX_CS; }
break;
case RX_CS:
IRrecieveBuffer[index]= RCREG2; // leading Cs receiveBusy=false;
recieveNewMessage =true; // using in Case ?R? in main() event2State=RX_IDLE;
break;
}
PIR3bits.RC2IF=0;
}
typedef enum {
TX_IDLE, TX_DATA_BITS, TX_START_BIT, TX_STOP_BIT
} myTXstates_t;
void myTMR1ISR(void) {
static uint8_t mask = 0b00000001;
static char letter= ' ';
static myTXstates_t tmr1ISRstate=TX_IDLE;
static uint8_t index2=0;
static bool checkSumSent=false;
switch (tmr1ISRstate) {
//---------------------------------
// If you are here, it means that you've been inactive //---------------------------------
case TX_IDLE:
if (transmitStart == true) {
//printf("We are in the idle case"); transmitBusy=true; transmitStart=false;

 tmr1ISRstate = TX_START_BIT; index2=0; checkSumSent=false;
} else{
EPWM2_LoadDutyValue(LED_OFF); }
break;
case TX_START_BIT:
//printf("We are in the in if statement case \r\n"); // printf("We are in the start case"); EPWM2_LoadDutyValue(LED_ON); tmr1ISRstate = TX_DATA_BITS;
mask = 0b00000001; letter=IRTransmitBuffer[index2];
break;
case TX_DATA_BITS:
//printf("We are in the data case");
if (mask == 0) {
tmr1ISRstate = TX_STOP_BIT; EPWM2_LoadDutyValue(LED_OFF); //transmitBusy=false;
} else {
}
break;
case TX_STOP_BIT:
//printf("We are in the stop case"); EPWM2_LoadDutyValue(LED_OFF); if (checkSumSent==true){
checkSumSent=false; tmr1ISRstate = TX_IDLE; transmitStart=false;
}
else if (IRTransmitBuffer[index2] == '\0'){
checkSumSent=true;
tmr1ISRstate =TX_START_BIT; }
else {
tmr1ISRstate =TX_START_BIT; transmitBusy=false;
if ((letter & mask) != 0) EPWM2_LoadDutyValue(LED_OFF); else EPWM2_LoadDutyValue(LED_ON);
mask = mask << 1;

 }
index2++;
break; //--------------------------------- // How did you get here? //--------------------------------- }
TMR1_WriteTimer(0x10000 - bitPeriodInTMR1Counts); // 6666 = 2400 Baud
PIR1bits.TMR1IF = 0; }
uint8_t userEnter8bit(void){ uint8_t tempTransmit=0;
while((cmd=EUSART1_Read())!='\r'){
if (cmd>='0' && cmd <='9'){ tempTransmit=tempTransmit*10+(cmd-'0'); }
printf("%c", cmd);
}
return tempTransmit;
}
void decodeIntoASCII(char msg[]){
uint8_t i=0;
while (msg[i]!='\0'){
printf(" %2d: %02x %c\r\n", i++;
}
}
/**
End of File
*/
i, msg[i], msg[i]);
