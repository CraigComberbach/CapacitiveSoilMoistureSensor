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
void Capacitive_Presence_Update(int channel);
void __attribute__((interrupt, auto_psv)) _OscillatorFail(void);
void __attribute__((interrupt, auto_psv)) _AddressError(void);
void __attribute__((interrupt, auto_psv)) _StackError(void);
void __attribute__((interrupt, auto_psv)) _MathError(void);
void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void);

/************* Device Definitions ***************/
#define Reset()	asm("reset")

/************* Module Definitions ***************/
/************* Other  Definitions ***************/

void Configure_For_Capacitive_Control(void)
{
	/*************        Pins        ***************/
	//PORTA
	Pin_Definition(PIN_RA0,				Rx0, &TRISA, &ODCA, &LATA, &PORTA); //RA0
	Pin_Definition(PIN_RA1,				Rx1, &TRISA, &ODCA, &LATA, &PORTA); //RA1
	Pin_Definition(PIN_RA2,				Rx2, &TRISA, &ODCA, &LATA, &PORTA); //RA2
	Pin_Definition(PIN_DIAGNOSTIC_LED,	Rx3, &TRISA, &ODCA, &LATA, &PORTA); //RA3
	Pin_Definition(PIN_RA4,				Rx4, &TRISA, &ODCA, &LATA, &PORTA); //RA4
	//Pin_Definition(PIN_A5,			Rx5, &TRISA, &ODCA, &LATA, &PORTA); //RA5
	Pin_Definition(PIN_RA6,				Rx6, &TRISA, &ODCA, &LATA, &PORTA); //RA6
	Pin_Initialize(PIN_RA0,				LOW, PUSH_PULL, INPUT);	//RA0
	Pin_Initialize(PIN_RA1,				LOW, PUSH_PULL, INPUT);	//RA1
	Pin_Initialize(PIN_RA2,				LOW, PUSH_PULL, INPUT);	//RA2
	Pin_Initialize(PIN_DIAGNOSTIC_LED,	LOW, PUSH_PULL, OUTPUT);//RA3
	Pin_Initialize(PIN_RA4,				LOW, PUSH_PULL, INPUT);	//RA4
	Pin_Initialize(PIN_RA6,				LOW, PUSH_PULL, INPUT);	//RA6

	//PORTB
	//Pin_Definition(PIN_B0,			Rx0, &TRISB, &ODCB, &LATB, &PORTB); //RB0
	//Pin_Definition(PIN_B1,			Rx1, &TRISB, &ODCB, &LATB, &PORTB); //RB1
	Pin_Definition(PIN_RB2,				Rx2, &TRISB, &ODCB, &LATB, &PORTB); //RB2
	Pin_Definition(PIN_RB4,				Rx4, &TRISB, &ODCB, &LATB, &PORTB); //RB4
	Pin_Definition(PIN_RB7,				Rx7, &TRISB, &ODCB, &LATB, &PORTB); //RB7
	Pin_Definition(PIN_RB8,				Rx8, &TRISB, &ODCB, &LATB, &PORTB); //RB8
	Pin_Definition(PIN_RB9,				Rx9, &TRISB, &ODCB, &LATB, &PORTB); //RB9
	Pin_Definition(PIN_RB12,			Rx12, &TRISB, &ODCB, &LATB, &PORTB); //RB12
	Pin_Definition(PIN_PRESENCE_SENSOR,	Rx13, &TRISB, &ODCB, &LATB, &PORTB); //RB13
	Pin_Definition(PIN_RB14,			Rx14, &TRISB, &ODCB, &LATB, &PORTB); //RB14
	Pin_Definition(PIN_RB15,			Rx15, &TRISB, &ODCB, &LATB, &PORTB); //RB15
	Pin_Initialize(PIN_RB2,				LOW, PUSH_PULL, INPUT);	//RB2
	Pin_Initialize(PIN_RB4,				LOW, PUSH_PULL, INPUT);	//RB4
	Pin_Initialize(PIN_RB7,				LOW, PUSH_PULL, INPUT);	//RB7
	Pin_Initialize(PIN_RB8,				LOW, PUSH_PULL, INPUT);	//RB8
	Pin_Initialize(PIN_RB9,				LOW, PUSH_PULL, INPUT);	//RB9
	Pin_Initialize(PIN_RB12,			LOW, PUSH_PULL, INPUT);	//RB12
	Pin_Initialize(PIN_PRESENCE_SENSOR,	LOW, PUSH_PULL, INPUT);	//RB13
	Pin_Initialize(PIN_RB14,			LOW, PUSH_PULL, INPUT);	//RB14
	Pin_Initialize(PIN_RB15,			LOW, PUSH_PULL, INPUT);	//RB15

	/*************        CTMU        ***************/
	Pin_Translation(AN0, PIN_RA0);										//AN0
	Pin_Translation(AN1, PIN_RA1);										//AN1
	//Pin_Translation(AN2, PIN_B0);										//AN2 - Used for programming
	//Pin_Translation(AN3, PIN_B1);										//AN3 - Used for programming
	Pin_Translation(AN4, PIN_RA2);										//AN4
	Pin_Translation(AN5, PIN_DIAGNOSTIC_LED);							//AN5
	//Pin_Translation(AN6, );											//AN6 - Does not exist on this chip
	//Pin_Translation(AN7, );											//AN7 - Does not exist on this chip
	//Pin_Translation(AN8, );											//AN8 - Does not exist on this chip
	//Pin_Translation(AN9, );											//AN9 - Does not exist on this chip
	Pin_Translation(AN10, PIN_RB14);									//AN10
	Pin_Translation(AN11_CAPACITIVE_PRESENSE_SENSOR, PIN_PRESENCE_SENSOR);	//AN11
	Pin_Translation(AN12, PIN_RB12);									//AN12
	//Pin_Translation(AN13, );											//AN13 - Does not exist on this chip
	//Pin_Translation(AN14, );											//AN14 - Does not exist on this chip
	//Pin_Translation(AN15, );											//AN15 - Does not exist on this chip

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
	A2D_Advanced_Channel_Settings(AN11_CAPACITIVE_PRESENSE_SENSOR, RESOLUTION_10_BIT, 16, NO_FORMATING, &CTMU_Start, &CTMU_Discharge, &Capacitive_Presence_Update, NORMAL_AVERAGING, A2D_ONE_SAMPLE);	//AN1

	//Setup A2D Scan Queue
	A2D_Add_To_Scan_Queue(AN11_CAPACITIVE_PRESENSE_SENSOR);

	//CTMU
	CTMU_Initialize();

	return;
}

void Capacitive_Presence_Update(int channel)
{
	capacitivePresense = A2D_Value(channel);
	return;
}

//Error Traps
//Oscillator Failed
void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
	INTCON1bits.OSCFAIL = 0;
}

//Address Out of range
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
	INTCON1bits.ADDRERR = 0;
	Reset();
}

//Stack Overflow
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
	INTCON1bits.STKERR = 0;
	Reset();
}

//Math error, stupid divide by zero!
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
	INTCON1bits.MATHERR = 0;
	Reset();
}

//Something has an unhandled interrupt!
void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
	return;
}
