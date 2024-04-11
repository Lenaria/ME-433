#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

 /*
    GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on BMP280
    board
    GPIO PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7)) -> SCL on
    BMP280 board
 */

 // device has default bus address of 0x76
#define ADDR _u(0b0100000)

// hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

void chip_init() {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111; // What pins are inputs/outputs: 7 is o/p, 6-0 are i/p
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void set(char val) {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = val<<7; // val = 0 for LED OFF and 1 for ON, shift it 7 bits to access GP7
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

int read() {
    
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);  // false - finished with bus

    if (buf[0]&0b1 == 0b1)
    {
        return (1);
    }
    else{
        return (0);
    }
}

int main() {
    stdio_init_all();
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    //gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    //gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    chip_init(); //Set GP7 to o/p and GP0 to i/p

    sleep_ms(250); // sleep so that data polling and register update don't collide
    while (1) {
        //blink GP25
        gpio_put(LED_PIN, 1);
        sleep_ms(200);
        gpio_put(LED_PIN, 0);
        //sleep_ms(100);

        //stuff:
        
        //Read from GP0
        if (read()== 1)//if GP0 is high: set GP7 high
        {
            set(1);
        }
        
        else //else: set GP7 low
        {
            set(0);
        }

        // poll every 200ms
        sleep_ms(300);
    }

}
