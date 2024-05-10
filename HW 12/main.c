// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h> // for memset
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWMPin 22 // the built in LED on the Pico
char message[100];

int to_angle(int angle)
{
    
    float m = ((12.3 - 2.5) / 180.0);
    float b = 2.5;
    int dutyCycle = 100/(m * angle + b);

    int16_t wrap = 62500;
    //pwm_set_gpio_level(PWMPin, wrap/dutyCycle); // set the duty cycle to 50%
    printf("The duty cycle percentage for %d degrees is: %d\n", angle, dutyCycle);

    return(dutyCycle);
}

int main()
{
    
    //printf("Start!\n");
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start 2!\n");
    
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_function(PWMPin, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num = pwm_gpio_to_slice_num(PWMPin); // Get PWM slice number
    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
    
    while(1){
        //Heartbeat
        //blink GP25
        gpio_put(LED_PIN, 1);
        sleep_ms(200);
        //printf("Start!\n");

        int dutyCycle = to_angle(0);
        pwm_set_gpio_level(PWMPin, wrap/dutyCycle);
        sleep_ms(2000);

        dutyCycle = to_angle(180);
        pwm_set_gpio_level(PWMPin, wrap/dutyCycle);
        sleep_ms(1500);
        
        gpio_put(LED_PIN, 0);
        sleep_ms(300);
    }
}
