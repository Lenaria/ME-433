// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h> // for memset
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "font.h"

int main(){
    
    //printf("Start!\n");
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start 2!\n");
    
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // ADC initializations
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    //gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    //gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    ssd1306_setup();

    char message[100];    
    
    while(1){
        //Heartbeat
        //blink GP25
        gpio_put(LED_PIN, 1);
        //ssd1306_drawPixel(0, 0, 1);
        //ssd1306_update();
        sleep_ms(200);
        printf("Start!\n");
        gpio_put(LED_PIN, 0);
        //ssd1306_drawPixel(0, 0, 0);
        //ssd1306_update();

        
        int a = 0;
        /*
        printf("Enter the message you would like to print: ");
        scanf("%99s", message); // Read up to 99 characters + null terminator
        printf("\nScreen will print %s\n", message); // Use %d for integers
        //sprintf(message, "Arctic Monkeys %d", a);
        drawString(0,0,message);
        ssd1306_update();
        */

        uint16_t result = adc_read();

        unsigned int start = to_us_since_boot(get_absolute_time());

        sprintf(message, "ADC value: %d",result);
        drawString(0,0,message);
        ssd1306_update();

        unsigned int stop = to_us_since_boot(get_absolute_time());

        unsigned int t = stop - start;
        sprintf(message, "FPS: %f", 1000000.0 / t);
        drawString(1,24,message);
        ssd1306_update();

        sleep_ms(300);
    }
}
