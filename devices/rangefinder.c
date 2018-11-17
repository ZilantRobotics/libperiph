

/** 
* @file rangefinder.с
* @brief Implementation of rangefinder
*/

/* Подключение ультразвукового дальномера SRF-05:
 * Ножка датчика                Подключение к dsPIC  
 * ECHO - output                RA15 (INT4) - input
 * TRIG - input                 RD8 (IC1) - output
 * GND                          GND
 * VCC                          5 V (but dsPIC only 3.3V)
 *
 * Принцип работы ультразвукового дальномера SRF-05
 * - на ножку TRIG датчика подаем импульс длительностью 10 мкс;
 * - с ножки ECHO датчика считываем импульс, длительность которого 
 * пропорциональна расстоянию до препятствия.
 */

#include "rangefinder.h"
#include "timer.h"

static uint32_t range;
static Timer timer; 


/**
* @brief Инициализация ултьтразвукового дальномера
*/
void rangefinder_init()
{
    rangefinder_init_interrupt();
    soft_timer_init(&timer);
    range = 0;
}


/**
* @brief Выполнить измерение расстояния
* @note Подать импульс, подождать, получить импульс, вернуть расстояние (в см)
*/
uint16_t rangefinder_do()
{
    rangefinder_give_impulse();
    timer_start_ms(&timer, 100);
    while(timer_report(&timer) == TIMER_WORKING);
    return rangefinder_get_range();
}

/**
* @brief Подать импульс на датчик длительностью минимум 10 мс
*/
void rangefinder_give_impulse()
{
    /// Заблаговременно устанавливаем ножку в 0
    RANGEFINDER_OUTPUT = 0;
    timer_start_us(&timer, 5);
    while(timer_report(&timer) == TIMER_WORKING);
    
    /// Подаем импульс 10 мкс
    RANGEFINDER_OUTPUT = 1;
    timer_start_us(&timer, 10);
    while(timer_report(&timer) == TIMER_WORKING);
    RANGEFINDER_OUTPUT = 0;
}


/**
* @brief Получить измеренное расстояние (в cм)
*/
uint16_t rangefinder_get_range()
{
    return (float)range*0.0085 + 1.43;
}      


/** 
* @brief Получить медианное значение среди трех измерений дальности
*/
uint16_t get_median_range()
{
    uint16_t range_1 = rangefinder_do();
    uint16_t range_2 = rangefinder_do();
    uint16_t range_3 = rangefinder_do();
    
    if (range_1 >= range_2)
    {
        if (range_1 >= range_3)
            return (range_2 >= range_3) ? range_2 : range_3;
        else
            return range_1;
    }
    else
    {
        if (range_2 >= range_3)
            return (range_1 > range_3) ? range_1 : range_3;
    }
        
    return range_2;
}


/**
* @brief Прерывание от дальномера (INT4)
*/
void __attribute__((interrupt, no_auto_psv)) _INT4Interrupt(void)
{
    static uint32_t lastTime;
    uint32_t nowTime = hard_timer_return_time();
    
    if ( (RANGEFINDER_TYPE_OF_INTERRUPT) == ENCODER_POSITIVE_EDGE)
    {
        lastTime = nowTime;
        RANGEFINDER_TYPE_OF_INTERRUPT = ENCODER_NEGATIVE_EDGE;
    }
    else
    {
        range = nowTime - lastTime;
        RANGEFINDER_TYPE_OF_INTERRUPT = ENCODER_POSITIVE_EDGE;
    }
    IFS3bits.INT4IF = 0;      /// Очистка флага прерывания
}