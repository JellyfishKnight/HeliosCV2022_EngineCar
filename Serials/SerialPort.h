//
// Created by wjy on 22-5-21.
//
#ifndef HELIOSCV2022_ENGINETRUCK_SERIALPORT_H
#define HELIOSCV2022_ENGINETRUCK_SERIALPORT_H

#include <iostream>
#include <unistd.h>     // UNIX Standard Definitions
#include <fcntl.h>      // File Control Definitions
#include <cerrno>      // ERROR Number Definitions
#include <termios.h>
#include <cstring>
#include <cstdio>



#define PC2STM32 "/dev/ttyUSB0"//串口位置


/*--------------------------------暂定协议-------------------------------------*/

//暂定25字节,头1字节,数据18字节,尾1字节
#define    VISION_LENGTH        5
//起始字节,协议固定为0xA5
#define    VISION_SOF         (0xA5)
//end字节,协议固定为0xA5
#define    VISION_TOF         (0xA6)

/**---------------------------------------SEND DATA PROTOCOL--------------------------------------------**/
/**    ----------------------------------------------------------------------------------------------------
FIELD  |  A5  |  CmdID  |  yaw  |  pitch  | distance  |  shoot  |  find  |  timestamp |  A6  |
       ----------------------------------------------------------------------------------------------------
BYTE   |   1  |    1    |   4   |    4    |     4     |    1    |    1   |     8     |   1  |
       ----------------------------------------------------------------------------------------------------
**/
/**---------------------------------------SEND DATA PROTOCOL--------------------------------------------**/


/**---------------------------------------RECEIVE DATA PROTOCOL-----------------------------------------------------------------------------**/
/**    -----------------------------------------------------------------------------------------------------------------------------------------
FIELD  |  head  |  CmdID  |  yawAngle  |  pitchAngle  | yawSpeed  |  pitchSpeed  |  targetMode  |  targetColor | blank |  blank |  blank |   A6  |
       ----------------------------------------------------------------------------------------------------
BYTE   |   1    |    1    |     4      |      4      |     4     |      4       |       4       |      8     |   1   |    1   |   1    |   1   |
------------------------------------------------------------------------------------------------------------------------------------------------
**/
/**---------------------------------------RECEIVE DATA PROTOCOL------------------------------------------------------------------------------**/
/**DESCRIPTION:
 * head: 0xA5
 * CmdID: Command ID
 * yawAngle: current yaw angle of pan-tilt
 * pitchAngle: current pitch angle of pan-tilt
 * yawSpeed: current spin speed of yaw direction
 * pitchSpeed: current pitch angle of pan-tilt
 * targetMode: the mode of vision task(AUTOSHOOT/ENERGY)
 * targetColor: blue or red enemy
 */

using namespace std;

/**
 * @brief receive data structure
 */
struct ReceiveData {
    uint8_t head = 0;
    uint8_t CmdID = 0;

    float yawAngle = 0;
    float pitchAngle = 0;
    float yawSpeed = 0;
    float bulletSpeed = 0;
    float pitchSpeed = 0;
    uint8_t targetMode = 0;

    uint8_t targetColor = 0;// 0 red target, 1 blue target
    uint8_t direction = 0;
    uint8_t blankC = 0;
    uint8_t blankD = 0;
    uint8_t blankE = 0;

    uint8_t end;
};

/**
 * @brief SerialPort
 * @param filename 串口名字
 * @param buadrate 串口波特率,用于stm32通信是0-B115200,用于stm32通信是1-B921600
 */
class Serial {
private:
    int fd;
    int nSpeed;
    char nEvent;
    int nBits;
    int nStop;
    uint8_t buff[VISION_LENGTH];
    uint8_t buffRead[100];
    uint8_t curBuf;
    int readCount;
    int maxReadTime;

    static int set_opt(int fd, int nSpeed, char nEvent, int nBits, int nStop);

public:
    explicit Serial(int nSpeed = 115200, char nEvent = 'N', int nBits = 8, int nStop = 1);

    ~Serial();

    void pack(uint8_t set, uint8_t direction, uint8_t toggle_flag);

    bool InitPort(int nSpeed = 115200, char nEvent = 'N', int nBits = 8, int nStop = 1);

    bool WriteData();

    bool ReadData(struct ReceiveData &buffer);
};


#endif //HELIOSCV2022_ENGINETRUCK_SERIALPORT_H
