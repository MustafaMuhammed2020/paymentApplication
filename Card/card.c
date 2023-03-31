
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){

	printf("\nPlease, Enter your card name: ");
	fflsh(stdout);
	scanf("%s",&cardData->cardHolderName);

	uint32_t len_name = strlen(cardData->cardHolderName);

	if(len_name > 24 || len_name < 20 || len_name == NULL)
		return WRONG_NAME ;

	return CARD_OK;
}



EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){

	printf("\nPlease, Enter the Expire date (MM/YY): ");
	fflsh(stdout);
	scanf("%s",&cardData->cardExpirationDate);

	uint32_t len_date = strlen(cardData->cardExpirationDate);

	if(len_date != 5 || cardData->cardExpirationDate[2]!= '/' || len_date == NULL)
		return WRONG_EXP_DATE ;

	return CARD_OK;
}



EN_cardError_t getCardPAN(ST_cardData_t *cardData){

	printf("\nPlease, Enter your primaryAccountNumber: ");
	fflsh(stdout);
	scanf("%s",&cardData->primaryAccountNumber);

	uint32_t len_PAN = strlen(cardData->primaryAccountNumber);

	if(len_PAN >19 || len_PAN <16 || len_PAN == NULL)
		return WRONG_PAN ;

	return CARD_OK;
}
