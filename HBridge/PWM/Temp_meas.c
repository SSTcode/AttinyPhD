/*
 * Temp_meas.c
 *
 * Created: 25.04.2021 09:46:28
 *  Author: MrTea
 */ 

#include "Temp_meas.h"

struct Thermistor_struct
{
	float Divider_supply;
	float R_divider;
	float B;
	float T_0;
	float R25;
	float DIV_Rinf;
}Therm;

int16_t Temps[4];

void Temp_meas_init(void)
{
	ADC0.CTRLC = ADC_PRESC_DIV8_gc | ADC_REFSEL_VDDREF_gc;
	ADC0.CTRLB = ADC_SAMPNUM_ACC64_gc;
	ADC0.CTRLA = ADC_ENABLE_bm;
	
	Therm.Divider_supply = 0.0f;
	Therm.R_divider = 4700.0f;
	Therm.B = 4110.0f;
	Therm.T_0 = 273.15f;
	Therm.R25 = 10000.0f;
	Therm.DIV_Rinf = expf(Therm.B/(Therm.T_0+25.0f))/Therm.R25;
}

void Temp_meas(void)
{			
	while (ADC0.COMMAND);
	uint16_t adcVal = ADC0.RES;
			
	static uint8_t Thermistor_select = 0;
	if(++Thermistor_select >= 4) Thermistor_select = 0;
	switch(Thermistor_select)
	{
		case 0:
		{
			ADC0.MUXPOS  = ADC_MUXPOS_AIN4_gc;
			break;
		}
		case 1:
		{
			ADC0.MUXPOS  = ADC_MUXPOS_AIN3_gc;
			break;
		}
		case 2:
		{
			ADC0.MUXPOS  = ADC_MUXPOS_AIN2_gc;
			break;
		}
		case 3:
		{
			ADC0.MUXPOS  = ADC_MUXPOS_AIN5_gc;
			break;
		}
	}
	ADC0.COMMAND = ADC_STCONV_bm;
			
	float Resistance = Therm.R_divider * (float)adcVal / (float)(65536 - 64 - adcVal);
	float Temperature = Therm.B/logf(Resistance * Therm.DIV_Rinf) - Therm.T_0;
	Temps[Thermistor_select] = fminf(Temperature, 300.0f) * 100.0f;
}