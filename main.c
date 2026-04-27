#include "mcc_generated_files/mcc.h"
#include "main.h"
#include "dmx_read.h"
#include "ControlPWM.h"


void main(void)
{    
    // initialize the device
    SYSTEM_Initialize();
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    //INTERRUPT_PeripheralInterruptDisable();
        
    TMR1IF = 1; // для перехода на чтение дипа
        
    while (1)
    {
        CLRWDT();
        ReadDip();
        HandlerDMX();
    }
}

//------------------------------------------------------------------------------
void interrupt_tmr0 (void) {  // period 4 ms
    DMX_TimeCounter (4);
    HandlerTimeCounter(4);
    PWM_Set();
}

void HandlerDMX(void) { 
    uint8_t channel_data;
    
    if (ReceivedDataDMX()) {
        channel_data = GetDataChannelDMX(1);        
    }                
    else if (HandlerTimeFlag()) {
        channel_data = 255;
    }
    else return;
    
    PWM_CalcSimple (channel_data);
}

//------------------------------------------------------------------------------
union DIP dip;

void ReadDip (void) {   // period call 8s
    if (TMR1IF) TMR1IF = 0;
    else return;
    
    dip.Dip = 0;
    dip.D10 = !RC3;
    dip.D9 = !RC2;
    dip.D8 = !RC1;
    dip.D7 = !RC0;
    dip.D6 = !RB7;
    dip.D5 = !RB6;
    dip.D4 = !RB5;
    dip.D3 = !RB4;
    dip.D2 = !RA5;
    dip.D1 = !RA4;
    
    if (dip.Dip > 512) dip.Dip = 512;        
    SetStartAddressDMX (dip.Dip);
}


void HandlerTimeCounter(uint8_t value) {
    static uint16_t counter = 0;
    if (ErrorDMX()) 
        counter += value;
    else 
        counter = 0;
    if (counter > _64MS) {
        handler_time_flag = true;
        counter = 0;
    }
}

bool HandlerTimeFlag (void) {
    if (!handler_time_flag)  return false;
    handler_time_flag = 0;
    return true;
}