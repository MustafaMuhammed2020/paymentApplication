#include "app.h"

void appStart(int x)
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
	if (panState) //Check if the PAN Number is Valid
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
	transaction.terminalData.maxTransAmount = terminal.maxTransAmount;
	do
	{
		Amountstate = getTransactionAmount(&terminal);  // Ask the user for the transaction amount

		Maxamountstate = isBelowMaxAmount(&terminal) ;  // Ceck the amount valid or not

	} 
	while (Maxamountstate!=TERMINAL_OK);


	transaction.terminalData.transAmount = terminal.transAmount;
	strcpy(transaction.cardHolderData.cardHolderName, card.cardHolderName);
	getDataBase(accountsDB);
	error = recieveTransactionData(&transaction);
	writeTransaction(transDB[x]);



}

void main()
{
	uint8_t cont = 'y';
	int i = 0;
	while (cont != 'q')
	{
		appStart(i);

		printf("\nPress q if u wish to quit program\n");
		cont = getch();
		cont = tolower(cont);
		i++;
	}
	//appStart();


	//system("pause");
}