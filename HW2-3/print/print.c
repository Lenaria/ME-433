/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");

    // ADC initializations
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    // I/O initilizations
    gpio_init(14);
    gpio_set_dir(14, GPIO_OUT); // Output to LED
    gpio_init(15);
    gpio_set_dir(15, GPIO_IN); // Input from Button
 
    while (1) {
        // char message[100];
        // scanf("%s", message);
        // printf("message: %s\r\n",message);
        // sleep_ms(50);
        gpio_put(14, 0); // Turn LED ON
        int status = gpio_get(15);
        while(status == 1) // Wait till Button is pressed
        {
            status = gpio_get(15);
            //printf("Looping!\n");
        }
        gpio_put(14, 1); // Turn LED OFF

        printf("Enter the number of times you want to read the ADC: ");
        char message[100];
        scanf("%99s", message); // Read up to 99 characters + null terminator
        int num = atoi(message); // Convert the string to an integer

        printf("\nADC will be read %d times\n", num); // Use %d for integers

        for(int i = 0; i < num; i++) 
        {
            printf("Reading ADC #%d\n", i+1);
            uint16_t result = adc_read();
            printf("ADC value: %d\r\n",result);
        }

        // printf("Read!\n"); // Verification Step
        sleep_ms(100); //DELETE
    }
}
