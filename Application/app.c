/**************************************************************************************/
/* 						Author   	 : Mark Ehab                                      */
/* 						Date     	 : Aug 18, 2021                                   */
/* 						Description	 : Application                               	  */
/* 						Version	     : V1.0			                             	  */
/**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../LIB/STD_TYPES.h"
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

void appStart(void)
{
    uint8_t repeat = '\0';
    EN_transState_t trans_status;
	ST_transaction_t trans_data;
	while(setMaxAmount(&trans_data.terminalData) == INVALID_MAX_AMOUNT)
    {
    	printf("Sorry,maximum amount must be greater than 0\n");
    }
    /*
     Read the new line character stored in input buffer after scanf is ended to avoid
     being read by another following scanf that may cause a bug in the program.
     */
    newLineChar = getchar();
	do
    {
    	while (getCardHolderName(&trans_data.cardHolderData) == WRONG_NAME)
    	{
    		printf("Wrong Name!\n");
    	}

    	while(getCardExpiryDate(&trans_data.cardHolderData) == WRONG_EXP_DATE)
    	{
    		printf("Wrong Date!\n");
    	}

    	while(getCardPAN(&trans_data.cardHolderData) == WRONG_PAN)
    	{
    		printf("Wrong PAN!\n");
    	}

        getTransactionDate(&trans_data.terminalData);

    	if(isCardExpired(trans_data.cardHolderData, trans_data.terminalData) == EXPIRED_CARD)
    	{
    		printf("Access Declined !...Your card is expired...Please Renew It !\n");
    		continue;
    	}

    	while(getTransactionAmount(&trans_data.terminalData) == INVALID_AMOUNT)
    	{
    		printf("Invalid Amount !, please enter a valid amount\n");
    	}

    	if(isBelowMaxAmount(&trans_data.terminalData) == EXCEED_MAX_AMOUNT)
    	{
    		printf("Transaction Declined !, amount exceeding limit!\n");
            /*
            Read the new line character stored in input buffer after scanf is ended to avoid
            being read by another following scanf that may cause a bug in the program.
            */
            newLineChar = getchar();
    		continue;
    	}

        trans_status = recieveTransactionData(&trans_data);

        switch (trans_status)
        {
            case DECLINED_STOLEN_CARD:
                printf("Access Declined !, invalid account\n");
                break;

            case DECLINED_INSUFFECIENT_FUND:
                printf("Transaction Declined !, insufficient funds\n");
                break;

            case INTERNAL_SERVER_ERROR:
                printf("Transaction Declined !, Internal server error\n");
                break;
        
            case APPROVED:
                printf("\t\t<<< End of a successful transaction >>>\t\t\n");
                break;
        }
        /* 
        Read the new line character stored in input buffer after scanf is ended to avoid 
        being read by another following scanf that may cause a bug in the program.
        */
        newLineChar = getchar();
        printf("Enter f to finish the process or any other key to proceed a new transaction: ");
        scanf("%c",&repeat);
        /*
        Read the new line character stored in input buffer after scanf is ended to avoid
        being read by another following scanf that may cause a bug in the program.
        */
        newLineChar = getchar();
    } while (repeat != 'f');

}
