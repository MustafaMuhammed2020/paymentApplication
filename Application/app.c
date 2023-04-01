#include "app.h"

void appStart(void)
{
	ST_cardData_t card;
	ST_terminalData_t terminal;
	EN_terminalError_t Maxamountstate = TERMINAL_OK;
	EN_terminalError_t Amountstate = TERMINAL_OK;
	ST_transaction_t transaction;
	EN_transState_t error;
	EN_cardError_t nameState,dateState,panState = 0;
	
	do
	{
		nameState = getCardHolderName(&card); /*get the card holder name*/
		
	} while (nameState);

	do
	{
		dateState=getCardExpiryDate(&card);/*get the card expiray date*/

	} while (dateState);

	panState=getCardPAN(&card);/*get the card PAN*/
	if (panState)
	{
		printf("\nWrong PAN!\n");
		return;
	}



	strcpy(transaction.cardHolderData.cardExpirationDate, card.cardExpirationDate);
	strcpy(transaction.cardHolderData.primaryAccountNumber, card.primaryAccountNumber);

	if (getTransactionDate(&terminal) == WRONG_DATE)
	{
		printf("Declined\nwrong card date\n");	
	}

	strcpy(transaction.terminalData.transactionDate, terminal.transactionDate);


	if (isCardExpired(&card, &terminal) == EXPIRED_CARD)
	{
		printf("Declined\nExpired card\n");
		return;
	}

	setMaxAmount(&terminal, 5000.00);
	do
	{
		Amountstate = getTransactionAmount(&terminal);  // Ask the user for the transaction amount

		Maxamountstate = isBelowMaxAmount(&terminal) ;  // Ceck the amount valid or not

	} while (Maxamountstate!=TERMINAL_OK);


	transaction.terminalData.maxTransAmount = terminal.maxTransAmount;
	transaction.terminalData.transAmount = terminal.transAmount;
	strcpy(transaction.cardHolderData.cardHolderName, card.cardHolderName);
	getDataBase(accountsDB);
	error = recieveTransactionData(&transaction);



	while(1)
	{
		Amountstate = getTransactionAmount(&terminal);
		transaction.terminalData.transAmount = terminal.transAmount;
		error = recieveTransactionData(&transaction);
	}

}

void main()
{
	appStart();

	system("pause");
}