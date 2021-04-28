//
// Created by idan on 09/12/2020.
//

#ifndef OS3_MESSAGE_SLOT_H
#define OS3_MESSAGE_SLOT_H

#include "linux/ioctl.h"

#define MAJOR_NUMBER 240
#define Array_Size 256
#define MSG_SLOT_CHANNEL _IOW(MAJOR_NUMBER, 0, unsigned long)

// Set the message of the device driver
#define IOCTL_SET_ENC _IOW(MAJOR_NUM, 0, unsigned long)


#define DEVICE_RANGE_NAME "char_dev"
#define DEVICE_FILE_NAME "simple_char_dev"
#define SUCCESS 0


#endif //OS3_MESSAGE_SLOT_H
