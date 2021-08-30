/* ========================================
 *
//Hello
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

#define TAXEL_COUNT         (25u)

/* The I2C Slave read and write buffers */
uint16 i2cReadBuffer [TAXEL_COUNT];
uint16 i2cWriteBuffer[BUFFER_SIZE];

int main(void)
{    
    CyGlobalIntEnable;
    __enable_irq(); /* Enable global interrupts. */
    
    I2C_Start();
    
    /* 
    * Set up communication and initialize data buffer to CapSense data 
    * structure to use Tuner application 
    
    I2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam.snsList.button0),
                        sizeof(CapSense_dsRam.snsList.button0),
                        (uint8_t *)&CapSense_dsRam.snsList.button0);
    */
    I2C_EzI2CSetBuffer1(sizeof(i2cReadBuffer),
                        sizeof(i2cReadBuffer),
                        (uint8_t *)&i2cReadBuffer);
    
    
    CapSense_Start();
    CapSense_ScanAllWidgets();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        if(!CapSense_IsBusy())
        {
            CapSense_ProcessAllWidgets();
            CapSense_RunTuner();
            CapSense_UpdateAllBaselines();
            CapSense_ScanAllWidgets(); 
        
            for(unsigned int i=0; i<TAXEL_COUNT; ++i)
            {
                i2cReadBuffer[i] = CapSense_dsRam.snsList.button0[i].raw[0];
            }
        }
        
    }
}

/* [] END OF FILE */
