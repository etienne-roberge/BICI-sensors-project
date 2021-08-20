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
#include "project.h"
#include "comm_driver.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
#include <stdlib.h>

uint8 sensorValueBuffer[BUFFER_SIZE];

uint32 readSensor(uint16 sensorAddress, uint8 sensorSize)
{
    uint32 status = TRANSFER_ERROR;
    
    memset(sensorValueBuffer, 0, BUFFER_SIZE);
    (void) I2CM_I2CMasterClearStatus();
    
    uint32 commFlag = I2CM_I2CMasterReadBuf(sensorAddress,
                                    sensorValueBuffer, sensorSize*2,
                                    I2CM_I2C_MODE_COMPLETE_XFER);
    
    if(I2CM_I2C_MSTR_NO_ERROR ==  commFlag)
    {
        /* If I2C read started without errors, 
        / wait until master complete read transfer */
        while(0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT))
        {
            /* Wait */
        }
        
        /* Display transfer status */
        if(0u == (I2CM_I2C_MSTAT_ERR_XFER & I2CM_I2CMasterStatus()))
        {
            uint32 sizeRead = I2CM_I2CMasterGetReadBufSize();
            status = TRANSFER_CMPLT;
        }
    }

    return (status);
}

int main(void)
{
    CyGlobalIntEnable;
    comm_init();

     /* Start the I2C Master */
    I2CM_Start();
    
    for(;;)
    {
        
        /* Read response packet from the slave */
        if (TRANSFER_CMPLT == readSensor(I2C_SLAVE_ADDR, 25))
        {
            // Send it through UART
            comm_putmsg((uint8*)sensorValueBuffer, BUFFER_SIZE);
        }
        
        // Delay (ms)
        CyDelay(50u);
    }
}

/* [] END OF FILE */
