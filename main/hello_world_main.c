/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "rfm95.h"
#include "spi.h"
#include "driver/gpio.h"
#include "module.h"

#include <esp_log.h>

//#define MILLISECONDS	1000
//#define SECONDS		1000000

#define MHz		1000000

#define BAND1    868.800E6//433E6  //you can set band here directly,e.g. 868E6,915E6
#define BAND2    868950E3//433E6  //you can set band here directly,e.g. 868E6,915E6
#define BAND3    869075E3//433E6  //you can set band here directly,e.g. 868E6,915E6
#define BAND4    869200E3//433E6  //you can set band here directly,e.g. 868E6,915E6




#define PUMP_FREQUENCY	 916600000
#define MAX_PACKET_LEN	22
	
#define TAG		"TEST_GFSK"
#define data "biggest test for gsfk"
#define ARRAY_SIZE 60
uint8_t data_big[ARRAY_SIZE];
//#define RECV_PACKET
#define MILLISECONDS	1000

static uint8_t fr[3];
static inline bool valid_frequency(uint32_t f) {
	if (863*MHz <= f && f <= 870*MHz) {
		return true;
	}
	if (910*MHz <= f && f <= 920*MHz) {
		return true;
	}
	return false;
}

static void check_frequency(void) {
	uint32_t f = ((uint32_t)fr[0] << 16) + ((uint32_t)fr[1] << 8) + ((uint32_t)fr[2]);
	uint32_t freq = (uint32_t)(((uint64_t)f * 24*MHz) >> 16);
	if (valid_frequency(freq)) {
		ESP_LOGI(TAG, "setting frequency to %d Hz", freq);
		set_frequency(freq);
	}
	else {
		ESP_LOGD(TAG, "invalid frequency (%d Hz)", freq);
	}
}

void app_main()
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  
//    printf("Restarting now.\n");
//    fflush(stdout);
//    esp_restart();
	
	
//	gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//	gpio_set_level(LORA_CS, 1);
//	
//	
	gpio_set_direction(SS1, GPIO_MODE_OUTPUT);
	gpio_set_level(SS1, 1);

	gpio_set_direction(SS2, GPIO_MODE_OUTPUT);
	gpio_set_level(SS2, 1);
	
	gpio_set_direction(SS3, GPIO_MODE_OUTPUT);
	gpio_set_level(SS3, 1);
//	
	gpio_set_direction(SS4, GPIO_MODE_OUTPUT);
	gpio_set_level(SS4, 1);
//	
//	
//	
	gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
	gpio_set_level(LORA_RST, 0);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	gpio_set_level(LORA_RST, 1);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	//gpio_set_direction(LORA_RST, GPIO_MODE_INPUT);
	
	rfm95_init();
	uint8_t v = read_version();
	printf("radio version %d.%d\n", version_major(v), version_minor(v));

	set_frequency(BAND1);
//	check_frequency();
//	set_frequency(PUMP_FREQUENCY);
	ESP_LOGD(TAG, "frequency set to %d Hz", read_frequency());

	dumpRegister();
	
	
#ifdef RECV_PACKET
	while (true)
	{
		ESP_LOGD(TAG, "start receive");
		char packet[MAX_PACKET_LEN];
		int n = receive(&packet, MAX_PACKET_LEN, 60*MILLISECONDS);
		if (n > 0)
		{
			packet[n] = 0;
			ESP_LOGD(TAG, "receive char = %s rssi=%d ", packet, read_rssi());
		}
		ESP_LOGD(TAG, "receive n = %d", n);
		usleep(1*MILLISECONDS);
	}
#else
	while (true)
		{
			transmit(&data_big, ARRAY_SIZE);
		usleep(1*MILLISECONDS);
		//vTaskDelay(4000 / portTICK_PERIOD_MS);
	}

#endif
		for (int i = 10; i >= 0; i--) {
			printf("Restarting in %d seconds...\n", i);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
		printf("Restarting now.\n");
		fflush(stdout);
		esp_restart();
}
