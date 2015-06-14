#ifndef CONFIG_H
#define	CONFIG_H

//Hardware specific include file
#include <p24F16KA101.h>

//Clock Frequency
#define FOSC_HZ	8000000	//8MHz

//Pins
enum PIN_DEFINITIONS
{
    PIN_RA0,			//RA0
    PIN_RA1,			//RA1
    PIN_RA2,			//RA2
    PIN_DIAGNOSTIC_LED,	//RA3
    PIN_RA4,			//RA4
    PIN_RA6,			//RA6
    PIN_RB2,			//RB2
    PIN_RB4,			//RB4
    PIN_RB7,			//RB7
    PIN_RB8,			//RB8
    PIN_RB9,			//RB9
    PIN_RB12,			//RB12
    PIN_PRESENCE_SENSOR,//RB13
    PIN_RB14,			//RB14
    PIN_RB15,			//RB15
	NUMBER_OF_PINS
};

//A2D Pin Definitions
enum A2D_PIN_DEFINITIONS
{
	AN0,								//AN0
	AN1,								//AN1
	AN2,								//AN2
	AN3,								//AN3
	AN4,								//AN4
	AN5,								//AN5
	AN6,								//AN6 - Does not exist on this chip
	AN7,								//AN7 - Does not exist on this chip
	AN8,								//AN8 - Does not exist on this chip
	AN9,								//AN9 - Does not exist on this chip
	AN10,								//AN10
	AN11_CAPACITIVE_PRESENSE_SENSOR,	//AN11
	AN12,								//AN12
	AN13,								//AN13 - Does not exist on this chip
	AN14,								//AN14 - Does not exist on this chip
	AN15,								//AN15 - Does not exist on this chip
	NUMBER_OF_A2D_PINS
};

//Libaries
//A2D Library
#define A2D_MAJOR	1
#define A2D_MINOR	1
#define A2D_PATCH	0

//Pins Library
#define PINS_MAJOR	2
#define PINS_MINOR	0
#define PINS_PATCH	0

//Pins CTMU
#define CTMU_MAJOR	0
#define CTMU_MINOR	0
#define CTMU_PATCH	0

void Configure_For_Capacitive_Control(void);

#endif	/* CONFIG_H */
