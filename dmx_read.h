// ver 2.0
#pragma once
#include <stdbool.h>
#include <xc.h> // include processor files - each processor file is guarded.  

// аппаратная привязка
#define DMX_BUFFER_SIZE        1   //колич исполь каналов прибором 
#define BUF_RX         RCREGbits.RCREG
#define BUF_RX9        RCSTAbits.RX9D
#define UART_Rx_ON     RCSTAbits.CREN = 1
#define UART_Rx_OFF    RCSTAbits.CREN = 0 
#define UART_FERR      RCSTAbits.FERR
#define UART_OVERFLOW  RCSTAbits.OERR
#define DMX_Dir        RC6
#define DRV_RECEIVE    DMX_Dir = 0
#define DRV_TRANSMIT   DMX_Dir = 1


// Personal_Descript , Footprint_Personality define in *.c
#define Total_Personality   1  // общее кол-во профилей dmx
typedef enum Person{
    direct
} person_t;

struct DMX{    
    unsigned break_ : 1;
    unsigned error : 1;
    unsigned flag_received : 1;   //флаг приема пакета, сброс в BREAK и при чтении        
    unsigned start_ : 2;
    
    uint16_t start_address;    // 1-512 !    
    uint16_t new_address;    
    uint16_t slot_counter;   // счетчик DMX каналов пакета
    uint16_t time_error_counter;      // счетчик таймаута, 1с
    uint8_t data[DMX_BUFFER_SIZE]; // приемный буфер dmx
    person_t personality;   // текущий профиль dmx
} ;
// dmx
void dmx_reset(void);


// API -public
void ReceiveDMX(void);
// передается шаг таймера, зависит от аппаратной установки периода
void DMX_TimeCounter (uint8_t);
// data value - number channel нумерация от 1 !!!!
uint8_t GetDataChannelDMX (uint8_t channel) ;
void SetStartAddressDMX (uint16_t);
uint16_t GetStartAddressDMX (void);
void SetPersonality (person_t);
person_t GetPersonality(void);
bool ReceivedDataDMX (void);
bool ErrorDMX (void);


