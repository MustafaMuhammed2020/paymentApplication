#include "server.h"
#define MAX_LINE_LENGTH 255

ST_transaction_t  transDB[255] = { 0 }; //Global Array for saving the transactions
ST_accountsDB_t accountsDB[255] = { '\0' }; //Global Array for copying the data from Database file
ST_accountsDB_t* p; // Global Pointer pointing at the address of the current account data from database array


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t accountRefrence;
	transData->transState = APPROVED; /*Default State is APPROVED*/
	if (isValidAccount(&transData->cardHolderData, &accountRefrence) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		printf("\nFraud Card\n");
	}
	
	if (transData->transState == APPROVED && isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		printf("\nAccount is blocked\n");
	}
	
	if (transData->transState == APPROVED && isAmountAvailable(&transData->terminalData, &accountRefrence) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		printf("\nInsuffecient funds\n");
	}

	if(transData->transState == APPROVED)
	{
		accountRefrence.balance -= transData->terminalData.transAmount;
		p->balance = accountRefrence.balance;//Updating the balance in database
		printf("\nSuccessful Transaction\n");
	}

	if (saveTransaction(&(*transData)) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		printf("\nSaving failed\n");
	}

	return transData->transState;
}



//void testPrint(char* functionName,int caseNo,char* caseDesc,char* inputData,char* expectedResult,char* actualResult) {
//	printf("Tester Name: Omar Ashraf Taha\nFunction Name: %s\nTestCase %d: %s\nInput Data: %s\nExpected Result: %s\nActual Result: %s\n",functionName,caseNo,caseDesc,inputData,expectedResult,actualResult);
//}


//char* transactionToString(ST_transaction_t* transData,char* str) {
//
//	char line[MAX_LINE_LENGTH] = { 0 };
//	char sn[100];
//
//
//	strcpy(line, "Trans date.: ");
//	strcat(line, transData->terminalData.transactionDate);
//	strcat(line, ","); //add new line at the end of the string
//
//	ltoa(transData->terminalData.transAmount,sn,10);
//	strcat(line, "Trans amount: ");
//	strcat(line, sn); //copies the current string to the string that will be passed to the transactionData.txt
//	strcat(line, ","); //add new line at the end of the string
//
//
//	
//	ltoa(transData->terminalData.maxTransAmount, sn, 10);
//	strcat(line, "Max trans amount: ");
//	strcat(line, sn); //copies the current string to the string that will be passed to the transactionData.txt
//	strcat(line, ","); //add new line at the end of the string
//
//	strcat(line, "cardHolder name: ");
//	strcat(line, transData->cardHolderData.cardHolderName);
//	strcat(line, ","); //add new line at the end of the string
//
//	strcat(line, "PAN: ");
//	strcat(line, transData->cardHolderData.primaryAccountNumber);
//	strcat(line, ","); //add new line at the end of the string
//
//	strcat(line, "Expiry date: ");
//	strcat(line,transData->cardHolderData.cardExpirationDate);
//	strcpy(str, line);
//	return line;
//
//}

//char* enumToString(EN_transState_t state) {
//	return state == 0 ? "APPROVED" : state == 1 ? "DECLINED_INSUFFECIENT_FUND" : state == 2 ? "DECLINED_STOLEN_CARD" : state == 3 ? "FRAUD_CARD" : "INTERNAL_SERVER_ERROR";
//}

