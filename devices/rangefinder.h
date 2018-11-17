/** 
* @file rangefinder.h
* @brief Declaration of rangefinder
*/

#ifndef RANGEFINDER_H
#define	RANGEFINDER_H

#include "hard.h"

void rangefinder_init();            /// Инициализация датчика
uint16_t rangefinder_do();          /// Выполнить измерение расстояния
void rangefinder_give_impulse();    /// Подать импульс на датчик
uint16_t rangefinder_get_range();   /// Получить измеренное расстояние (в мм)
uint16_t get_median_range();          /// Получить медианное измеренное расстояние (в мм)

#endif	/* RANGEFINDER_H */