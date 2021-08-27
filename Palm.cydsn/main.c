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

uint16 i2cReadBuffer [6];

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    __enable_irq(); /* Enable global interrupts. */
    EZI2C_Start(); /* Start EZI2C Component */
    /*
    * Set up communication and initialize data buffer to CapSense data structure
    * to use Tuner application
    */
    
    EZI2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam),
                            sizeof(CapSense_dsRam),
                            (uint8_t *)&(CapSense_dsRam));
    
    /*
    EZI2C_EzI2CSetBuffer1(sizeof(i2cReadBuffer),
                            sizeof(i2cReadBuffer),
                            (uint8_t *)&(i2cReadBuffer));
    */
    CapSense_Start(); /* Initialize Component */
    CapSense_ScanAllWidgets(); /* Scan all widgets */
    for(;;)
    {
        /* Do this only when a scan is done */
        if(CapSense_NOT_BUSY == CapSense_IsBusy())
        {
            CapSense_ProcessAllWidgets(); /* Process all widgets */
            CapSense_RunTuner(); /* To sync with Tuner application */
            //if (CapSense_IsAnyWidgetActive()) /* Scan result verification */
            //{
                /* add custom tasks to execute when touch detected */
            //}
            CapSense_ScanAllWidgets(); /* Start next scan */

           // for (unsigned int i=0; i<6; ++i)
           // {
           //     i2cReadBuffer[i] = CapSense_dsRam.snsList.button0[i].raw[0];
           // }

        }
    }
}

/* [] END OF FILE */