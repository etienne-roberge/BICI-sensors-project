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

uint8 sensorValueBuffer[BUFFER_SIZE+1];

int main(void)
{
    CyGlobalIntEnable;
    comm_init();

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
            sensorValueBuffer[0] = sensorList[i].i2cAddr;
            for(uint8 ii=0; ii<sensorList[i].nbTaxels; ++ii)
            {
                uint8 r = rand() % 100;
                sensorValueBuffer[(ii)*2 + 1] = 0x05; 
                sensorValueBuffer[(ii)*2 + 2] = r; 
            }
            comm_putmsg((uint8*)sensorValueBuffer, (sensorList[i].nbTaxels)*2+1);
            // Delay (ms)
            CyDelay(200u);
        }
        CyDelay(100u);
    }
     
}

/* [] END OF FILE */