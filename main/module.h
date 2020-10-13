// Configuration for TTGO ESP32 LoRa OLED v1 module
//#define ROUTE

#define SS1 18
#define SS2 23
#define SS3 22
#define SS4 14

#define BUTTON		GPIO_NUM_0
#define LED		GPIO_NUM_2

#define OLED_SDA	GPIO_NUM_4
#define OLED_SCL	GPIO_NUM_15
#define OLED_RST	GPIO_NUM_16

#define LORA_SCK	GPIO_NUM_5
#define LORA_MISO	GPIO_NUM_19
#define LORA_MOSI	GPIO_NUM_27
#define LORA_CS		18
#ifdef ROUTE
#define LORA_RST	GPIO_NUM_2
#else
#define LORA_RST	GPIO_NUM_14
#endif

#define LORA_IRQ	GPIO_NUM_26
