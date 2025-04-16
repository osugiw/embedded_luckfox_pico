#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_DEVICE_PATH "/dev/i2c-3"

int main(){
    uint8_t data[2] = {0x01, 0x02};
    const char *i2c_device = I2C_DEVICE_PATH;
    int i2c_file;

    if ((i2c_file = open(i2c_device, O_RDWR)) < 0) {
        perror("Failed to open I2C device");
        return -1;
    }

    ioctl(i2c_file, I2C_TENBIT, 0);
    ioctl(i2c_file, I2C_RETRIES, 5);


    printf("i2cdetect addr : ");
    for (int x=0; x<0x7F; x++){
        if (ioctl(i2c_file, I2C_SLAVE, x) < 0){
            perror("Failed to set I2C slave address");
            close(i2c_file);
            return -1;
        }

        if (write(i2c_file, data, 2) == 2){
            printf("0x%x, ", x);
        }
    }

    close(i2c_file);
    printf("\r\n");

    return 0;
}