/**************************************************************************************/
/* 						Author   	 : Mark Ehab                                      */
/* 						Date     	 : Aug 18, 2021                                   */
/* 						Description	 : Server                                     	  */
/* 						Version	     : V1.0			                             	  */
/**************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "../LIB/STD_TYPES.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"

ST_transaction_t transactions[255] = {0};

ST_accountsDB_t accountsDB[255] = {
    {35000  , RUNNING , "324879324987423134"},
    {8000   , RUNNING , "239498773726478832"},
    {11500  , BLOCKED , "827148973264873284"},
    {15000  , RUNNING , "186239159294689243"},
    {12355  , BLOCKED , "715974764873268732"},
    {120000 , RUNNING , "932764873268427632"}
};

uint16_t DB_Counter = 0;

ST_accountsDB_t accountRef ;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{

    EN_transState_t status = DECLINED_INSUFFECIENT_FUND;

    if (isValidAccount(&transData->cardHolderData , &accountRef) == ACCOUNT_NOT_FOUND)
    {
        status = FRAUD_CARD;
		return status;
    }

    if (isBlockedAccount(&accountRef) == BLOCKED_ACCOUNT)
    {
        status = DECLINED_STOLEN_CARD;
        return status;
    }

    if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
    {
        // Error status remains DECLINED_INSUFFECIENT_FUND
    }
    
	else
	{
        if(saveTransaction(&transData) == OK_SERVER)
        {
            status = APPROVED;
            accountsDB[DB_Counter].balance -= transData->terminalData.transAmount;
		    printf("Transaction is done successfully !\n");
            printf("You new balance: %f\n", accountsDB[DB_Counter].balance);
        }
        
        else
        {
            status = INTERNAL_SERVER_ERROR;
        }
	}

    return status;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
    EN_serverError_t status = ACCOUNT_NOT_FOUND;

	for (DB_Counter=0; DB_Counter<255 ; DB_Counter++)
	{
		if (!(strcmp(accountsDB[DB_Counter].primaryAccountNumber, cardData->primaryAccountNumber)))
		{
            *accountRefrence = accountsDB[DB_Counter] ;
			status = OK_SERVER;
            break;
		}

        else
        {
            // Error status remains ACCOUNT_NOT_FOUND
        }
	}
	return status;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
    EN_serverError_t status = BLOCKED_ACCOUNT;
    if (accountRefrence->state == RUNNING)
    {
       status = OK_SERVER;
     
    }

    else
    {
        // Error status remains BLOCKED_ACCOUNT
    }
    return status;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData)
{
    EN_serverError_t status = LOW_BALANCE;
    if ((termData->transAmount) <= accountsDB[DB_Counter].balance)
    {
		status = OK_SERVER;
    }

    else
    {
		// Error status remains LOW_BALANCE
    }

    return status;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    EN_serverError_t status = SAVING_FAILED;
    static uint16_t transCounter = 0;

    if(transCounter < 255)
    {
        strcpy(transactions[transCounter].cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
	    strcpy(transactions[transCounter].cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);
	    strcpy(transactions[transCounter].cardHolderData.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber);
	    strcpy(transactions[transCounter].terminalData.transactionDate, transData->terminalData.transactionDate);
    
        transactions[transCounter].terminalData.transAmount = transData->terminalData.transAmount;
        transactions[transCounter].transState = transData->transState;
	    transactions[transCounter].transactionSequenceNumber = ++transCounter;
        status = OK_SERVER;
    }

    else
    {
        // Error status remains SAVING_FAILED
    }

    return status;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    EN_serverError_t status = TRANSACTION_NOT_FOUND;
	for (uint16_t transCounter=0; transCounter<255 ; transCounter++)
	{
		if (transactionSequenceNumber == transactions[transCounter].transactionSequenceNumber)
		{
			status = OK_SERVER;
            *transData = transactions[transCounter];
		}

		else
		{
			// Error status remains TRANSACTION_NOT_FOUND
		}
	}
	return status;
}

