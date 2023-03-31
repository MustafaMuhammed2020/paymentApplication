#include "server.h"

ST_transaction_t  transDB[255] = { 0 };
ST_accountsDB_t accountsDB[255];

ST_accountsDB_t account1 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account2 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account3 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account4 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account5 = { 100000.0, BLOCKED, "5807007076043875" };

void main() 
{
	accountsDB[0] = account1;
	accountsDB[1] = account2;
	accountsDB[2] = account3;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t accountRefrence;
	isValidAccount(transData->cardHolderData,&accountRefrence);
	isBlockedAccount(&accountRefrence);
	isAmountAvailable(transData->terminalData,&accountRefrence);
	saveTransaction();
	return ;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	for(int i=0;i<255;i++)
	{
		if (accountsDB[i].primaryAccountNumber == (cardData->primaryAccountNumber))
		{
			accountRefrence = accountsDB[i];
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	accountsDB[accountRefrence].primaryAccountNumber
	return ;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	return ;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	return ;
}

void listSavedTransactions(void)
{
}
