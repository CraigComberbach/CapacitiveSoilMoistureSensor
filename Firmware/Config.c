/**************************************************************************************************
Authours:				Craig Comberbach
Target Hardware:		PIC
Chip resources used:
Code assumptions:
Purpose:

Version History:
v0.0.0	2013-07-11  Craig Comberbach
	Compiler: C30 v3.31		IDE: MPLABx 1.80	Tool: RealICE	Computer: Intel Xeon CPU 3.07 GHz, 6 GB RAM, Windows 7 64 bit Professional SP1
	First version
**************************************************************************************************/
/*************    Header Files    ***************/
#include "Config.h"
#include "../../Libraries/Pins/Pins.h"
#include "../../Libraries/A2D/A2D.h"
#include "../../Libraries/CTMU/CTMU.h"

/************* Semantic Versioning***************/
//This project reqires the Pins library to fulfil it's roll
#ifndef PINS_LIBRARY
	#error "You need to include the Pins library for this code to compile"
#endif

#ifndef A2D_LIBRARY
	#error "You need to include the A2D library for this code to compile"
#endif

#ifndef CTMU_LIBRARY
	#error "You need to include the A2D library for this code to compile"
#endif

/************Arbitrary Functionality*************/
/*************   Magic  Numbers   ***************/
/***********State Machine Definitions*************/
/*************  Global Variables  ***************/
int capacitivePresense;

/*************Function  Prototypes***************/
/************* Device Definitions ***************/
/************* Module Definitions ***************/
const struct PIN_DEFINITION PIN_DIAGNOSTIC_LED	= {Rx0, &TRISA, &ODCA, &LATA, &PORTA};	//RA0
const struct PIN_DEFINITION PIN_RA1				= {Rx1, &TRISA, &ODCA, &LATA, &PORTA};	//RA1
const struct PIN_DEFINITION PIN_RA2				= {Rx2, &TRISA, &ODCA, &LATA, &PORTA};	//RA2
const struct PIN_DEFINITION PIN_RA3				= {Rx3, &TRISA, &ODCA, &LATA, &PORTA};	//RA3
const struct PIN_DEFINITION PIN_RA4				= {Rx4, &TRISA, &ODCA, &LATA, &PORTA};	//RA4
//const struct PIN_DEFINITION PIN_A5			= {Rx5, &TRISA, &ODCA, &LATA, &PORTA};	//RA5
const struct PIN_DEFINITION PIN_RA6				= {Rx6, &TRISA, &ODCA, &LATA, &PORTA};	//RA6
//const struct PIN_DEFINITION PIN_B0			= {Rx0, &TRISB, &ODCB, &LATB, &PORTB};	//RB0
//const struct PIN_DEFINITION PIN_B1			= {Rx1, &TRISB, &ODCB, &LATB, &PORTB};	//RB1
const struct PIN_DEFINITION PIN_RB2				= {Rx2, &TRISB, &ODCB, &LATB, &PORTB};	//RB2
const struct PIN_DEFINITION PIN_RB4				= {Rx4, &TRISB, &ODCB, &LATB, &PORTB};	//RB4
const struct PIN_DEFINITION PIN_RB7				= {Rx7, &TRISB, &ODCB, &LATB, &PORTB};	//RB7
const struct PIN_DEFINITION PIN_RB8				= {Rx8, &TRISB, &ODCB, &LATB, &PORTB};	//RB8
const struct PIN_DEFINITION PIN_RB9				= {Rx9, &TRISB, &ODCB, &LATB, &PORTB};	//RB9
const struct PIN_DEFINITION PIN_RB12			= {Rx12, &TRISB, &ODCB, &LATB, &PORTB};	//RB12
const struct PIN_DEFINITION PIN_PRESENCE_SENSOR	= {Rx13, &TRISB, &ODCB, &LATB, &PORTB};	//RB13
const struct PIN_DEFINITION PIN_RB14			= {Rx14, &TRISB, &ODCB, &LATB, &PORTB};	//RB14
const struct PIN_DEFINITION PIN_RB15			= {Rx15, &TRISB, &ODCB, &LATB, &PORTB};	//RB15

/************* Other  Definitions ***************/

void Configure_For_Capacitive_Control(void)
{
	/*************        Pins        ***************/
	//PORTA
	Pin_Initialize(PIN_DIAGNOSTIC_LED,	LOW, PUSH_PULL, OUTPUT);	//RA0
	Pin_Initialize(PIN_RA1,				LOW, PUSH_PULL, INPUT);		//RA1
	Pin_Initialize(PIN_RA2,				LOW, PUSH_PULL, INPUT);		//RA2
	Pin_Initialize(PIN_RA3,				LOW, PUSH_PULL, OUTPUT);	//RA3
	Pin_Initialize(PIN_RA4,				LOW, PUSH_PULL, OUTPUT);	//RA4
	Pin_Initialize(PIN_RA6,				LOW, PUSH_PULL, OUTPUT);	//RA6

	//PORTB
	Pin_Initialize(PIN_RB2,				LOW, PUSH_PULL, OUTPUT);	//RB2
	Pin_Initialize(PIN_RB4,				LOW, PUSH_PULL, OUTPUT);	//RB4
	Pin_Initialize(PIN_RB7,				LOW, PUSH_PULL, INPUT);		//RB7
	Pin_Initialize(PIN_RB8,				LOW, PUSH_PULL, OUTPUT);	//RB8
	Pin_Initialize(PIN_RB9,				LOW, PUSH_PULL, OUTPUT);	//RB9
	Pin_Initialize(PIN_RB12,			LOW, PUSH_PULL, OUTPUT);	//RB12
	Pin_Initialize(PIN_PRESENCE_SENSOR,	LOW, PUSH_PULL, OUTPUT);	//RB13
	Pin_Initialize(PIN_RB14,			LOW, PUSH_PULL, OUTPUT);	//RB14
	Pin_Initialize(PIN_RB15,			LOW, PUSH_PULL, OUTPUT);	//RB15

	/*************       Timers       ***************/
	//Timer 1
	PR1 = 500;//500 = 1 mS assuming Prescaler /8
	IEC0bits.T1IE = 1;
	T1CONbits.TCS	= 0;
//	T1CONbits.TSYNC	= Ignored
	T1CONbits.TCKPS	= 1; //1 = Fosc/8
	T1CONbits.TGATE	= 0;
	T1CONbits.TSIDL	= 0;
	T1CONbits.TON	= 1;

	/*************        A2D         ***************/
	//A2D
	A2D_Initialize();

	//Setup the individual A2D channel settings
	A2D_Advanced_Channel_Settings(CAPACITIVE_PRESENSE_SENSOR, RESOLUTION_10_BIT, 16, NO_FORMATING, &CTMU_Start, &CTMU_Stop, &Capacitive_Presence_Update, NORMAL_AVERAGING, A2D_MAX_SAMPLES);	//AN1
//	A2D_Channel_Settings(CAPACITIVE_PRESENSE_SENSOR, RESOLUTION_10_BIT,	16,	NO_FORMATING, &CTMU_Start, &CTMU_Stop, &Capacitive_Presence_Update);	//AN1

	//Setup A2D Scan Queue
	A2D_Add_To_Scan_Queue(CAPACITIVE_PRESENSE_SENSOR);

	//CTMU
	CTMU_Initialize();

	return;
}

void Capacitive_Presence_Update(int channel)
{
	capacitivePresense = A2D_Value(channel);
	return;
}
