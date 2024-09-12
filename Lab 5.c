  /**
  Generated Main Source File
  Company:
    Microchip Technology Inc.
  File Name:
    main.c
  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs
  Description:
    This header file provides implementations for driver APIs for all modules
selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F25K22
        Driver Version    :  2.00
*/
/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.
    Subject to your compliance with these terms, you may use Microchip software
and any
    derivatives exclusively with Microchip products. It is your responsibility to
comply with third party
    license terms applicable to your use of third party software (including open
source software) that
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
#include "mcc_generated_files/mcc.h"
#define N 4 // Number of notes
#define NUM_OCTAVES 2
#define SONG_LENGTH 25
#define A5 5
#define C5 0
#define D5 1
#define F5 2
#define E5 3
#define G5 4
#define C4 6
#define AS5 7
#define WHOLE 62500
#define HAL 31250
#define QUA 15625
#define REST 1953
#define MAX_SONG_LENGTH 7
#define NUM_SONGS 4
#define DELTA 10000
uint8_t playingSong= false;
uint8_t playingNote= false;
uint8_t noteIndex= 0;
uint8_t songNumber= 0;

  #define TEST_SONG 0
#define COPY_SONG 1
#define FAIL_SONG 2
#define SUCCESS_SONG 3
uint16_t scale[NUM_OCTAVES*12] = {7648, 6816, 5728, 6080, 5104, 9088,
15296,8592};
uint16_t notes[NUM_SONGS][SONG_LENGTH] = {
    {C5, C5, D5, C5, F5, E5},
    {C5, C5, D5, C5, G5, F5},
    {C5, C5, C4, A5, G5, E5, D5},
    {AS5, AS5, A5, F5, G5, F5}
};
uint16_t duration[NUM_SONGS][SONG_LENGTH] =
{{QUA, QUA, QUA, QUA, QUA, HAL},
{QUA, QUA, QUA, QUA, QUA, HAL},
{QUA, QUA, QUA, QUA, QUA, QUA, HAL},
{QUA, QUA, QUA, QUA, QUA, HAL}
};
uint8_t songLength[NUM_SONGS]= {6,6,7,6};
typedef enum tmr0ISRState_t{TMR0_IDLE,TMR0_PLAY_NOTE, TMR0_PLAY_REST};
void myTMR0ISR(void);
void myTMR1ISR(void);
void main(void)
{
    char cmd;
    bool success;
    uint8_t pressDuration;
    uint8_t prevpressTMR;
    // Initialize the device
    SYSTEM_Initialize();
    TMR1_WriteTimer(0x0000);
    PIR1bits.TMR1IF = 0;

  while (PIR1bits.TMR1IF == 0);
TMR0_SetInterruptHandler(myTMR0ISR);
TMR1_SetInterruptHandler(myTMR1ISR);
INTERRUPT_GlobalInterruptEnable(); // ISR not working? - you probably
INTERRUPT_PeripheralInterruptEnable(); // forgot to add these 2 lines
printf("inLab 04\r\n");
printf("Interrupt Music Box\r\n");
printf("\r\n");
for (;;)
{
//EUSART
    if (EUSART1_DataReady){
        cmd=EUSART1_Read();
        switch (cmd){
            case '?':
                printf("------------------------------\r\n");
                printf("?: Help menu\r\n");
                printf("o: k\r\n");
                printf("Z: Reset processor\r\n");
                printf("z: Clear the terminal\r\n");
                printf("p: play song once\r\n");
                printf("r: Rhythm practice\r\n");
                printf("------------------------------\r\n");
                break;
            case 'p':
                printf("play song once\r\n"); //print something
                songNumber=TEST_SONG;
                noteIndex=0;
                playingSong=true;
                break;
            case 'r':
                songNumber=COPY_SONG;
                noteIndex=0;
                playingSong=true;
                while(playingSong == true);
                success=true;

  this line
while(TOP_BUTTON_GetValue()==1);
//while(TOP_BUTTON_GetValue()==0);
INTCONbits.TMR0IE=0; //what does this do
noteIndex=0;
while(noteIndex< songLength[COPY_SONG] && success==true){
    if (noteIndex!=0){
        pressDuration=TMR0_ReadTimer()-prevpressTMR; // fix
        prevpressTMR= TMR0_ReadTimer();
                            if (pressDuration >
(duration[songNumber][noteIndex-1]+REST+DELTA) || pressDuration <
duration[songNumber][noteIndex-1]+REST-DELTA){
                                success=false;
recreate it
} }
    else{
        prevpressTMR=TMR0_ReadTimer();
    }
    playingNote=true;
    while(TOP_BUTTON_GetValue() == 0); //fix mcc probably
    playingNote=false;
    if(noteIndex<songLength[COPY_SONG]-1){
        while(TOP_BUTTON_GetValue()==1);
    }
    noteIndex+=1;
}
    INTCONbits.TMR0IE=1;
    if (success== true){
        songNumber=SUCCESS_SONG;
        noteIndex=0;
        playingSong=true;

  } }
} }
            while(playingSong==true);
            printf("success\r\n");
            //play success song
        }
        if (success==false){
            songNumber=FAIL_SONG;
            noteIndex=0;
            playingSong=true;
            printf("failure\r\n");
            //play fail song
}
    break;
case 'z':
    for (int i =0 ; i< 40; i++){
        printf("\n");
}
    break;
case 'Z':
    for (int i =0 ; i< 40; i++){
        printf("\n");
    }
    RESET();
    break;
case 'o':
    printf("k.\r\n");
    break;
default:
    printf("Unknown key %c\r\n", cmd);
    break;

  void myTMR0ISR(void){
    uint8_t note_index=0;
    static enum tmr0ISRState_t tmr0State = TMR0_IDLE;
    switch (tmr0State){
        case TMR0_IDLE:
            if (playingSong==true){
                 //printf("0 ");
                tmr0State=TMR0_PLAY_NOTE;
                note_index=0;
            }
            break;
        case TMR0_PLAY_NOTE:
            //printf("1 ");
            playingNote=true; //global variable
            TMR0_WriteTimer(0x10000-duration[songNumber][noteIndex]);
            tmr0State=TMR0_PLAY_REST;
            break;
        case TMR0_PLAY_REST:
            //printf("2 ");
            playingNote=false;
            TMR0_WriteTimer(0x10000-REST);
            noteIndex+=1;
           if (noteIndex> songLength[songNumber]){
               playingSong=false;
               tmr0State=TMR0_IDLE;
}
else
           {
               tmr0State=TMR0_PLAY_NOTE;
}
            break;
        default:
            printf("Oh no.\r\n");
break; }
    INTCONbits.TMR0IF=0;

  }
    void myTMR1ISR(void) {
        if (playingNote==true){
            //printf("p");
            //printf(" note %d",noteIndex);
            TMR1_WriteTimer(0x10000-scale[notes[songNumber][noteIndex]]);
            SPEAKER_PIN_Toggle();
        }
        PIR1bits.TMR1IF=0; //what does this port do again?
}
/**
 End of File
*/
