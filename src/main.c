#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "max7219.h"


void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
}

void display(uint8_t address, uint8_t data) {
    uint8_t mask;
    LOW(CS); // začátek přenosu

    /* pošlu adresu */
    mask = 128;
    mask = 1 << 7;
    mask = 0b10000000;
    while (mask) {
        if (address & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }
    /* pošlu data */
    mask = 0b10000000;
    while (mask) {
        if (data & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }

    HIGH(CS); // konec přenosu
}

int main(void) {

    uint32_t time1 = 0;
    uint32_t time2 = 0;
    uint8_t number1 = 0;
    uint16_t number2 = 000;
    uint16_t vysledek1;
    uint16_t vysledek2;
    uint16_t n;
    init();

    display(DECODE_MODE, 0b11111111);
    display(SCAN_LIMIT, 7);
    display(INTENSITY, 1);
    display(DISPLAY_TEST, DISPLAY_TEST_OFF);
    display(SHUTDOWN, SHUTDOWN_ON);
    display(DIGIT0, 0xF);
    display(DIGIT1, 0xF);
    display(DIGIT2, 0xF);
    display(DIGIT3, 0xF);
    display(DIGIT4, 0xF);
    display(DIGIT5, 0xF);
    display(DIGIT6, 0xF);
    display(DIGIT7, 0xF);

    while(1){

        if (milis() - time1 > 1000){
            time1 = milis();
            display(DIGIT0, number1);
            number1 += 1;

            if(number1 > 9){
                number1 = 0;
            }
            }

        if (milis() - time2 > 100) {
            time2 = milis();
            vysledek1 = number2 / 100;
            display(DIGIT6, vysledek1);
            n = number2 % 100;
            vysledek2 = n / 10;
            display(DIGIT5, vysledek2);
            n = number2 % 10;
            display(DIGIT4, n);
            number2 += 1;

            if(number2 > 999){
                number2 = 0;
            }
        }
        
    }
        
}