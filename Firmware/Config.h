#ifndef CONFIG_H
#define	CONFIG_H

//Hardware specific include file
#include <p24F16KA101.h>

//Clock Frequency
#define FOSC_HZ	8000000	//8MHz

//Frequency Definitions
enum FREQUENCY_UNITS
{
	Hz,
	kHz,
	MHz,
};

//Timers
//#define MAIN_LOOP	TIMER1
//#define UNUSED_TMR2	TIMER2
//#define UNUSED_TMR3	TIMER3

//I2C Modules
//#define I2C_1	1
//#define I2C_2	2
//#define I2C_3	3
//#define I2C_USE_MASTER
//#define I2C_USE_SLAVE

//SPI Modules
//enum
//{
//	SPI_TEST_MODULE,		//SPI1
//	NUMBER_OF_SPI_MODULES,
//};

//UART Modules
//#define UART1	1
//#define UART2	2
//#define UART3	3
//#define UART4	4

//Capture Compare Modules
//#define OC1		1
//#define OC2		2
//#define OC3		3
//#define OC4		4
//#define OC5		5
//#define OC6		6
//#define OC7		7
//#define OC8		8

//Fatal Errors
//enum
//{
//	TIMER_LIBRARY,
//	I2C_LIBRARY,
//	SPI_LIBRARY,
//	UART_LIBRARY,
//	A2D_LIBRARY,
//	CAPTURE_COMPARE_LIBRARY,
//	PINS_LIBRARY
//};

//Timers
//extern const struct TIMER_DEFINITION BLINKING_LED;
//extern const struct TIMER_DEFINITION UNUSED_TMR2;
//extern const struct TIMER_DEFINITION UNUSED_TMR3;

//Pins
extern const struct PIN_DEFINITION PIN_DIAGNOSTIC_LED;	//RA0
extern const struct PIN_DEFINITION PIN_RA1;				//RA1
extern const struct PIN_DEFINITION PIN_RA2;				//RA2
extern const struct PIN_DEFINITION PIN_RA3;				//RA3
extern const struct PIN_DEFINITION PIN_RA4;				//RA4
extern const struct PIN_DEFINITION PIN_RA6;				//RA6
extern const struct PIN_DEFINITION PIN_RB2;				//RB2
extern const struct PIN_DEFINITION PIN_RB4;				//RB4
extern const struct PIN_DEFINITION PIN_RB7;				//RB7
extern const struct PIN_DEFINITION PIN_RB8;				//RB8
extern const struct PIN_DEFINITION PIN_RB9;				//RB9
extern const struct PIN_DEFINITION PIN_RB12;			//RB12
extern const struct PIN_DEFINITION PIN_PRESENCE_SENSOR;	//RB13
extern const struct PIN_DEFINITION PIN_RB14;			//RB14
extern const struct PIN_DEFINITION PIN_RB15;			//RB15

//A2D Pin Definitions
enum A2D_PIN_DEFINITIONS
{
	AN0,	//AN0
	AN1,	//AN1
	AN2,	//AN2
	AN3,	//AN3
	AN4,	//AN4
	AN5,	//AN5
	AN6,	//AN6
	AN7,	//AN7
	AN8,	//AN8
	AN9,	//AN9
	AN10,	//AN10
	CAPACITIVE_PRESENSE_SENSOR,	//AN11
	AN12,	//AN12
	AN13,	//AN13
	AN14,	//AN14
	AN15,	//AN15
};

//Libaries
//A2D Library
#define A2D_MAJOR	1
#define A2D_MINOR	1
#define A2D_PATCH	0

//Pins Library
#define PINS_MAJOR	1
#define PINS_MINOR	0
#define PINS_PATCH	0

//Pins CTMU
#define CTMU_MAJOR	0
#define CTMU_MINOR	0
#define CTMU_PATCH	0

void Configure_For_Capacitive_Control(void);
void Capacitive_Presence_Update(int channel);

#endif	/* CONFIG_H */
