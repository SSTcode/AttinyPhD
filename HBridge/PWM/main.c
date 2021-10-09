/*
 * Bridge.c
 *
 
 * Created: 26/09/2019 16:12:28
 * Author : FRA
 */ 

#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <string.h>

#include "Comm.h"
#include "Temp_meas.h"

void CLK_ISO_init(void)
{

	
	/* set waveform output on PORT A */
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm /* enable compare channel 0 . Match between the counter value and the Compare 1 register.*/
	| TCA_SINGLE_CMP2EN_bm /* enable compare channel 2 */
	| TCA_SINGLE_WGMODE_DSBOTTOM_gc; /* set dual-slope PWM mode */

	/* disable event counting */
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);

	/* set PWM frequency 400KHz*/
	TCA0.SINGLE.PERBUF = 0x0C; /*buffer of the Period register*/
	/*  duty cycle (50%) */
	TCA0.SINGLE.CMP0BUF = (TCA0.SINGLE.PERBUF>>1)+1; //1010>>0101___10>>1=5 DUTY_CYCLE Shift right Symbol: >>
	TCA0.SINGLE.CMP2BUF = (TCA0.SINGLE.PERBUF>>1)-1; //
	
		TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc /* set clock source (sys_clk/1) */
		| TCA_SINGLE_ENABLE_bm; /* start timer */
	
	PORTA.DIR |= PIN2_bm | PIN3_bm;
	PORTA.PIN3CTRL |= PORT_INVEN_bm;

	
	while(TCA0.SINGLE.CMP0BUF > (TCA0.SINGLE.PERBUF>>1)+6)
	{
		TCA0.SINGLE.CMP0BUF--;
		TCA0.SINGLE.CMP2BUF++;
		_delay_ms(10);
	}
}

int main(void)
{
	cli();
	/*setting clock*/
	CPU_CCP = 0xD8; // CCP: Configuration Change Protection Un-protect protected I/O registers
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc; // 16/20 MHz internal oscillator
	CPU_CCP = 0xD8;
	CLKCTRL.MCLKCTRLB = 1;
	/*END setting clock*/
		
	CLK_ISO_init();
		
	Comm_init();

	Temp_meas_init();

	sei();

    while (1)
    {
		Temp_meas();
		memcpy(valid_data_tx, &Temperature_int, 2);
		Comm_send();
    }
}
