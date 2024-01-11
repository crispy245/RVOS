#include "os.h"

#define DISK_PORT_NAME "disk0"

static deviceCB_t *disk;

deviceCB_t *disk_dev_init(){
    
    disk = device_create(Device_Class_MTD,DISK_PORT_NAME);
    disk->init = NULL;
    disk->open = NULL;
    disk->close = NULL;
    disk->read = NULL;
    disk->write = NULL;
    disk->control = NULL;
    
    err_t regResult = device_register(disk,DISK_PORT_NAME,FLAG_RDWR|FLAG_DEACTIVATE);
    
    if (regResult != E_DEV_OK) {
        return NULL;
    }

    return disk;
}

static err_t disk0_init(){
    
}