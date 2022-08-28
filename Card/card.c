/**************************************************************************************/
/* 						Author   	 : Mark Ehab                                      */
/* 						Date     	 : Aug 18, 2021                                   */
/* 						Description	 : Card                                        	  */
/* 						Version	     : V1.0			                             	  */
/**************************************************************************************/

#include "../LIB/STD_TYPES.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    EN_cardError_t status = WRONG_NAME;
    uint8_t holderName[25] = "";

    printf("Please enter card holder name keeping in mind that name must be (20-24 characters): ");
    gets(holderName);
    if((holderName == "\0") || (strlen(holderName)<20) || (strlen(holderName)>24))
    {
        //Do not store the card holder name and the status error remains WRONG_NAME
    }

    else
    {
        status = OK_CARD;

        /*
            strcpy function in string.h library takes the destination array that stores 
            string and the source array of string to copy to the destination including 
            the null character (\0)
         */
        strcpy(cardData->cardHolderName, holderName);
    }

    return status;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t status = WRONG_EXP_DATE;
    uint8_t expDate[6] = "";

    printf("Please enter the card expiry date in the format of (MM/YY) as shown in your card: ");
    gets(expDate);

    if((strlen(expDate)==5)
        && ((expDate[0]=='0' && expDate[1]>'0' && expDate[1]<='9') || (expDate[0]=='1' && expDate[1]>='0' && expDate[1]<'3'))
        && (expDate[2]=='/')
        && (expDate[3]>'1' && expDate[3]<='9' && expDate[4]>='0' && expDate[4]<='9')
        )
    {
        status = OK_CARD;
        strcpy(cardData->cardExpirationDate, expDate);
    }

    else
    {
        //Do not store the card Expiration Date and the status error remains WRONG_EXP_DATE
    }

    return status;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{

    EN_cardError_t status = WRONG_PAN;
    uint8_t PAN[20] = "";

    printf("Please enter the PAN and must be (16-19 alphanumeric characters): ");
    gets(PAN);

    if(strlen(PAN)<16 || strlen(PAN)>19)
    {
        //Do not store the card PAN and the status error remains WRONG_PAN
    }

    else
    {
        status = OK_CARD;
        strcpy(cardData->primaryAccountNumber, PAN);
    }

    return status;
}
