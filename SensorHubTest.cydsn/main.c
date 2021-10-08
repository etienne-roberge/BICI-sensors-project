/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "comm_driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

uint32 counter = 0; // initialize counter for timestamps

CY_ISR (Timer_Int_Handler)
{
    if (Timer_GetInterruptSource() == Timer_INTR_MASK_CC_MATCH)
    {
        counter += Timer_ReadCounter(); // add 16-bit timer count to our 32-bit counter
        Timer_WriteCounter(0); // reset the counter
        Timer_ClearInterrupt(Timer_INTR_MASK_CC_MATCH); // clear the interrupt
    }

    if (Timer_GetInterruptSource() == Timer_INTR_MASK_TC)
    {
       Timer_ClearInterrupt(Timer_INTR_MASK_TC);
    }
    
}

int main(void)
{
    CyGlobalIntEnable;
    comm_init();
    
    /* Start Timer */
    Timer_Start();
    Timer_Int_StartEx(Timer_Int_Handler);

     /* Start the I2C Master */
    I2CM_Start();
    
    //init sensorList structs
    for(uint8 i=0; i<NUMBER_OF_SENSORS; ++i)
    {
        sensorList[i].i2cAddr = sensorAddrList[i];
        sensorList[i].nbTaxels = nbTaxelList[i];
    }
    
    for(;;)
    {
        for(uint8 i=0; i<NUMBER_OF_SENSORS; ++i)
        {
            // add sensor number to data (1 byte)
            uartBuffer[0] = sensorList[i].i2cAddr;
            
            // trigger counter interrupt to get timestamp 
            Timer_SetInterrupt( Timer_INTR_MASK_CC_MATCH );
            
            // add timestamp to data (4 bytes)
            uartBuffer[1] = (uint8)(counter >> 24);
            uartBuffer[2] = (uint8)(counter >> 16);
            uartBuffer[3] = (uint8)(counter >> 8);
            uartBuffer[4] = (uint8)counter;
            
            // add sensor readings to data
            for(uint8 ii=0; ii<sensorList[i].nbTaxels; ++ii)
            {
                uint8 r = rand() % 100;
                uartBuffer[(ii)*2 + MESSAGE_HEADER_SIZE] = 0x05; 
                uartBuffer[(ii)*2 + MESSAGE_HEADER_SIZE + 1] = r; 
            }
            comm_putmsg((uint8*)uartBuffer, (sensorList[i].nbTaxels)*2+MESSAGE_HEADER_SIZE);
            // Delay (ms)
            //CyDelay(50u);
        }
        CyDelay(50u);
    }
    
    // stop timer and associated interrupt
    Timer_Stop();
    Timer_Int_Stop();
     
}

/* [] END OF FILE */