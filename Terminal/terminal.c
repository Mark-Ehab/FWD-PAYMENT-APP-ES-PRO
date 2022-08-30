/**************************************************************************************/
/* 						Author   	 : Mark Ehab                                      */
/* 						Date     	 : Aug 18, 2021                                   */
/* 						Description	 : Terminal                                    	  */
/* 						Version	     : V1.0			                             	  */
/**************************************************************************************/

#include "../LIB/STD_TYPES.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    int day, mon, year;
    time_t sysTime;
    struct tm* sysDate;
    EN_terminalError_t status = OK_TERMINAL;
    uint8_t transDate[11]="";
    /* Set the current system date as transaction date */
    time(&sysTime);
    sysDate = localtime(&sysTime);
    /*
      localtime() function returns a pointer to tm struct and this struct includes many attributes
      but we will concern only three of them as follows :
      1) tm_mday ==> day of the month [1,31]
      2) tm_mon  ==> months since Jan. [0,11] where 0 indicates Jan and 11 indicates Dec
      3) tm_year ==> years since 1900
      So our current system day , month and year will be as follows
     */
    day  = sysDate->tm_mday;
    mon  = sysDate->tm_mon+1;      // Add 1 to month offset to get the right month
    year = sysDate->tm_year+1900;  // Add 1900 to years offset to get the right year

    /* Set transaction date on the format DD/MM/YY */
    for (uint8_t dateFormatCounter = 0; dateFormatCounter < 10; dateFormatCounter++)
    {
        if (dateFormatCounter == 0 || dateFormatCounter == 1)
        {
            if (dateFormatCounter == 0)
            {
               transDate[dateFormatCounter + 1] = day % 10 + '0';
            }
            else
            {
               transDate[dateFormatCounter - 1] = day % 10 + '0';
            }
            day /= 10;
        }
       else if (dateFormatCounter == 2 || dateFormatCounter == 5)
        {
            transDate[dateFormatCounter] = '/';
        }
       else if (dateFormatCounter == 3 || dateFormatCounter == 4)
        {
            if (dateFormatCounter == 3)
            {
                transDate[dateFormatCounter + 1] = mon % 10 + '0';
            }
            else
            {
                transDate[dateFormatCounter - 1] = mon % 10 + '0';
            }
            mon /= 10;
        }
       else
        {
            if (dateFormatCounter == 6)
            {
                transDate[dateFormatCounter + 3] = year % 10 + '0';
            }
            else if(dateFormatCounter == 7)
            {
                transDate[dateFormatCounter + 1] = year % 10 + '0';
            }
            else if(dateFormatCounter == 8)
            {
                transDate[dateFormatCounter - 1] = year % 10 + '0';
            }
            else
            {
                transDate[dateFormatCounter - 3] = year % 10 + '0';
            }
            year /= 10;
        }
    }
    transDate[10] = '\0';
    strcpy(termData->transactionDate, transDate);
    return status;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    EN_terminalError_t status = EXPIRED_CARD;

    uint16_t expYear, transYear;
    uint8_t expMonth, transMonth;

    /*
     Calculate the expiration year from the string MM/YY
     Note add 2000 to the year as the expireation year on the car is written the the last 2 numbers of the year
     */
    expYear = (((cardData.cardExpirationDate[3] - '0' ) * 10) + (cardData.cardExpirationDate[4] - '0' ))+ 2000;
    
    /* Calculate the expirtion month from the string MM/YY */
    expMonth = (cardData.cardExpirationDate[0] - '0') * 10 + ( cardData.cardExpirationDate[1] - '0');

    /* Calculate the transaction year from the string DD/MM/YYYY */
    transYear = (((termData.transactionDate[6] - '0' ) * 1000) + ((termData.transactionDate[7] - '0' ) * 100)
                  + ((termData.transactionDate[8] - '0' ) * 10) + (termData.transactionDate[9] - '0' ));

    /* Calculate the transaction month from the string DD/MM/YYYY */
    transMonth = (((termData.transactionDate[3] - '0') * 10) + (termData.transactionDate[4] - '0'));

    if((expYear>=transYear))
    {
        if((expYear==transYear) && (expMonth<transMonth))
        {
            // Error status remains EXPIRED_CARD
        }

        else
        {
            status = OK_TERMINAL;
        }
    }

    else
    {
        // Error status remains EXPIRED_CARD
    }

    return status;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    
    return OK_TERMINAL;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t status = INVALID_AMOUNT;
    f32 transAmount = 0;

    printf("Please enter transaction amount: ");
    scanf("%f",&transAmount);

    if (transAmount<=0)
    {
        //Error status remains INVALID_AMOUNT
    }

    else
    {
        status = OK_TERMINAL;
        termData->transAmount = transAmount;
    }

    return status;

}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t status = EXCEED_MAX_AMOUNT;

    if(termData->transAmount<=termData->maxTransAmount)
    {
        status = OK_TERMINAL;
    }

    else
    {
        //Error status remains EXCEED_MAX_AMOUNT
    }
    
    return status;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t status = INVALID_MAX_AMOUNT;
    f32 maxAmount = 0;

    printf("Please enter maximum transaction amount: ");
    scanf("%f",&maxAmount);

if (maxAmount>0)
    {
        status = OK_TERMINAL;
        termData->maxTransAmount = maxAmount;
    }
    
else
   {
        // Error status remains INVALID_MAX_AMOUNT and don't set maximum amount
   }

    return status;
}

