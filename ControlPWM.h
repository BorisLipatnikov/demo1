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


// ver 2.0
// Простое одноканальное управление низкочастотным диммированием
// отсутствует выделенный канал отключения светодрайвера
// отсутствует диммирующий транзистор
#include <stdbool.h>
#include "mcc_generated_files/pwm1.h"

// базовые установки
#define TotalChannelPWM     1   // 1-4
#define PWM1_Set(a)     PWM1_DutyCycleSet(a), PWM1_LoadBufferSet()
#define PWM2_Set(a)     
#define PWM3_Set(a)     
#define PWM4_Set(a)     

// размер счетчика периода шим для выбранной частоты шим
#define SIZE_COUNT_PWM  (uint32_t)52800
#define _RatioNormaliz  ((SIZE_COUNT_PWM<<15)/65025)

// 


typedef enum Curve{
    linear, square, end_curve 
}curve_t;

// public
// запись шим, периодический вызов по прерыванию
void PWM_Set (void); 

// 
void PWM_CalcSimple (uint8_t new_data);
void PWM_Calc (uint8_t dimmer, const uint8_t* new_data);

void SetCurve (curve_t);
curve_t GetCurve (void);

void SetSpeed (uint8_t speed);
uint8_t GetSpeed (void);