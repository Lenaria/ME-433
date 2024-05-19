// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h> // for memset
#include <math.h>
#include "blink.h"

char message[100];
int16_t acc[3], gyro[3], temp[1];
uint8_t acc_planar[2];

void chip_init() {

    uint8_t buf[2];
    // send register number followed by its corresponding value

    //Turn Chip On
    buf[0] = PWR_MGMT_1;
    buf[1] = 0b00000000;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    //write to the ACCEL_CONFIG register
    buf[0] = ACCEL_CONFIG;
    buf[1] = 0b00000000; //sensitivity to plus minus 2g
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    //write to the GYRO_CONFIG register
    buf[0] = GYRO_CONFIG;
    buf[1] = 0b00000011; //sensitivity to plus minus 2000 dps
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    
}

void whoami(){
    
    uint8_t buf[1];

    uint8_t reg = WHO_AM_I;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);

    printf("WHO_AM_I = %u\n", buf[0]);

}

void read(){

    uint8_t buffer[14];
    uint8_t reg = ACCEL_XOUT_H;
    int j = 0;

    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buffer, 14, false);  // false - finished with bus

    for (int i = 0; i < 6; i += 2) {
        acc[j] = (buffer[i] << 8 | buffer[i + 1]);
        j++;
    }

    j = 0;
    for (int i = 6; i < 8; i += 2) {
        temp[j] = (buffer[i] << 8 | buffer[i + 1]);
        temp[j] = temp[j]/340.0 + 36.53;
        j++;
    }

    j = 0;
    for (int i = 8; i < 14; i += 2) {
        gyro[j] = (buffer[i] << 8 | buffer[i + 1]);
        j++;
    }

    for (int i = 0; i < 3; i++) {
        gyro[i] = gyro[i]*0.007630;
        acc[i] = acc[i]*0.000061;
    } 

    printf("Acceleration Values: X = %d, Y = %d, Z = %d\n", acc[0], acc[1], acc[2]);
    printf("Temperature Values: = %d\n", temp[0]);
    printf("Gyroscope Values: X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);

}

void read_mouse(){

    uint8_t buffer[14];
    uint8_t reg = ACCEL_XOUT_H;
    int j = 0;

    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);  // false - finished with bus

    for (int i = 0; i < 6; i += 2) {
        acc[j] = (buffer[i] << 8 | buffer[i + 1]);
        acc[j] = acc[j]*0.000061;
        j++;
    }

   for (int i = 0; i < 2; i++) {

        if (acc[i] == 0) {
            acc_planar[i] = 0; // Set to 0 if acc is 0
        } else if (acc[i] < 0.1 && acc[i] > -0.1) {
            acc_planar[i] = (acc[i] > 0) ? 20 : -20; // +1 or -1 depending on sign of acc
        } else if (acc[i] < -0.3 && acc[i] > -0.3) {
            acc_planar[i] = (acc[i] > 0) ? 30 : -30; // +2 or -2 depending on sign of acc
        } else {
            acc_planar[i] = (acc[i] > 0) ? 50 : -50; // +3 or -3 depending on sign of acc
        }
   }
}

uint8_t read_mouse_x(){
    return acc_planar[0];
}

uint8_t read_mouse_y(){
    return acc_planar[1];
}

/*
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

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    //gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    //gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    chip_init();
    whoami();
    //read();
    
    while(1){
        //Heartbeat
        //blink GP25
        gpio_put(LED_PIN, 1);
        sleep_ms(200);
        //printf("Start!\n");

        read();

        gpio_put(LED_PIN, 0);
        sleep_ms(300);
    }
}
*/