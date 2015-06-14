/**************************************************************************************************
Target Hardware:
Code assumptions:
Purpose:
Notes:

Version History:
vnext	Y-M-D	Craig Comberbach	Compiler: XC16 v?.??	Optimization: 0	IDE: MPLABx 2.2	Tool: RealICE	Computer: Intel Xeon CPU 3.07 GHz, 6 GB RAM, Windows 7 64 bit Professional SP1
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
#define BREATHE_MAX	500

/*************    Enumeration     ***************/
/***********  Structure Definitions  ************/
/***********State Machine Definitions************/
/*************  Global Variables  ***************/
int lockedOut = 1;

/*************Interrupt Prototypes***************/
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);

/*************Function  Prototypes***************/
/************* Device Definitions ***************/
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
	_FOSC(FCKSM_CSDCMD/*FCKSM_CSECME & SOSCSEL_SOSCLP & POSCFREQ_MS*/ & OSCIOFNC_ON & POSCMOD_NONE)//Clock switchin and fail safe monitor are enabled, Secondary oscilator is low power, Oscillator is between 0.1 and 8 MHz, CLK0 is used as pin IO, The primary oscillator is disabled
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
	int direction = 0;
	int breathe = BREATHE_MAX;

	Configure_For_Capacitive_Control();
	while(1)
	{
		ClrWdt();

		while(!lockedOut)
		{
			//Crudely Breathe the Diagnostic LED
			if(delay++ >= breathe)
			{
				if(direction)
					++breathe;
				else
					--breathe;

				if(breathe <= 0)
					direction = 1;
				else if(breathe >= BREATHE_MAX)
					direction = 0;

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
