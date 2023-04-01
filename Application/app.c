#include<stdlib.h>
#include<string.h>
#include"card.h"
#include"terminal.h"
#include"server.h"
#include"app.h"

void appStart(void)
{
	ST_cardData_t card;
	ST_terminalData_t terminal;
	ST_transaction_t transaction;
	EN_transState_t error;

	getCardHolderName(&card); /*get the card holder name*/
	getCardExpiryDate(&card);/*get the card expiray date*/
	getCardPAN(&card);/*get the card PAN*/
	
	//strcpy(transaction.cardHolderData.primaryAccountNumber,card.primaryAccountNumber);
	strcpy(transaction.terminalData.transactionDate,terminal.transactionDate);
	strcpy(transaction.cardHolderData.cardExpirationDate,card.cardExpirationDate);
	transaction.terminalData.transAmount=terminal.transAmount;
	strcpy(transaction.cardHolderData.primaryAccountNumber,card.primaryAccountNumber);
	
	if (getTransactionDate(&terminal) == WRONG_DATE)
	{
		printf("Declined\nwrong card date\n");
		exit(0);
	}
	
	
	if (isCardExpired(&card, &terminal) == EXPIRED_CARD)
	{
		printf("Declined\nExpired card\n");
		exit(0);
	}
	
	

	if (getTransactionAmount(&terminal) == INVALID_AMOUNT)
	{
		printf("Declined\nInvalid Amount\n");
		exit(0);
	}
	
	
	if (isBelowMaxAmount(&terminal) == EXCEED_MAX_AMOUNT)
	{
		printf("Declined\nAmount exceeding lemit\n");
		exit(0);
	}
	
	transaction.terminalData.maxTransAmount = terminal.maxTransAmount;
	printf("transaction max amount%f\n",transaction.terminalData.maxTransAmount);
	transaction.terminalData.transAmount=terminal.transAmount;
	strcpy(transaction.cardHolderData.cardHolderName,card.cardHolderName);
	error = recieveTransactionData(&transaction);


	if (error == FRAUD_CARD || error == DECLINED_STOLEN_CARD )
	{
		printf("transaction PAN is %s\n",transaction.cardHolderData.primaryAccountNumber);
		printf("Declined\nInvalid account\n");
		exit(0);
	}
	else if (error == DECLINED_INSUFFECIENT_FUND)
	{
		printf("Declined\nInsuffecient Fund\n");
		exit(0);
	}
	else if (error == SAVING_FAILED)
	{
		printf("Declined\nfailed to save\n");
		exit(0);
	}
	
	
	
	saveTransaction(&transaction);
	listSavedTransactions();
}
