In PowerSeqeuenc subroutine, add below code to make BMC perform data integrity checking over SPI ROM.
      case POWERUP:
        if(!LED_in_use) GPIO_write(BMC_STATUS, LED_ON);
        GPIO_disableAllInt();
        
        Task_sleep(500);        //new added. This delay is necessary, otherwise, changing SPI may cause Top swap function enabled.
        PerformSPIRead();   	//new added. 
        Task_sleep(500);		//new added. This delay is necessary, otherwise, changing SPI may cause Top swap function enabled.
