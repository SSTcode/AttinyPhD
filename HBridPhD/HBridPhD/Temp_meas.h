/*
 * Temp_meas.h
 *
 * Created: 28-09-2021 20:50:14
 *  Author: fruiz
 */ 


#ifndef TEMP_MEAS_H_
#define TEMP_MEAS_H_

#include <avr/io.h>
#include <stdlib.h>
#include <math.h>

extern int16_t Temps[1];

void Temp_meas_init(void);
void Temp_meas(void);




#endif /* TEMP_MEAS_H_ */