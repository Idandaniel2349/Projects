//
// Created by idan on 09/12/2020.
//

#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE

#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/uaccess.h>  /* for get_user and put_user */
#include <linux/string.h>   /* for memset. NOTE - not string.h!*/
#include "linux/fs.h"
#include "linux/errno.h"
#include "linux/list.h"
#include "linux/slab.h"

MODULE_LICENSE("GPL");
#include "message_slot.h"


struct list_node{
    struct list_head node;
    unsigned int channelId;
    char * data;
    int dataLength;
};

struct list_node * slots_array[Array_Size]={NULL};



//================== DEVICE FUNCTIONS ===========================
static int device_open( struct inode* inode,struct file*  file ){

    unsigned int * channelId;
    printk(KERN_INFO "Invoking device_open(%p)\n", file);

    //if file not yet set
    if((slots_array[iminor(inode)])==NULL){
        slots_array[iminor(inode)]=kmalloc(sizeof(struct list_node),GFP_KERNEL);
        if(slots_array[iminor(inode)]==NULL){//kmalloc failed
            return -ENOMEM;
        }
        INIT_LIST_HEAD(&(slots_array[iminor(inode)]->node));
    }

    channelId=kmalloc(sizeof(unsigned int),GFP_KERNEL);
    if(channelId==NULL){
        return -ENOMEM;
    }

    *channelId=0;

    file->private_data=channelId;

    return SUCCESS;
}

static int device_release( struct inode* inode,struct file*  file){
    unsigned int * channelId;

    printk(KERN_INFO "Invoking device_release(%p)\n", file);

    channelId=file->private_data;
    if(channelId!=NULL){
        kfree(channelId);
    }


    return SUCCESS;
}

// a process which has already opened
// the device file attempts to read from it
static ssize_t device_read( struct file* file,char __user* buffer,size_t length,loff_t* offset ){

    struct list_node * currentChannel,*temp;
    struct list_node * fileSlotHead;
    unsigned int channelId;
    char * data;
    int dataLength;
    int result;
    int i;

    printk(KERN_INFO "Invoking device_read(%p)\n", file);

    channelId = *((unsigned int *)file->private_data);


    if(channelId==0){//channel not set
        return -EINVAL;
    }

    if(access_ok(buffer,length)==0){
        return -EINVAL;
    }


    fileSlotHead = slots_array[iminor(file_inode(file))];
    channelId = *((unsigned int *)file->private_data);


    //find the channel in the slot
    list_for_each_entry_safe(currentChannel,temp,&(fileSlotHead->node),node){
        if(currentChannel->channelId==channelId){
            break;
        }
    }
    data=currentChannel->data;
    dataLength=currentChannel->dataLength;


    if(data==NULL || dataLength==-1){//no data set on channel
        return -EWOULDBLOCK;
    }

    if(dataLength>length){// buffer size too small
        return -ENOSPC;
    }


    //read from channel
    for(i=0;i<dataLength;i++){
        result = put_user(data[i],&buffer[i]);
        if(result<0){
            return -EFAULT;
        }
    }
    return dataLength;
}

// a processs which has already opened
// the device file attempts to write to it
static ssize_t device_write( struct file* file,const char __user* buffer,size_t length,loff_t* offset){

    struct list_node * currentChannel,*temp;
    struct list_node * fileSlotHead;
    unsigned int channelId;
    int result;
    int i;
    char * tempData;

    printk(KERN_INFO "Invoking device_write(%p)\n", file);

    channelId = *((unsigned int *)file->private_data);

    if(channelId==0){//channel not set
        return -EINVAL;
    }

    if(access_ok(buffer,length)==0){
        return -EINVAL;
    }

    if(length==0 || length>128){
        return -EMSGSIZE;
    }

    fileSlotHead = slots_array[iminor(file_inode(file))];


    //find the channel in the slot
    list_for_each_entry_safe(currentChannel,temp,&(fileSlotHead->node),node){
        if(currentChannel->channelId==channelId){
            break;
        }
    }

    //make copy of channel data
    tempData=kmalloc(length,GFP_KERNEL);
    if(tempData==NULL){
        return -ENOMEM;
    }

    //write to channel copy
    for(i=0;i<length;i++){
        result = get_user(tempData[i],&buffer[i]);
        if(result<0){
            kfree(tempData);
            return -EFAULT;
        }
    }

    //write to channel
    currentChannel->data=kmalloc(length,GFP_KERNEL);
    if(currentChannel->data==NULL){
        return -ENOMEM;
    }
    for(i=0;i<length;i++){
        currentChannel->data[i]=tempData[i];
    }
    currentChannel->dataLength=length;



    kfree(tempData);
    return length;
}

static long device_ioctl( struct   file* file,unsigned int ioctl_command_id,unsigned long  ioctl_param ){

    struct list_node * currentChannel,*temp;
    struct list_node * channel;
    struct list_node * listHead;
    unsigned int * channelId;

    printk(KERN_INFO "Invoking device_ioctl(%p)\n", file);

    if(ioctl_command_id != MSG_SLOT_CHANNEL){
        return -EINVAL;
    }
    if(ioctl_param==0){
        return -EINVAL;
    }

    //save the channelId
    channelId=file->private_data;
    *channelId=(unsigned int)ioctl_param;

    listHead = slots_array[iminor(file_inode(file))];

    //checks if the channel already exists on the slot
    list_for_each_entry_safe(currentChannel,temp,&(listHead->node),node){
        if(currentChannel->channelId==ioctl_param){
            return SUCCESS;
        }
    }

    //sets the channel
    channel = kmalloc(sizeof(struct list_node),GFP_KERNEL);
    if(channel==NULL){//kmalloc failed
        return -ENOMEM;
    }
    channel->channelId = (unsigned int)ioctl_param;
    channel->dataLength=-1;
    INIT_LIST_HEAD(&(channel->node));
    list_add_tail(&(channel->node),&(listHead->node));

    return SUCCESS;
}

//==================== DEVICE SETUP =============================

// This structure will hold the functions to be called
// when a process does something to the device we created
struct file_operations Fops =
        {
                .owner	  = THIS_MODULE,
                .read           = device_read,
                .write          = device_write,
                .open           = device_open,
                .unlocked_ioctl = device_ioctl,
                .release        = device_release,
        };

// Initialize the module - Register the character device
static int __init init(void){
    int rc=-1;
    printk(KERN_INFO "Invoking init\n");
    // Register driver capabilities. Obtain major num
    rc = register_chrdev( MAJOR_NUMBER, DEVICE_RANGE_NAME, &Fops );

    //registration failed
    if( rc < 0 ){
        printk(KERN_ERR "%s registraion failed for  %d\n", DEVICE_FILE_NAME, MAJOR_NUMBER);
        return rc;
    }

    return SUCCESS;
}

static void __exit cleanup(void){
    struct list_node * currentChannel,*temp;
    int i;

    printk(KERN_INFO "Invoking cleanup\n");

    //always succeed to unregister
    unregister_chrdev(MAJOR_NUMBER, DEVICE_RANGE_NAME);

    //free needed memory
    for(i=0; i<Array_Size;i++){
        if((slots_array[i])!=NULL){
            list_for_each_entry_safe(currentChannel,temp,&(slots_array[i]->node),node){
                if(currentChannel->data!=NULL){
                    kfree(currentChannel->data);
                }
                list_del(&(currentChannel->node));
                kfree(currentChannel);
            }
            list_del(&(slots_array[i]->node));
            kfree(slots_array[i]);
        }
    }
}

module_init(init);
module_exit(cleanup);




