# 概述
[vfd_driver](https://github.com/Pickaxe-Hit/vfd_driver)是一个基于乐鑫esp32-s3开发的针对日本则武生产的vfd屏幕（MN12832J）（下称vfd）的驱动程序，并搭配以专用的驱动板。  

本项目包含库文件以及示例函数，示例函数在main.c文件中，如不作修改则驱动程序将使vfd以1Hz闪烁，库文件则为mn12832j.c以及mn12832j.h。  

本项目作为一个完整的[esp-idf](https://github.com/espressif/esp-idf)项目，可以直接编译和烧录。