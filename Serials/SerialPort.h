//
// Created by wjy on 22-5-21.
//
#ifndef HELIOSCV2022_ENGINETRUCK_SERIALPORT_H
#define HELIOSCV2022_ENGINETRUCK_SERIALPORT_H

#include <iostream>

/*--------------------------------暂定协议-------------------------------------*/

//暂定6字节，头1字节，数据4字节，尾1字节
#define     VISION_LENGTH      25
//起始字节，协议固定为0xA5
#define     VISION_SOF         (0xA5)
//结束字节，协议固定为0xA5
#define     VISION_TOF         (0xA5)

/**-------------------SEND DATA PROTOCOL-------------------------------------**/
/**    ------------------------------------------------------------------------
FILED  |   A5  |  Direction  |  A6  |
       ------------------------------------------------------------------------
BYTE   |   1   |      4      |   1  |
       ------------------------------------------------------------------------
 **/
/**-------------------SEND DATA PROTOCOL-------------------------------------**/


/**-------------------RECIEVE DATA PROTOCOL----------------------------------**/

/**-------------------RECIEVE DATA PROTOCOL----------------------------------**/
/** DESCRIPSTION:
 *  head: 0xA5
 *  direction: which direction should turn to
 *  end: 0xA5
 * **/

using namespace std;

/**
 * @brief data structure
 */
struct Data{
    uint8_t head = 0;

    int Direction = 0;

    uint8_t end;
};


/**
 * @brief SerialPort
 * @param filename 串口名字
 * @param buadrate 串口波特率,用于stm32通信是0-B115200,用于stm32通信是1-B921600
 */
class Serial{
private:
    int nSpeed;            //串口波特率
    int nBits;             //发送数据的长度
    int nStop;             //结束字节的长度
    uint8_t buff[VISION_LENGTH];
    uint8_t buffRead[VISION_LENGTH];
    uint8_t curbuff;
};

#endif //HELIOSCV2022_ENGINETRUCK_SERIALPORT_H
