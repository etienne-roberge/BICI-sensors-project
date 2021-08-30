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

uint8 sensorValueBuffer[BUFFER_SIZE];

uint32 readSensor(uint16 sensorAddress, uint8 sensorSize)
{
    uint32 status = TRANSFER_ERROR;
    
    memset(sensorValueBuffer, 0, BUFFER_SIZE);
    (void) I2CM_I2CMasterClearStatus();
    
    uint8 slaveReadyFlag[2];
    
    uint32 commFlag = I2CM_I2CMasterReadBuf(sensorAddress,
                                    slaveReadyFlag, 2,
                                    I2CM_I2C_MODE_COMPLETE_XFER);
    
    if(I2CM_I2C_MSTR_NO_ERROR ==  commFlag)
    {
        /* If I2C read started without errors, 
        / wait until master complete read transfer */
        while(0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT))
        {
            /* Wait */
        }
        
        if(slaveReadyFlag[1] != 1)
        {
           return SLAVE_NOT_READY;
        }
    }
    
    commFlag = I2CM_I2CMasterReadBuf(sensorAddress,
                                    sensorValueBuffer, sensorSize*2+1,
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

uint32 startCapSenseAcquisition()
{
    uint8  buffer[BUFFER_SIZE] = {1};
    uint8 test[1] = {1};
    uint32 status = TRANSFER_ERROR;

    (void) I2CM_I2CMasterClearStatus();
    
    /* Start I2C write and check status*/
    if(I2CM_I2C_MSTR_NO_ERROR == I2CM_I2CMasterWriteBuf(0x00,
                                    test, 1,
                                    I2CM_I2C_MODE_COMPLETE_XFER))
    {
        /*If I2C write started without errors, 
        / wait until I2C Master completes write transfer 
        */
        while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT))
        {
            /* Wait */
        }
        
        /* Display transfer status */
        if (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_ERR_XFER))
        {
            /* Check if all bytes was written */
            if (I2CM_I2CMasterGetWriteBufSize() == BUFFER_SIZE)
            {
                status = TRANSFER_CMPLT;
            }
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
    
    //init sensorList structs
    for(uint8 i=0; i<NUMBER_OF_SENSORS; ++i)
    {
        sensorList[i].i2cAddr = sensorAddrList[i];
        sensorList[i].nbTaxels = nbTaxelList[i];
    }
    
    
    for(;;)
    {
        
        //Send acquisition command
        startCapSenseAcquisition();
        
        /* Read response packet from the slave */
        if (TRANSFER_CMPLT == readSensor(0x16, 25))
        {
            // Send it through UART
            comm_putmsg((uint8*)sensorValueBuffer, BUFFER_SIZE);
        }
        
        // Delay (ms)
        CyDelay(50u);
    }
}

/* [] END OF FILE */
