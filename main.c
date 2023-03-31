/****************************************************************************************/
/* Author   : Mustafa Mohammed Abdou                                                    */
/* Date     : 29 Mar , 2023                                                             */
/* Version  : V01                                                                       */
/* Hint     : Header file of terminal module                                            */
/****************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS



#include"Terminal/terminal.h"
#include"Card/card.h"


int main(void)
{

	ST_terminalData_t t1;

	EN_terminalError_t Datestate = TERMINAL_OK , Amountstate = TERMINAL_OK , Maxamountstate = TERMINAL_OK ;
	/*
	do
	{
		Datestate = getTransactionDate(&t1); /* Ask the user for the transaction date 

	} while(Datestate); /* Repeat while the date is valid 
	
	*/
	do
	{
		Amountstate = getTransactionAmount(&t1) ;  /* Ask the user for the transaction amount */

		Maxamountstate = isBelowMaxAmount(&t1) ;  /* Ceck the amount valid or not */

	} while (Maxamountstate);
	

	printf("%f", t1.transAmount);

	for (;;)
	{

	}
	return 0;
}