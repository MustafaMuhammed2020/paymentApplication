#include "server.h"


ST_transaction_t  transDB[255] = { 0 };
ST_accountsDB_t accountsDB[255];

ST_accountsDB_t account1 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account2 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account3 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account4 = { 2000.0, RUNNING, "8989374615436851" };
ST_accountsDB_t account5 = { 100000.0, BLOCKED, "5807007076043875" };




EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t accountRefrence;
	transData->transState = APPROVED;
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
	for (int i = 0;i < 255 && accountsDB[i].primaryAccountNumber != '\0';i++)
	{
		if (accountsDB[i].primaryAccountNumber == (cardData->primaryAccountNumber))
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

//ST_accountsDB_t getDataBase(ST_accountsDB_t* DB)
//{
//	FILE* infile;
//	infile = fopen("Project_Database.txt", "r");
//	if (infile == NULL) {
//		fprintf(stderr, "\nError opening file\n");
//		exit(1);
//	}
//	fread(accountsDB, sizeof(accountsDB), 1, infile);
//}

//
//int readTxtFile(const char* filename, ST_accountsDB_t* accounts) {
//	FILE* fin = fopen(filename, "r");
//	if (!fin) {
//		printf("Can't open file: %s\n", filename);
//		return 0;
//	}
//	int i = 0;
//
//	char* lineptr = NULL;
//	size_t size = 0;
//	/*
//	ssize_t getline(char **lineptr, size_t *n, FILE *stream);
//	while (fscanf(fin, "%d%s %d",
//				  &students[i].stNo,
//				  students[i].name,
//				  &students[i].points))
//				  */
//
//
//	while (getline(&lineptr, &size, fin) != -1)
//	{
//		parse_accounts(lineptr, &accounts[i].balance, &accounts[i].primaryAccountNumber, &accounts[i].state);
//		i++;
//	}
//	free(lineptr);
//	lineptr = NULL;
//	fclose(fin);
//	return i;
//}

//void parse_accounts(char* line, float* balance, uint8_t* pan, EN_accountState_t* state)
//{
//	char* tok = NULL;
//
//	tok = strtok(line, ",");
//	if (tok)
//		*balance = atoi(tok);
//
//	tok = strtok(NULL, ",");
//	if (tok)
//		strcpy(pan, tok);
//
//	tok = strtok(NULL, ",");
//	if (tok)
//		*state = atoi(tok);
//}
//
//void main() {
//	readTxtFile("file.txt", &accountsDB);
//}
