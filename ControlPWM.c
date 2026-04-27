#include "ControlPWM.h"

struct {
    curve_t curve;
    uint8_t speed;  // 7, 8,9
    uint16_t count;
    uint16_t ratio_normaliz;
    uint16_t step[TotalChannelPWM];
    uint32_t current[TotalChannelPWM];
    bool rise[TotalChannelPWM];
} pwm = {square, 7, 0, _RatioNormaliz};

//------------------------------------------------------------------------------
void PWM_Set (void) {   // 
    if (pwm.count) {
        --pwm.count;
        for (uint8_t count=0; count<TotalChannelPWM; ++count) {
            if(pwm.rise[count]) 
                pwm.current[count] += pwm.step[count];
            else 
                pwm.current[count] -= pwm.step[count];
        }   
        uint16_t temp = (pwm.current[0]>>pwm.speed);
    // костыль с нижней отсечкой 
        if (temp<200) {
            PWM1_Set(0);
        }
        else 
            PWM1_Set(temp); // (uint16_t)(pwm.current[0]>>pwm.speed)
        PWM2_Set((uint16_t)(pwm.current[1]>>pwm.speed));
        PWM3_Set((uint16_t)(pwm.current[2]>>pwm.speed));
        PWM4_Set((uint16_t)(pwm.current[3]>>pwm.speed));
    }    
    // strob;
}

uint16_t CurveCalc (uint8_t dimmer, uint8_t channel) {  // 
    if (pwm.curve == square) {
        uint24_t ret = dimmer+1;
        ret *= dimmer;
        ret *= channel;
        return (uint16_t)( ret>>8 );
    }    
    return (dimmer*channel);    // linear
}

// 16-ти битный формат, соот-вие м/у входным значением и заполнением шим
uint16_t Normalization (uint32_t _pwm) {    // 
    return (uint16_t)(_pwm * pwm.ratio_normaliz >> 15);
}

void PWM_CalcSimple (uint8_t new_data) {    // 153us
    uint16_t new_pwm ;    // 
    new_pwm = Normalization(CurveCalc(new_data, 255));    // 85us (66us)
                
    pwm.count = 0;
    uint16_t temp = (uint16_t)(pwm.current[0]>>pwm.speed) ;
    if (new_pwm > temp) {
        pwm.step[0] = new_pwm - temp;
        pwm.rise[0] = true;
    }
    else {
        pwm.step[0] = temp - new_pwm;
        pwm.rise[0] = false;
    }
    pwm.count = 64;   // >32ms    1<<pwm.speed; 
}

void PWM_Calc (uint8_t dimmer, const uint8_t* new_data){
    
}

//------------------------------------------------------------------------------
curve_t GetCurve (void) {
    return pwm.curve;}

void SetCurve (curve_t curve) {    
    if (end_curve > curve) pwm.curve = curve;    
}

void SetSpeed (uint8_t speed) {    
    pwm.speed = speed;    
}
uint8_t GetSpeed (void){
    return pwm.speed ;
}

