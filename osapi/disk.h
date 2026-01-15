/* disk.h */
#pragma once
#include <stdio.h>
#include "osapi.h"

#define DriveC 0x01 /* 0001 */
#define DriveD 0x02 /* 0010  */
#define BasePath $1 "/Users/zoowa/Desktop/llp/ddos/drives/disk."

struct internal packed s_disk
{
    int32 fd;
    int16 blocks;
    int8 drive : 2;
};

typedef struct s_disk disk;

internal disk *dattach(int8);
internal void ddetach(disk *);
internal void dshow(disk *);