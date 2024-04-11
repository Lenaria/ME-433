#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

/* Example code to talk to a bme280 humidity/temperature/pressure sensor.

   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefore SPI) cannot be used at 5v.

   You will need to use a level shifter on the SPI lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board and a generic bme280 board, other
   boards may vary.

   GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO on bme280 board
   GPIO 17 (pin 22) Chip select -> CSB/!CS on bme280 board
   GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK on bme280 board
   GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI on bme280 board
   3.3v (pin 36) -> VCC on bme280 board
   GND (pin 38)  -> GND on bme280 board

   Note: SPI devices can have a number of different naming schemes for pins. See
   the Wikipedia page at https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
   for variations.

   This code uses a bunch of register definitions, and some compensation code derived
   from the Bosch datasheet which can be found here.
   https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
*/

#define READ_BIT 0x80

#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    //asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    //asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    //asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    //asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
static void write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg & 0x7f;  // remove read bit as this is a write
    buf[1] = data;
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
    sleep_ms(10);
}

static void write_voltage(uint8_t reg, uint16_t data) 
{
    uint8_t buf[2];

    // Ensure 'data' is only 10 bits. If 'data' can be larger, mask the lower 10 bits.
    data &= 0x03FF;

    // - MSB from 'reg' (0 or 1), shift it to bit 7 position.
    // - Next three bits set to 1.
    // - Last four bits are the top four bits of 'data'.
    buf[0] = (reg << 7) | 0x70 | ((data >> 6) & 0x0F);

    // - First 6 bits of buf[1] are the least significant 6 bits of 'data'.
    buf[1] = (data << 2) & 0xFF;

    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
    
}


#endif

int main() {
    stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else

    printf("Starting SPI...\n");

    // This example will use SPI0 at 20kHz.
    spi_init(spi_default, 2000 * 1000);
    
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI); //SCK GP18
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);  //SD0 GP19 to SDI on Chip

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT); //Set Chip Select to Output
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); //Initialize Chip Select to 1

    float dt = 0.0; //time increment - assume we are checking for data every millisecond so dt = dt + 0.001
    float fsin, ftri;
    int counter = 0, sign = 1; //Triangle wave switches sign after 500 counts
    float m = 1023/(500); //Frequency of wave is 1Hz, so it completes 1 cycle in 1000ms, half cycle in 500ms
    float trial = 512; //send it a constant voltage
    uint16_t data;
    uint8_t channel;

    while (1) {
        
        // Verification Step
        data = (uint16_t)trial;
        channel = 0x01;
        // write_voltage(channel, data);
        
        // Sine Wave
        fsin = 512 + 511 * sin(dt*2*2*3.14);
        dt = dt + 0.001;
        data = (uint16_t)fsin;
        channel = 0x01;
        //write register function channel A
        write_voltage(channel, data);

        // Triangle Wave 
        if (counter < 500)
        {
            //sign = 1;
            ftri = m*counter;
        }
        else if (counter < 1000)
        {
            //sign = -1;
            ftri = 1023 - m*(counter-500);
        }
        else if (counter == 1000)
        {
            counter = 0;
        }
        counter++;
        //ftri = m*sign*counter;
        data = (uint16_t)ftri;
        channel = 0x00;
        
        //write register function channel B
        write_voltage(channel, data);
        
        sleep_ms(1); //Allow enough time for next time increment
    }
#endif
}
