/*
 * Comm.h
 *
 * Created: 25.04.2021 00:16:27
 *  Author: MrTea
 */ 


#ifndef COMM_H_
#define COMM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#define GPIO_WRITE(pin, val) *(&PORTA.OUTCLR + ((pin) / 8)*sizeof(PORTA) - (val)) = 1 << ((pin) % 8)
#define GPIO_DIR(pin, val) *(&PORTA.DIRCLR + ((pin) / 8)*sizeof(PORTA) - (val)) = 1 << ((pin) % 8)
#define GPIO_READ(pin) ((*(&PORTA.IN + ((pin) / 8)*sizeof(PORTA)) >> ((pin) % 8) & 1))

struct Outputs_struct
{
	uint8_t RELAY_SS_BAT : 1;
	uint8_t RELAY_SS_12V : 1;
	uint8_t RELAY_BAT : 1;
	uint8_t RELAY_12V : 1;
	uint8_t LED3 : 1;
	uint8_t LED2 : 1;
	uint8_t LED1 : 1;
	uint8_t FAN_ON : 1;
};
extern struct Outputs_struct Outputs;

struct Inputs_struct
{
	uint8_t ONOFF : 1;
	uint8_t BAT_PRESENT : 1;
	uint8_t BAT12V_PRESENT : 1;
	uint8_t rsvd : 5;
};
extern struct Inputs_struct Inputs;

void IO_update(void);



#define DATA_TX_LENGTH 9
#define DATA_RX_LENGTH 1

char valid_data_rx[DATA_RX_LENGTH];
char valid_data_tx[DATA_TX_LENGTH];

void Comm_init(void);

#endif /* COMM_H_ */