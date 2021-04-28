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

#define messageSize 128

int main(int argc, char *argv[]) {
    int file_desc;
    int ret_val;
    int result;
    long int channel;
    char *endptr;

    if(argc!=3){//wrong number of arguments passed
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

    //read from channel

    char * buffer = malloc(messageSize);//the channel messages are at most 128 bytes
    if(buffer==NULL){
        fprintf(stderr,"%s\n",strerror(errno));
        close(file_desc);
        exit(1);
    }
    ret_val=read(file_desc,buffer,messageSize);
    if(ret_val<0){
        fprintf(stderr,"%s\n",strerror(errno));
        close(file_desc);
        free(buffer);
        exit(1);
    }

    close(file_desc);

    //print message
    result=write(1,buffer,ret_val);
    if(result<0){//failed to write
        fprintf(stderr,"%s\n",strerror(errno));
        close(file_desc);
        free(buffer);
        exit(1);
    }


    free(buffer);
    exit(0);
}