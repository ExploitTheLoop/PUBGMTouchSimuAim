//
// Created by PCAditiaID29 on 7/1/2021.
//

#ifndef PUBGM_AIMBOT_TOUCH_TEST_TOUCHHELPER_H
#define PUBGM_AIMBOT_TOUCH_TEST_TOUCHHELPER_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <linux/uinput.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <iconv.h>

#define sizeoq 11
#define GRAB 1
static const char *uinput_device_path = "/dev/uinput";

class TouchHelper {
public:
    static void Init(int &fb, int &fd, float &px, float &py, float &SlideX, float &SlideY);
    static int GetEventID();
    static int GetEventCount();
    static int open_uinput_device(int x, int y);
};

int TouchHelper::GetEventID() {
    int EventCount = GetEventCount();
    int *fdArray = (int *)malloc(EventCount * 4 + 4);
    for (int i = 0; i < EventCount; i++)
    {
        char temp[128];
        sprintf(temp, "/dev/input/event%d", i);
        fdArray[i] = open(temp, O_RDWR | O_NONBLOCK);
    }
    LOGI("Due to some of your reasons, the device needs to be calibrated, please click on the screen to complete the calibration");
    int k = 0;
    input_event ev{};
    while (1)
    {
        for (int i = 0; i < EventCount; i++)
        {
            memset(&ev, 0, sizeof(ev));
            read(fdArray[i], &ev, sizeof(ev));
            if (ev.type == EV_ABS)
            {
                free(fdArray);
                return i;
            }
        }
        usleep(100);
    }
}

int TouchHelper::GetEventCount() {
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != nullptr){
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    LOGI("Event Count: %d", count);
    return count;
}

void TouchHelper::Init(int &fb, int &fd, float &px, float &py, float &SlideX, float &SlideY) {
    char l[256];
    sprintf(l, "/dev/input/event%d", GetEventID());
    fb = open(l, O_RDWR);
    if (fb < 0)
    {
        LOGE("%s NULL", l);
        fb = NULL;
    } else {
        struct input_absinfo absX;
        struct input_absinfo absY;
        ioctl(fb, EVIOCGABS(ABS_MT_POSITION_X), &absX);
        ioctl(fb, EVIOCGABS(ABS_MT_POSITION_Y), &absY);
        SlideX = py * (830.0f / 1080.0f);
        SlideY = px * (1350.0f / 2248.0f);
        fd = open_uinput_device(absX.maximum, absY.maximum);
        assert(fd >= 0);
        if (ioctl(fb, EVIOCGRAB, GRAB))
            LOGE("Can't Grabbed!");
        else
            LOGI("Grabbed!");
        usleep(1000);
    }
}

int TouchHelper::open_uinput_device(int x, int y) {
    struct uinput_user_dev ui_dev;
    int uinp_fd = open(uinput_device_path, O_WRONLY | O_NONBLOCK);
    if (uinp_fd <= 0)
    {
        LOGE("Could not open %s, %s\n", uinput_device_path, strerror(errno));
        return -1;
    }

    memset(&ui_dev, 0, sizeof(ui_dev));
    strncpy(ui_dev.name, "VirtualTouch",UINPUT_MAX_NAME_SIZE);
    ui_dev.id.bustype = BUS_USB;
    ui_dev.id.vendor = 0x1341;
    ui_dev.id.product = 0x0001;
    ui_dev.id.version = 5;

    ui_dev.absmin[ABS_MT_POSITION_X] = 0;
    ui_dev.absmax[ABS_MT_POSITION_X] = x;
    ui_dev.absmin[ABS_MT_POSITION_Y] = 0;
    ui_dev.absmax[ABS_MT_POSITION_Y] = y;
    ui_dev.absmax[ABS_MT_TRACKING_ID] = 14;
    //enable direct
    ioctl(uinp_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

    //enable touch event
    ioctl(uinp_fd, UI_SET_EVBIT, EV_ABS);
    //ioctl(uinp_fd, UI_SET_ABSBIT, ABS_X);
    //ioctl(uinp_fd, UI_SET_ABSBIT, ABS_Y);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl(uinp_fd, UI_SET_ABSBIT,ABS_MT_TRACKING_ID);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_SYN);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_BACK);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_APPSELECT);
    write(uinp_fd, &ui_dev, sizeof(ui_dev));
    if (ioctl(uinp_fd, UI_DEV_CREATE))
    {
        LOGE("Unable to create UINPUT device.\n");
        return -1;
    }
    return uinp_fd;
}

#endif //PUBGM_AIMBOT_TOUCH_TEST_TOUCHHELPER_H