//void recieveTransactionDataTest(void)
//{
//	printf("");
//
//	ST_cardData_t cardData = { "Omar Ashraf Taha Abdel","8989374615436851","03/24" };
//	ST_terminalData_t terminalData = { 100.0,5000.0,"01/01/2024" };
//	ST_transaction_t transData = {cardData,terminalData,APPROVED,0};
//	recieveTransactionData(&transData);
//	char strData[255];
//	transactionToString(&transData, strData);
//	testPrint("recieveTransactionData",1,"Accepted Transaction", strData, "APPROVED", enumToString(transData.transState));
//	transData.terminalData.transAmount = 3000;
//	recieveTransactionData(&transData);
//	transactionToString(&transData, strData);
//	testPrint("recieveTransactionData", 2, "Trans amount higher than balance", strData, "DECLINED_INSUFFECIENT_FUND", enumToString(transData.transState));
//}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	for (int i = 0;i < 255 && strlen(accountsDB[i].primaryAccountNumber) != 0;i++) //Checking if the current index in the Database if empty or not
	{
		if (!strcmp(accountsDB[i].primaryAccountNumber,cardData->primaryAccountNumber)) /*strcmp(string1,string2) compares between 2 strings and return 0 if equal*/
		{
			*accountRefrence = accountsDB[i];
			p = &accountsDB[i];//Storing the address of the current account to the global pointer
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

//void isValidAccountTest(void) {
//	ST_cardData_t cardData = { "Omar Ashraf Taha Abdel","8989374615436851","03/24" };
//	ST_accountsDB_t accountRef;
//	EN_serverError_t actualResult = isValidAccount(&cardData, &accountRef);
//	testPrint("isValidAccount", 1, "Testing valid account", "Card PAN: 8989374615436851","SERVER_OK",actualResult == 0 ?"SERVER_OK" :"ACCOUNT_NOT_FOUND");
//
//	*(cardData.primaryAccountNumber) = "3211234567891231";
//	actualResult = isValidAccount(&cardData, &accountRef);
//	testPrint("isValidAccount", 2, "Testing invalid account", "Card PAN: 3211234567891231", "ACCOUNT_NOT_FOUND", actualResult == 0 ? "SERVER_OK" : "ACCOUNT_NOT_FOUND");
//}

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

EN_serverError_t saveTransaction(ST_transaction_t* transData) //Save the Transaction in the global transaction array
{
	for (int i = 0;i < 255;i++)
	{
		if (transDB[i].transactionSequenceNumber == 0)
		{
			transDB[i] = *transData;
			transDB[i].transactionSequenceNumber = (i+1);
			transData->transactionSequenceNumber = (i + 1);
			break;
			//return SERVER_OK;
		}
	}
	listSavedTransactions();
	return SERVER_OK;
	//return SAVING_FAILED;
}

void listSavedTransactions(void)
{

	for (int i = 0;i < 255;i++) 
	{
		if (transDB[i].transactionSequenceNumber > 0) //List the saved transactions in the array 
		{
			printf("###########################################\n");
			printf("Transaction Sequence Number: %d \n",transDB[i].transactionSequenceNumber);
			printf("Transaction Date           : %s \n", transDB[i].terminalData.transactionDate);
			printf("Transaction Amount         : %f \n", transDB[i].terminalData.transAmount);
			if(transDB[i].transState==0)
				printf("Transaction State          : %s \n", "Approved");
			if (transDB[i].transState == 1)
				printf("Transaction State          : %s \n", "DECLINED_INSUFFECIENT_FUND");
			if (transDB[i].transState == 2)
				printf("Transaction State          : %s \n", "DECLINED_STOLEN_CARD");
			if (transDB[i].transState == 3)
				printf("Transaction State          : %s \n", "FRAUD_CARD");
			if (transDB[i].transState == 4)
				printf("Transaction State          : %s \n", "INTERNAL_SERVER_ERROR");
			printf("Terminal Max Amount        : %f \n", transDB[i].terminalData.maxTransAmount);
			printf("Cardholder Name            : %s \n", transDB[i].cardHolderData.cardHolderName);
			printf("PAN                        : %s \n", transDB[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Date       : %s \n", transDB[i].cardHolderData.cardExpirationDate);
			printf("###########################################\n");

		}
		else 
		{
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


void writeTransaction(ST_transaction_t* transData)
{
	char* path = "transactionData.txt";
	char line[MAX_LINE_LENGTH] = { 0 };
	FILE* file = fopen(path, "a");
	char sn[100];

	itoa(transData->transactionSequenceNumber, sn, 10);
	strcpy(line, sn); //Copy the current string to the string that will be passed to the transactionData.txt
	strcat(line, ",");

	strcat(line, transData->terminalData.transactionDate);
	strcat(line, ",");

	ltoa(transData->terminalData.transAmount,sn,10);
	strcat(line, sn); //Concatenate the current string to the string that will be passed to the transactionData.txt
	strcat(line, ",");

	//APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR

	/*itoa(transData->transState, sn, 10);*/
	if (transData->transState == 0)
	{
		strcat(line, "APPROVED");
		strcat(line, ",");
	}
	if (transData->transState == 1)
	{
		strcat(line, "DECLINED_INSUFFECIENT_FUND");
		strcat(line, ",");
	}
	if (transData->transState == 2)
	{
		strcat(line, "DECLINED_STOLEN_CARD");
		strcat(line, ",");
	}
	if (transData->transState == 3)
	{
		strcat(line, "FRAUD_CARD");
		strcat(line, ",");
	}
	if (transData->transState == 4)
	{
		strcat(line, "INTERNAL_SERVER_ERROR");
		strcat(line, ",");
	}
	
	ltoa(transData->terminalData.maxTransAmount, sn, 10);
	strcat(line, sn); 
	strcat(line, ",");

	strcat(line, transData->cardHolderData.cardHolderName); 
	strcat(line, ","); 

	strcat(line, transData->cardHolderData.primaryAccountNumber);
	strcat(line, ","); 

	strcat(line,transData->cardHolderData.cardExpirationDate);
	strcat(line, "\n");//add new line at the end of the string to make sure next entry will be stored at the new line
	
	
	fputs(line, file);
	fclose(file);
}


//void main()
//{
//	getDataBase(accountsDB);
//	if (!strcmp(accountsDB[0].primaryAccountNumber, "898937461543685221"))
//	{
//		printf("FOUND\n");
//	}
//	else {
//		printf("NOT FOUND\n");
//	}
//
//	for(int i=0;i<255 && strlen(accountsDB[i].primaryAccountNumber) != 0;i++)
//	{
//		printf("Balance : %f\nState   : %d\nPAN     : %s\n---------------\n", accountsDB[i].balance, accountsDB[i].state, accountsDB[i].primaryAccountNumber);
//	}
//
//	system("pause");
//	
//}

//void main(){
//	getDataBase(accountsDB);
//	recieveTransactionDataTest();
// 
//	isValidAccountTest();
//	system("pause");
//}