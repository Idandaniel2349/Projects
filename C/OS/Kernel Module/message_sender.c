//
// Created by idan on 09/12/2020.
//

#include "message_slot.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "fcntl.h"      /*open*/
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */

int main(int argc, char *argv[]) {
    int file_desc;
    int ret_val;
    char * message;
    int length;
    long int channel;
    char *endptr;

    if(argc!=4){//wrong number of arguments passed
        fprintf(stderr,"%s\n",strerror(EINVAL));
        exit(1);
    }

    //open file
    file_desc=open(argv[1],O_RDWR);
    if(file_desc<0){
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }

    //set channel
    channel = strtol(argv[2],&endptr,10);
    if((*endptr)!='\0'){//strtol failure
        fprintf(stderr,"%s\n",strerror(errno));
        close(file_desc);
        exit(1);
    }
    ret_val=ioctl(file_desc,MSG_SLOT_CHANNEL,channel);
    if(ret_val<0){
        fprintf(stderr,"%s\n",strerror(errno));
        close(file_desc);
        exit(1);
    }

    message=argv[3];
    length = strlen(message);
    message[length]=(char)' ';//remove the null terminator

    //write to channel
    ret_val=write(file_desc,message,length);
    if(ret_val<0){
        fprintf(stderr,"%s\n",strerror(errno));
        close(file_desc);
        exit(1);
    }

    close(file_desc);
    exit(0);

}

