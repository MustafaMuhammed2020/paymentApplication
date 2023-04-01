#include "server.h"
#define MAX_LINE_LENGTH 100

ST_transaction_t  transDB[255] = { 0 };
ST_accountsDB_t accountsDB[255] = { '\0' };






EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t accountRefrence;
	transData->transState = APPROVED; /*Default State is APPROVED*/
	if (isValidAccount(&transData->cardHolderData, &accountRefrence) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
	}
	
	if (transData->transState == APPROVED && isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT)
	{
		transData->transState = DECLINED_STOLEN_CARD;
	}
	
	if (transData->transState == APPROVED && isAmountAvailable(&transData->terminalData, &accountRefrence) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
	}

	if(transData->transState == APPROVED)
	{
		accountRefrence.balance -= transData->terminalData.transAmount;
	}
	
	if (saveTransaction(&transData) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
	}

	return transData->transState;
}

void recieveTransactionDataTest(void)
{
	recieveTransactionData(transDB);
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	for (int i = 0;i < 255 && strlen(accountsDB[i].primaryAccountNumber) != 0;i++)
	{
		if (!strcmp(accountsDB[i].primaryAccountNumber,cardData->primaryAccountNumber)) /*strcmp(string1,string2) compares between 2 strings and return 0 if equal*/
		{
			*accountRefrence = accountsDB[i];
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	return accountRefrence->state == BLOCKED ? BLOCKED_ACCOUNT : SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if ((termData->transAmount) > (accountRefrence->balance))
	{
		return LOW_BALANCE;
	}
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	for (int i = 0;i < 255;i++)
	{
		if (transDB[i].transactionSequenceNumber == 0)
		{
			transDB[i] = *transData;
			transDB[i].transactionSequenceNumber = (i + 1);
			return SERVER_OK;
		}
	}
	return SAVING_FAILED;
}

void listSavedTransactions(void)
{
	for (int i = 1;i < 255;i++) 
	{
		if (transDB[i].transactionSequenceNumber > 0)
		{
			printf("################################\n");
			printf("Transaction Sequence Number: %d \n",transDB[i].transactionSequenceNumber);
			printf("Transaction Date: %d \n", transDB[i].terminalData.transactionDate);
			printf("Transaction Amount: %d \n", transDB[i].terminalData.transAmount);
			printf("Transaction State: %d \n", transDB[i].transState);
			printf("Terminal Max Amount: %d \n", transDB[i].terminalData.maxTransAmount);
			printf("Cardholder Name: %d \n", transDB[i].cardHolderData.cardHolderName);
			printf("PAN: %d \n", transDB[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Date: %d \n", transDB[i].cardHolderData.cardExpirationDate);
			printf("################################\n");

		}
		else {
			break;
		}
	}
}

void getDataBase(ST_accountsDB_t* DB)
{
	char* tok = NULL;
	char* path = "file.txt";
	char line[MAX_LINE_LENGTH] = { 0 };
	int i = 0;

	FILE* file = fopen(path, "r");

	while (fgets(line, MAX_LINE_LENGTH, file)) //Reads each line from the file and assigns each elements to it's corresponding element in the database
	{ 
		
		tok = strtok(line, ","); /*Splits the string to tokens containing the string until the next "," */

		if (tok)
			DB[i].balance = atof(tok); /*Converts string to float*/

		tok = strtok(NULL, ",");

		if (tok)
			DB[i].state = atoi(tok); /*Converts string to integer*/

		tok = strtok(NULL, "\n"); /*Last token is followed by a newline so delimeter is changed to "\n"*/

		if (tok)
			strcpy(&DB[i].primaryAccountNumber, tok); /*Copy the token string to PAN*/
		i++;
	}
	
}


//void main()
//{
//	getDataBase(accountsDB);
//	if (!strcmp(accountsDB[0].primaryAccountNumber, "8989374615436851"))
//	{
//		printf("FOUND\n");
//	}
//	for(int i=0;i<255 && strlen(accountsDB[i].primaryAccountNumber) != 0;i++)
//	{
//		printf("Balance : %f\nState   : %d\nPAN     : %s\n---------------\n", accountsDB[i].balance, accountsDB[i].state, accountsDB[i].primaryAccountNumber);
//	}
//
//	system("pause");
//}