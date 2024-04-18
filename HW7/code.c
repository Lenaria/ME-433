// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h> // for memset
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "font.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

volatile int i = 0, row = 0;

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        
        if(ch == '\n' || ch == '\r')
        {
            i = 0;
            row = 0;
            ssd1306_clear();
        }
        
        else
        {
            drawChar((i*5), (row*8), ch);
            i++;
            if (i == 25 || i == 50 || i>=75)
            {
                row++;
                i=0;
            }
            ssd1306_update();
        }

        // Can we send it back?
        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            //ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}

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

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    //gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    //gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    ssd1306_setup();

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "\nHello, uart interrupts\n");

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
        sleep_ms(300);
    }
}
