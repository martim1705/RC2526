#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include <stddef.h>
#include <stdint.h>

// variáveis globais definidas apenas em serial.c
extern int fd;
extern volatile int STOP;
extern struct termios oldtio;

// funções comuns
int openSerialPort(const char *port, int speed);
int closeSerialPort(void);
int readByteSerialPort(uint8_t *b);
int writeBytesSerialPort(const unsigned char *b, int n);

#endif