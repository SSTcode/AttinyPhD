#define F_CPU 10000000

#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <string.h>
#include <util/delay.h>

#include "Comm.h"
#include "Temp_meas.h"

void CLK_ISO_init(void)
{
	TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm; //Enable Split mode
	TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm;
	TCA0.SPLIT.HPER = 99;
	TCA0.SPLIT.HCMP0 = 0x0; 
	TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1_gc | TCA_SPLIT_ENABLE_bm; //Clock Select and Enable the peripheral
	GPIO_DIR(0 + 3, 1); //port a 3, output   
	// w-r  (    , 0)   0=read  1=writ
	// WO[n+3] Out
	_delay_ms(1);
	
	while(TCA0.SPLIT.HCMP0 < (TCA0.SPLIT.HPER+1) >> 1)
	{
		TCA0.SPLIT.HCMP0++;
		_delay_ms(1);
	}
}

int main(void)
{
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
		cli();
		memcpy(valid_data_tx, Temps, 8);
	    sei();
		/*if (TCB0.CNT > 7500)
		{
			*(uint8_t *)&Outputs = 0;
			IO_update();
		}*/
	}
}
