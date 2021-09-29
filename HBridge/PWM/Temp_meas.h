/*
 * Temp_meas.h
 *
 * Created: 25.04.2021 09:46:42
 *  Author: MrTea
 */ 


#ifndef TEMP_MEAS_H_
#define TEMP_MEAS_H_

#include <avr/io.h>
#include <stdlib.h>
#include <math.h>

extern int16_t Temps[4];

void Temp_meas_init(void);
void Temp_meas(void);

#endif /* TEMP_MEAS_H_ */