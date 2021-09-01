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

#include <main.h>

#define TAXEL_COUNT         (25)
#define READY_READ          (0xFFFE)
#define WAITING_FOR_MASTER  (0)
#define SLAVE_STATE_BYTE    (0)

/* The I2C Slave read and write buffers */
uint16 i2cReadBuffer [TAXEL_COUNT+1];

#define WRITE_BUFFER_SIZE (1)
uint8 i2cWriteBuffer[WRITE_BUFFER_SIZE];


int main(void)
{    
    CyGlobalIntEnable;
    __enable_irq(); /* Enable global interrupts. */
    
    /* Start the I2C Slave */
    I2C_I2CSlaveInitReadBuf ((uint8 *)i2cReadBuffer,  TAXEL_COUNT*2);
    I2C_I2CSlaveInitWriteBuf(i2cWriteBuffer, WRITE_BUFFER_SIZE);
    I2C_Start();
    
    CapSense_Start();
    CapSense_ScanAllWidgets();
    
    uint8 readCapSenseFlag = 0;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        uint32 tt = I2C_I2CSlaveStatus();
        /* Write complete: parse the command packet */
        if (0u != (tt & I2C_I2C_SSTAT_WR_CMPLT))
        {
            int test = I2C_I2CSlaveGetWriteBufSize();
            /* Check the packet length */
            if (WRITE_BUFFER_SIZE == test)
            {
                /* Check the start and end of packet markers */
                if(i2cWriteBuffer[0] == 1)
                {   
                    i2cReadBuffer[SLAVE_STATE_BYTE] = WAITING_FOR_MASTER;
                    readCapSenseFlag = 1;
                    i2cWriteBuffer[0] = 0 ;
                }
                else
                {
                    //ohoh spaghattiyo
                }
            }
            
            /* Clear the slave write buffer and status */
            I2C_I2CSlaveClearWriteBuf();
            (void) I2C_I2CSlaveClearWriteStatus();       
        }
        
        if(readCapSenseFlag == 1)
        {
            if(!CapSense_IsBusy())
            {
                CapSense_ProcessAllWidgets();
                CapSense_RunTuner();
                CapSense_ScanAllWidgets(); 
            
                for(unsigned int i=0; i<TAXEL_COUNT; ++i)
                {
                    i2cReadBuffer[i+1] = CapSense_dsRam.snsList.button0[i].raw[0];
                }
                
                i2cReadBuffer[SLAVE_STATE_BYTE] = READY_READ;
                readCapSenseFlag = WAITING_FOR_MASTER;
            }
        }
        
        
        /* Read complete: expose buffer to master */
        if (0u != (tt & I2C_I2C_SSTAT_RD_CMPLT))
        {
            /* Clear the slave read buffer and status */
            I2C_I2CSlaveClearReadBuf();
            (void) I2C_I2CSlaveClearReadStatus();
        }
        
    }
}

/* [] END OF FILE */
