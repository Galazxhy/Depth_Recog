#include "serial/serial.h"
#include "recog/Position.h"

serial::Serial sp;

//缓冲区
unsigned char buffer[16];

//浮点数和十六进制字符串转换
typedef union{
    float position;
    unsigned char data_char[4];
}serial_data;


unsigned char* Protocol(unsigned char* buffer,const recog::Position::ConstPtr &position_msg);
