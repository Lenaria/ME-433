#ifndef BLINK_H
#define BLINK_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Define the I2C device address
#define ADDR _u(0b1101000)

// Define config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C

// Define sensor data registers
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75

// Declare functions used by blink.c
void chip_init(void);
void whoami(void);
void read(void);
void read_mouse(void);
uint8_t read_mouse_x(void);
uint8_t read_mouse_y(void);

#endif // BLINK_H
