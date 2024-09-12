  //*****************************************************************
// Name:
// Date:
// Lab:
// Purp: Happy Birthday
//
//Grant Barbone
//Fall 2020
//03
// Assisted: The entire EENG 383 class
// Assisted by: Technical documents
//
// Academic Integrity Statement: I certify that, while others may have
// assisted me in brain storming, debugging and validating this program,
// the program itself is my own work. I understand that submitting code
// which is the work of other individuals is a violation of the course
// Academic Integrity Policy and may result in a zero credit for the
// assignment, course failure and a report to the Academic Dishonesty
// Board. I also understand that if I knowingly give my original work to
// another individual that it could also result in a zero credit for the
// assignment, course failure and a report to the Academic Dishonesty
// Board.
//*****************************************************************
#include "mcc_generated_files/mcc.h"
#pragma warning disable 520
#pragma warning disable 1498
#define N       4       // Number of notes
#define NUM_OCTAVES 2
#define SONG_LENGTH 25
#define REST 1953
#define QUA 15625
#define HAL 31250
#define WHOLE 62500
#define C5 0
#define D5 1
#define F5 2
#define E5 3
#define G5 4
#define A5 5
#define C4 6

  #define AS5 7
#define RES 32
void microSecondDelay(uint16_t us);
void milliSecondDelay(uint16_t ms);
//*****************************************************************
//*****************************************************************
void main(void) {
    uint16_t scale[NUM_OCTAVES*12] = {7648, 6816, 5728, 6080, 5104, 9088,
15296,8592};
    uint16_t notes[SONG_LENGTH] = {C5, C5, D5, C5, F5, E5,
                                C5, C5, D5, C5, G5, F5,
                                C5, C5, C4, A5, G5, E5, D5,
                                AS5, AS5, A5, F5, G5, F5};
    uint16_t duration[SONG_LENGTH] = {QUA, QUA, QUA, QUA, QUA,  HAL,
                                     QUA,  QUA,  QUA,  QUA,  QUA,  HAL,
                                     QUA, QUA, QUA, QUA, QUA, QUA, HAL,
                                     QUA, QUA, QUA, QUA, QUA,  HAL};
    SYSTEM_Initialize();
    for (;;) {
        if (BOTTUM_BOTTUM_GetValue()==0){
            while(BOTTOM_BUTTON_GetValue()==0);
            for (int j =0; j< SONG_LENGTH; j++){
                TMR0_WriteTimer((0x10000)-duration[j]);
                INTCONbits.TMR0IF=0;
                while(TMR0_HasOverflowOcurred()==false){
                    TMR1_WriteTimer(0x10000-scale[notes[j]]);
                    PIR1bits.TMR1IF= 0;
                    while(TMR1_HasOverflowOcurred()==false){
                        SPEAKER_PIN_Toggle();
                    }
                    SPEAKER_PIN_SetLow();
                    TMR0_WriteTimer(0xF000);
                    INTCONbits.TMR0IF= 0;
                    while(TMR0_HasOverflowOcurred()==false);
}

              } // end pressed button
        }
    } // end infinite loop
} // end main
