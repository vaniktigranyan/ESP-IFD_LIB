#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "MFRC522.h"
#include "esp_log.h"

static const char* TAG = "RC522";
char username[16] = "vanik";
char password[16] = "tigranyan";
uint8_t card_rx_buffer[20];
uint8_t card_rx_len = 20;
MIFARE_Key key = 
{
 .keyByte = {0xff,0xff,0xff,0xff,0xff,0xff}
};
uint8_t req_buffer[16];
uint8_t req_len = 16;
uint8_t status = 0;

void app_main(void)
{
     esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num=19,
        .mosi_io_num=23,
        .sclk_io_num=18,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1
    };
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=5000000,               //Clock out at 5 MHz
        .mode=0,                                //SPI mode 0
        .spics_io_num=5,               //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        //.pre_cb=ili_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
    };
    //Initialize the SPI bus
    ret=spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret==ESP_OK);
    //Attach the RFID to the SPI bus
    ret=spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);
   
    PCD_Init(spi);
    while(1)
    {
        if(PICC_IsNewCardPresent(spi))                   //Checking for new card
    	{
            //GetStatusCodeName();
            printf("*********************************card detected!**************************\n");

            /*----------------read sectors------------------------------------------------ */
    		////GetStatusCodeName(PICC_Select(spi,&uid,0));
            //PICC_Select(spi,&uid,0);
    		//PICC_DumpToSerial(spi,&uid);                  // SECTORS
            
            /*----------------write text-------------------------------------------------- */
            //PICC_Select(spi,&uid,0);
            //PCD_Authenticate(spi,PICC_CMD_MF_AUTH_KEY_A,5,&key, &(uid));
            //MIFARE_Write(spi,5,(uint8_t*)username,16);// SECTOR 5
            //MIFARE_Write(spi,6,(uint8_t*)password,16);
            //PCD_StopCrypto1(spi);
            
            /*----------------read content in 5 sector----------------------------------- */
            /*PICC_Select(spi,&uid,0);
            PCD_Authenticate(spi,PICC_CMD_MF_AUTH_KEY_A,5,&key, &(uid));
            MIFARE_Read(spi,5,card_rx_buffer,&card_rx_len);
            ESP_LOGI(TAG,"bufer ==>  %s",card_rx_buffer);
            PICC_DumpToSerial(spi,&uid);
            PCD_StopCrypto1(spi);*/
             
            PICC_Select(spi,&uid,0);
            PCD_Authenticate(spi,PICC_CMD_MF_AUTH_KEY_A,5,&key, &(uid));
            MIFARE_Read(spi,5,card_rx_buffer,&card_rx_len);
            ESP_LOGI(TAG,"block 5 content ==>  %s",card_rx_buffer);
            //PICC_DumpToSerial(spi,&uid);
            PCD_StopCrypto1(spi);
            

    		vTaskDelay(100 / portTICK_PERIOD_MS);

    	}
        //ESP_LOGI(TAG,"bufer ==>  %s",card_rx_buffer);
            //PCD_StopCrypto1(spi);
        //PICC_DumpToSerial(spi,&uid);
    	vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
