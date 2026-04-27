#pragma once
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* XC_HEADER_TEMPLATE_H */

// идентификатор версии программы по 4 бита
#pragma config IDLOC0 = 1 // тип
#pragma config IDLOC1 = 0 // номер сборки(десятки) 0-9
#pragma config IDLOC2 = 2 // номер сборки(единицы) 1-9-0
#pragma config IDLOC3 = 0 // резерв

void interrupt_tmr0 (void);
void uart_read(void);

void HandlerDMX(void);
void HandlerTimeCounter(uint8_t );
// запрос состояния флага 
bool HandlerTimeFlag (void);
// сброс флага при запросе HandlerTimeFlag())
bool handler_time_flag; 
#define _64MS   16

union DIP {
    uint16_t Dip;
    struct {
        unsigned D1 : 1;
        unsigned D2 : 1;
        unsigned D3 : 1;
        unsigned D4 : 1;
        unsigned D5 : 1;
        unsigned D6 : 1;
        unsigned D7 : 1;
        unsigned D8 : 1;
        unsigned D9 : 1;
        unsigned D10 : 1;
    };
};
void ReadDip (void);
