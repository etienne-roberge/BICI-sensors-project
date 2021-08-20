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

int main(void)
{

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

   CyGlobalIntEnable;
    __enable_irq(); /* Enable global interrupts. */
    
    I2C_Start();
    
    I2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam),
                        sizeof(CapSense_dsRam),
                        (uint8_t *)&CapSense_dsRam);
    
    CapSense_Start();
    CapSense_ScanAllWidgets();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        if(!CapSense_IsBusy())
        {
            CapSense_ProcessAllWidgets();
           
            CapSense_ScanAllWidgets(); 
            CapSense_RunTuner();
        }
        
    }
}

/* [] END OF FILE */
