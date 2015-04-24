/**************************************************************************************************
Target Hardware:
Code assumptions:
Purpose:
Notes:

Version History:
vnext	Y-M-D	Craig Comberbach	Compiler: XC16 v?.??	Optimization: 0	IDE: MPLABx 2.1	Tool: RealICE	Computer: Intel Xeon CPU 3.07 GHz, 6 GB RAM, Windows 7 64 bit Professional SP1
	First version
**************************************************************************************************/
/*************    Header Files    ***************/
#include <p24F16KA101.h>
#include "Pins.h"
#include "Config.h"
#include "A2D.h"

/************* Library Definition ***************/
/************* Semantic Versioning***************/
/************Arbitrary Functionality*************/
/*************   Magic  Numbers   ***************/
/*************    Enumeration     ***************/
/***********  Structure Definitions  ************/
/***********State Machine Definitions************/
/*************  Global Variables  ***************/
int lockedOut = 1;

/*************Interrupt Prototypes***************/
void __attribute__((interrupt, auto_psv)) _OscillatorFail(void);
void __attribute__((interrupt, auto_psv)) _AddressError(void);
void __attribute__((interrupt, auto_psv)) _StackError(void);
void __attribute__((interrupt, auto_psv)) _MathError(void);
void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void);
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);

/*************Function  Prototypes***************/
/************* Device Definitions ***************/
#define Reset()	asm("reset")

/************* Module Definitions ***************/
/************* Other  Definitions ***************/

//Config bits
#ifdef REFRENCEBOARD
	//Used on Reference Board
//FBS
	_FGS(GWRP_OFF & GCP_OFF)
//_FOSCSEL(IESO_OFF & FNOSC_FRC)//Borrowed from the actual board
//	_FOSCSEL(IESO_OFF & FNOSC_PRI)//From the reference board
	_FOSCSEL(IESO_OFF & FNOSC_FRCDIV)//My concoction
	_FOSC(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS)
	_FWDT(WINDIS_OFF & FWDTEN_ON & FWPSA_PR32 & WDTPS_PS4)
//FPOR
	_FICD(BKBUG_OFF & COE_OFF & ICS_PGx3)
//FDS
#else
	_FBS(BWRP_OFF & BSS_OFF)	//Boot Segement is not write protected and no security is in place
	_FGS(GWRP_OFF & GCP_OFF)	//General Segement is not write protected and no security is in place
	_FOSCSEL(IESO_OFF & FNOSC_FRC) //Two speed start up disabled, and FRC used as the main clock
//	_FOSCSEL(IESO_OFF & FNOSC_FRCPLL) //Try this one some day!
	_FOSC(FCKSM_CSDCMD/*FCKSM_CSECME & SOSCSEL_SOSCLP & POSCFREQ_MS*/ & OSCIOFNC_OFF & POSCMOD_HS)//Clock switchin and fail safe monitor are enabled, Secondary oscilator is low power, Oscillator is between 0.1 and 8 MHz, CLK0 is used as pin IO, The primary oscillator is HS
	_FWDT(FWDTEN_ON & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)
//	_FWDT(FWDTEN_ON & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS4)//Non-Windowed WDT, Nominal period is 4 mS
	_FPOR(MCLRE_ON & BORV_V27 & I2C1SEL_PRI & PWRTEN_ON & BOREN_BOR3)	//MCLR is used as MCLR, BOR occurs at 2.7V, I2C lines are default location, Power-Up timer is on (50-90mS, typical 64mS), BOR is hardware controlled only
	_FICD(ICS_PGx1 & COE_OFF & BKBUG_OFF)
	_FDS(DSWDTEN_OFF & DSBOREN_ON & RTCOSC_LPRC & DSWDTOSC_LPRC & DSWDTPS_DSWDTPS5)
#endif

//===============================================================================================
int main()
{
	int delay = 0;
	Configure_For_Capacitive_Control();
	while(1)
	{
		ClrWdt();

		while(!lockedOut)
		{
			if(delay++ >= 10)
			{
				Pin_Toggle(PIN_DIAGNOSTIC_LED);
				delay = 0;
			}

			A2D_Routine();
			//Wait to for timing purposes
			lockedOut = 1;
		}
	}
	
	return 0;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
	lockedOut = 0;
	TMR1 = 0;
	IFS0bits.T1IF = 0;
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
