// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h> // for memset
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "font.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"


#define PWMPin 22
#define IOPin 13

#define RPWMPin 10
#define RIOPin 12

static int chars_rxed = 0;
char message[100];
int motorSpeed;

int setDir(int motorSpeed)
{
    int dir;
    if (motorSpeed > 0) 
    {
        dir = 0;  // Motor speed positive, set direction to 0
    } 
    else if(motorSpeed < 0) 
    {
        dir = 1;  // Motor speed negative, set direction to 1
    }
    return(dir);
}

int setLeftSpeed(int motorSpeed, int dir)
{
    int dutyCycle, Speed;
    uint16_t wrap = 62500;

    if (motorSpeed > 40) 
    {
        Speed = 100;
    } 
    else if(motorSpeed < -40) 
    {
        Speed = -100;
    }
    else if(motorSpeed < 40 && motorSpeed >= 0) 
    {
        Speed = 2.5*motorSpeed;
    }
    else if(motorSpeed > -40 && motorSpeed < 0) 
    {
        Speed = -2.5*motorSpeed;
    }

    if (dir == 0) 
    {
        dutyCycle = (Speed*62500.0)/100;  // Motor speed positive, set direction to 0
    } 
    else if (dir == 1) 
    {
        dutyCycle = ((100 + Speed)*62500.0)/100;  // Motor speed negative, set direction to 1
    }

    //pwm_set_gpio_level(PWMPin, wrap/dutyCycle); // set the duty cycle to 50%
    printf("The Left Wheel Speed is: %d\n", Speed);

    return(dutyCycle);
}

int setRightSpeed(int motorSpeed, int dir)
{
    int dutyCycle, Speed;
    uint16_t wrap = 62500;

    if (motorSpeed < 60 && motorSpeed >= 0) 
    {
        Speed = 100;
    } 
    else if(motorSpeed > -60 && motorSpeed < 0) 
    {
        Speed = -100;
    }
    else if(motorSpeed > 60 ) 
    {
        motorSpeed = 100 - motorSpeed;
        Speed = 2.5*motorSpeed;
    }
    else if(motorSpeed < -60 ) 
    {
        motorSpeed = -100 - motorSpeed;
        Speed = -2.5*motorSpeed;
    }

    if (dir == 0) 
    {
        dutyCycle = (Speed*62500.0)/100;  // Motor speed positive, set direction to 0
    } 
    else if (dir == 1) 
    {
        dutyCycle = ((100 + Speed)*62500.0)/100;  // Motor speed negative, set direction to 1
    }

    //pwm_set_gpio_level(PWMPin, wrap/dutyCycle); // set the duty cycle to 50%
    printf("The Right Wheel Speed is: %d\n", Speed);

    return(dutyCycle);
}


int main()
{
    int dutyCycle_left, dutyCycle_right, dir;

    //printf("Start!\n");
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start 2!\n");
    
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // I/O initilizations
    gpio_init(IOPin);
    gpio_set_dir(IOPin, GPIO_OUT); // Output to IN1!!!!
    gpio_init(RIOPin);
    gpio_set_dir(RIOPin, GPIO_OUT); // Output to IN4!!!!


    gpio_set_function(PWMPin, GPIO_FUNC_PWM); // Set GP22 PWM to IN2!!!!
    uint slice_num = pwm_gpio_to_slice_num(PWMPin); // Get PWM slice number
    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM

    gpio_set_function(RPWMPin, GPIO_FUNC_PWM); // Set GP10 PWM to IN3!!!!
    uint rslice_num = pwm_gpio_to_slice_num(RPWMPin); // Get PWM slice number
    float rdiv = 40; // must be between 1-255
    pwm_set_clkdiv(rslice_num, rdiv); // divider
    uint16_t rwrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(rslice_num, rwrap);
    pwm_set_enabled(rslice_num, true); // turn on the PWM
    
    while(1){
        //Heartbeat
        //blink GP25
        gpio_put(LED_PIN, 1);
        sleep_ms(200);

        printf("Enter the next value [0 to 100]: ");
        scanf("%d", &motorSpeed);
        printf("You entered: %d\n", motorSpeed);

        dir = setDir(motorSpeed);
        //dutyCycle = setSpeed(motorSpeed);
        dutyCycle_left = setLeftSpeed(motorSpeed, dir);
        dutyCycle_right = setRightSpeed(motorSpeed, dir);

        gpio_put(IOPin, dir);
        pwm_set_gpio_level(PWMPin, dutyCycle_left);

        gpio_put(RIOPin, dir);
        pwm_set_gpio_level(RPWMPin, dutyCycle_right);

        sleep_ms(2000);
        
        gpio_put(LED_PIN, 0);
        sleep_ms(300);
    }
}
