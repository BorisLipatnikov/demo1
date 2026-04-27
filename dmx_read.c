#include <xc.h>
#include "dmx_read.h"

struct DMX dmx;
// подписи профилей
 uint8_t*  Personal_Descript [Total_Personality] = {
    "DIRECT"
};
// кол-во занимаемых каналов в каждом профиле dmx
const uint8_t Footprint_Personality [Total_Personality] = {1};

//------------------------------------------------------------------------------ 
void operation_clear(void) {
    dmx.break_ = 0; 
    dmx.start_ = 0;            
    dmx.slot_counter = 0;
    dmx.time_error_counter = 0; 
    dmx.error = 0;
    dmx.flag_received = 0;
}

void ReceiveDMX (void){        
    uint8_t buffer_9, buffer_data ;
    
    if (UART_OVERFLOW) {
        dmx_reset();
    }    
    // сигнал сброса BREAK
    else if (UART_FERR) {
        operation_clear();
        buffer_9 = BUF_RX9;  
        buffer_data = BUF_RX;
        if ((buffer_data == 0)&&(buffer_9 == 0)) {
            dmx.break_ = 1;            
            dmx.error = 0;
        }
    }        
    // сигнал приема START
    else if (dmx.break_) {
        buffer_data = BUF_RX;
        if (buffer_data == 0) dmx.start_ = 1;  // прием значений каналов 
        dmx.break_ = 0;
    }
    // прием значений каналов      
    else if (dmx.start_ == 1) {
        buffer_data = BUF_RX;
        ++dmx.slot_counter ;        
        if (dmx.slot_counter >= dmx.start_address) {
            uint16_t  temp = dmx.slot_counter - dmx.start_address;
            if (temp < DMX_BUFFER_SIZE) {
                dmx.data[temp] = buffer_data;    
                if ((temp+1) == DMX_BUFFER_SIZE) {
                    dmx.flag_received = 1;
                    dmx.break_ = 0;
                    dmx.start_ = 0;
                }
            }
        }        
    } 
}

void DMX_TimeCounter (uint8_t a) {
    if (dmx.time_error_counter < 1000) dmx.time_error_counter += a; 
    else dmx.error = 1;
   
}
    
void dmx_reset(void) {
    UART_Rx_OFF;         // сброс флага OERR		
	uint8_t temp = BUF_RX;
	temp = BUF_RX;
	temp = BUF_RX;
	operation_clear();
    UART_Rx_ON;
}

uint8_t GetDataChannelDMX (uint8_t channel) {
    if (channel > Footprint_Personality[dmx.personality]) 
        channel = Footprint_Personality[dmx.personality];
    if (channel == 0) channel = 1;
    dmx.flag_received = 0;
    return dmx.data[channel-1];
}

void SetStartAddressDMX (uint16_t value) {
    if (GetStartAddressDMX() == value) return;
    while(value > 512)
        value -= 512;   
    dmx.start_address = value;
}

uint16_t GetStartAddressDMX (void) {
    return dmx.start_address;
}

void SetPersonality (person_t personality){    
    if ((uint8_t)personality >= Total_Personality ) personality = 0;     
    dmx.personality = personality;
}

person_t GetPersonality(void) {
    return dmx.personality;
}

bool ReceivedDataDMX (void){
    return dmx.flag_received;
}

bool ErrorDMX (void) {
    return dmx.error;
}