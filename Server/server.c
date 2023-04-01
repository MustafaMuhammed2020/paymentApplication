#include "server.h"
#define MAX_LINE_LENGTH 100


	//accountsDB[0].balance=10000.0;
	//accountsDB[0].balance = 10000.0;

//ST_accountsDB_t account1 = { 2000.0, RUNNING, "8989374615436851" };
//ST_accountsDB_t account2 = { 2000.0, RUNNING, "8989374615436851" };
//ST_accountsDB_t account3 = { 2000.0, RUNNING, "8989374615436851" };
//ST_accountsDB_t account4 = { 2000.0, RUNNING, "8989374615436851" };
//ST_accountsDB_t account5 = { 100000.0, BLOCKED, "5807007076043875" };

ST_transaction_t  transDB[255]={0};
ST_accountsDB_t accountsDB[255]={0};



EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	//scanf("%f",transData->terminalData.transAmount);
	
	
	
	
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
	
	
	/*if (saveTransaction(transData) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
	}*/

	return transData->transState;
}

/*void recieveTransactionDataTest(void)
{
	recieveTransactionData(transDB);
}*/

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	for (int i = 0;i < 255 && accountsDB[i].primaryAccountNumber != '\0';i++)
	{
		if (strcmp(accountsDB[i].primaryAccountNumber, cardData->primaryAccountNumber) == 0)
		{
			accountRefrence->balance = accountsDB[i].balance;
			accountRefrence->state = accountsDB[i].state;
			strcpy(accountRefrence->primaryAccountNumber, accountsDB[i].primaryAccountNumber);
			
			
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
		printf("accountsDB[0].balance=%d \n",accountsDB[0].balance);
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
			
			strcpy(transDB[i].terminalData.transactionDate, transData->terminalData.transactionDate);
			transDB[i].terminalData.transAmount=transData->terminalData.transAmount;
			transDB[i].transState=transData->transState;
			transDB[i].terminalData.maxTransAmount=transData->terminalData.maxTransAmount;
			strcpy(transDB[i].cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
			strcpy(transDB[i].cardHolderData.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber);
			strcpy(transDB[i].cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);
			transDB[i].terminalData.transAmount = transData->terminalData.transAmount;
			//transDB[i] = *transData;
			transDB[i].transactionSequenceNumber = (i + 1);
			return SERVER_OK;
		}
	}
	return SAVING_FAILED;
}

void listSavedTransactions(void)
{
	for (int i = 0;i < 255;i++) 
	{
		if (transDB[i].transactionSequenceNumber > 0)
		{
			printf("################################\n");
			printf("Transaction Sequence Number: %d \n",transDB[i].transactionSequenceNumber);
			printf("Transaction Date: %s \n", transDB[i].terminalData.transactionDate);
			printf("Transaction Amount: %f \n", transDB[i].terminalData.transAmount);
			if(transDB[i].transState == APPROVED)
				printf("Transaction State: APPROVED \n");
			printf("Terminal Max Amount: %f \n", transDB[i].terminalData.maxTransAmount);
			printf("Cardholder Name: %s \n", transDB[i].cardHolderData.cardHolderName);
			printf("PAN: %s \n", transDB[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Date: %s \n", transDB[i].cardHolderData.cardExpirationDate);
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
