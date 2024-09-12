  /*
 * File:   Lab02.c
 * Author: grant barbone
 *
 * Created on September 4, 2023, 10:02 AM
 */
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
// Configuration bits
#pragma config FOSC = INTIO67
oscillator block
#pragma config PLLCFG = OFF
#pragma config PRICLKEN = ON
enabled
 // look @ \Microchip\xc8\v1.38\include
  // Oscillator Selection bits->Internal
// 4X PLL Enable->Oscillator used directly
 // Primary clock enable bit->Primary clock
void initPIC(void);
void milliSecondDelay(uint16_t ms);
void microSecondDelay(uint16_t us);
void blink(char letter); //declaration
uint8_t conversion(char letter);
#define BUTTON_PIN  PORTAbits.RA2
#define BUTTON_TRIS TRISAbits.TRISA2
#define BUTTON_ANG  ANSELAbits.ANSA2
// always use "PORT" for inputs
// Make your code self-documenting
// needed for digital input
// always use "LAT" for outputs
// Make code self-documenting
// Page 135 of PIC18(L)F2X/4XK22 Data
// Page PIC18(L)F2X/4XK22 Data Sheet
#define LED_PIN
#define LED_TRIS    TRISBbits.TRISB5
#define LED_ANG     ANSELBbits.ANSB5
#define INPUT
#define OUTPUT
Sheet
1 0
#define ANALOG
#define DIGITAL     0
LATBbits.LATB5
1

  #define TIME_UNIT
#define DOT_DELAY
#define DASH_DELAY
#define INTER_LETTER_DELAY
#define INTRA_LETTER_DELAY
#define INTRA_WORD_DELAY
#define DOT
#define DASH
#define END
void main(){
    initPIC();
    char msg[]="sos help";
    uint8_t i=0;
    while (BUTTON_PIN==1);
    while (BUTTON_PIN==0);
    while (msg[i]!='\0'){
200
1*TIME_UNIT
3*TIME_UNIT
1*TIME_UNIT
3*TIME_UNIT
7*TIME_UNIT
0 1 2
} }
if (msg[i]==' '){
    milliSecondDelay(INTRA_WORD_DELAY);
}
else{
blink(msg[i]);
milliSecondDelay(INTRA_LETTER_DELAY);
} i++;

  void blink(char letter){
    uint8_t LED_ON=0;
    uint8_t LED_OFF=1;
    uint8_t index=0;
    uint8_t morseCode[26][5] = {
{DOT, DASH,
{DASH, DOT,
{DASH, DOT,
{DASH, DOT,
{DOT, END},
{DOT, DOT, DASH, DOT, END},
{DASH, DASH, DOT, END},
{DOT, DOT, DOT, DOT, END},
{DOT, DOT, END},
{DOT, DASH, DASH, DASH, END},
{DASH, DOT, DASH, END},
{DOT, DASH, DOT, DOT, END},
{DASH, DASH, END},
{DASH, DOT, END},
{DASH, DASH, DASH, END},
{DOT, DASH, DASH, DOT, END},
{DASH, DASH, DOT, DASH, END},
{DOT, DASH, DOT, END},
{DOT, DOT, DOT, END},
{DASH, END},
{DOT, DOT, DASH, END},
{DOT, DOT, DOT, DASH, END},
{DOT, DASH, DASH, END},
{DASH, DOT, DOT, DASH,END},
{DASH, DOT, DASH, DASH, END},
{DASH, DASH, DOT, DOT, END}
//a //b //c //d //e //f //g //h //i //j //k //l //m //n //o //p //q //r //s //t //u //v //w //x //y
// z
END},
DOT, DOT, END},
DASH, DOT, END},
DOT, END},
};
while (morseCode[conversion(letter)][index]!=0){
    LED_PIN=LED_ON;
    if (morseCode[conversion(letter)][index]==1){
        milliSecondDelay(DOT_DELAY);
    }
    else if (morseCode[conversion(letter)][index]==2){

  } }
    milliSecondDelay(DASH_DELAY);
}
LED_PIN= LED_OFF;
index++;
if (morseCode[conversion(letter)][index]!=0){
    milliSecondDelay(INTER_LETTER_DELAY);
}
    void initPIC(void) {
    // ---------------Configure Oscillator------------------
    OSCCONbits.IRCF2 = 1; // Internal RC Oscillator Frequency Select bits
    OSCCONbits.IRCF1 = 1; // Set to 16Mhz
    OSCCONbits.IRCF0 = 1; //
    OSCTUNEbits.PLLEN = 1; // enable the 4xPLL, wicked fast 64Mhz
    BUTTON_ANG = DIGITAL; // Must do for any input which is multiplex with ADC
    BUTTON_TRIS = INPUT; // initialize DDR bit makes push button an input
    LED_ANG = DIGITAL; // Not really needed because LED is an output
    LED_TRIS = OUTPUT; // initialize DDR bit makes LED an output
} // end initPIC
//*****************************************************************
// Call microseconds(1000) a number of times
//*****************************************************************
void milliSecondDelay(uint16_t ms) {
    uint16_t i;
    for (i = 0; i < ms; i++) microSecondDelay(1000);
} // end millSecond

  //*****************************************************************
// Create a delay of 1uS and loop a number of times
//*****************************************************************
void microSecondDelay(uint16_t us) {
    uint16_t i;
    for (i = 0; i < us; i++) {
        asm("NOP"); // 1
        asm("NOP"); // 2
        asm("NOP"); // 3
        asm("NOP"); // 4
        asm("NOP"); // 5
        asm("NOP"); // 6
        i = i;
    } // end for
}
uint8_t conversion(char letter){
    uint8_t number=letter-'a';
    return number;
}
// "morseCode" letter array stores dot/dash patterns for all 26 letters.
