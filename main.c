/****************************************************************************************/
/* Author   : Mustafa Mohammed Abdou                                                    */
/* Date     : 29 Mar , 2023                                                             */
/* Version  : V01                                                                       */
/* Hint     : Header file of terminal module                                            */
/****************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include<stdio.h>

#include"Terminal/terminal.h"
#include"Card/card.h"


int main(void)
{

	ST_terminalData_t t1;

	ST_cardData_t c1; 

	EN_terminalError_t Datestate = TERMINAL_OK , Amountstate = TERMINAL_OK , Maxamountstate = TERMINAL_OK , maxtransamountstate = TERMINAL_OK , cardexpirestatus = TERMINAL_OK;
	
	EN_cardError_t cardstatus = CARD_OK;
	
	do /* must ask for transaction date first */
	{
		Datestate = getTransactionDate(&t1); /* Ask the user for the transaction date */

	} while (Datestate); /* Repeat while the date is valid */


	getCardExpiryDate(&c1);
	

	

	cardexpirestatus = isCardExpired(&c1, &t1);
	
	
	/*
	maxtransamountstate = setMaxAmount(&t1, 5000.0);

	printf("%d\n", maxtransamountstate);

	/*
	do
	{
		cardstatus = getCardHolderName(&c1);

	} while (cardstatus);
	

	printf("%d", cardstatus);


	/*
	do
	{
		Datestate = getTransactionDate(&t1); /* Ask the user for the transaction date 

	} while(Datestate); /* Repeat while the date is valid 
	
	*/
	/*
	do
	{
		Amountstate = getTransactionAmount(&t1) ;  /* Ask the user for the transaction amount 

		Maxamountstate = isBelowMaxAmount(&t1) ;  /* Ceck the amount valid or not 

	} while (Maxamountstate);
	

	printf("%f", t1.transAmount);
	*/
	for (;;)
	{

	}
	return 0;
}