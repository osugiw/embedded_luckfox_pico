#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main(){
    int serial_port_num;
    char serial_port[15];

    printf("Select a serial port (3/4): ");
    scanf("%d", &serial_port_num);

    sprintf(serial_port, "/dev/ttyS%d", serial_port_num);
    int serial_fd;

    /*  Opening selected serial port number */
    serial_fd = open(serial_port, O_RDWR | O_NOCTTY);
    if (serial_fd == -1){
        perror("Failed to open serial port");
        return 1;
    }

    /* Get the current attribute    */
    struct termios tty;
    memset(&tty, 0, sizeof(tty));   
    if (tcgetattr(serial_fd, &tty) != 0){
        perror("Error from tcgetattr");
        return 1;
    }

    /*  Setting input and output baudrate   */
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB; // Disable parity
    tty.c_cflag &= ~CSTOPB; // Use one stop bit
    tty.c_cflag &= ~CSIZE;  // Clear data bit
    tty.c_cflag |= CS8;     // 8 bit data

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        perror("Error from tcsetattr");
        return 1;
    }

    char tx_buffer[] = "hello world!\n";
    ssize_t bytes_written = write(serial_fd, tx_buffer, sizeof(tx_buffer));
    if (bytes_written < 0) {
        perror("Error writing to serial port");
        close(serial_fd);
        return 1;
    }
    printf("\rtx_buffer: \n %s ", tx_buffer);

    char rx_buffer[256];
    int bytes_read = read(serial_fd, rx_buffer, sizeof(rx_buffer));
    if (bytes_read > 0) {
        rx_buffer[bytes_read] = '\0';
        printf("\rrx_buffer: \n %s", rx_buffer);
    }
    else {
        printf("No data received.\n");
    }
    close(serial_fd);

    return 0;
}