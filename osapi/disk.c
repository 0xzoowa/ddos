/* disk.c */
#include "disk.h"

/**
 * attached : b1001 -> drive 0 and 3 attached
 */

internal int8 attached;

public void dinit()
{
    disk *dd[2];
    block bl = {[0 ... Blocksize - 1] = (int8)0x06};
    block in;
    attached = 0;
    bool x;
    int16 i;

    *dd = dattach(1);
    *(dd + 1) = dattach(2);

    dshow(*dd);
    printf("attached: %.02hhx\n", attached);

    x = dwrite(*dd, bl, 0);
    printf("x= %s\n", x ? "true" : "false");

    bool y = dread(*dd, in, 1);
    printf("y= %s\n", y ? "true" : "false");
    for (int16 n = 0; n < Blocksize; n++)
    {
        printf("%d= %02hhx\n", n, in[n]);
    }

    ddetach(*dd);
    ddetach(*(dd + 1));

    return;
}
internal void dshow(disk *dd)
{
    if (dd)
        printf(
            "drive 0×%.02hhx \n"
            " fd=%d\n"
            " blocks=%d\n\n",
            (char)dd->drive, $i dd->fd, $i dd->blocks);
    return;
}

internal void ddetach(disk *dd)
{
    int8 x;
    if (!dd)
        return;

    close(dd->fd);
    x = ~(dd->drive) & attached;
    attached = x;

    free(dd);
    return;
}

internal disk *dattach(int8 drive)
{

    int8 *file;
    signed int tmp;
    int16 size;
    disk *dd;
    struct stat sbuf;

    if ((drive == 1) || (drive == 2))
        ;
    else
        return (disk *)0;

    if (attached & drive) // b1001(already attached drives) & b0010(drive to atttach) -> 0000
        return (disk *)0;

    size = sizeof(disk); // struct internal packed s_disk
    dd = (disk *)malloc(size);
    if (!dd)
        return (disk *)0;
    zero($1 dd, size);
    file = strnum(BasePath, drive);
    tmp = open($c file, O_RDWR);
    if (tmp < 3)
    {
        free(dd);
        return (disk *)0;
    }
    dd->fd = $4 tmp;
    tmp = fstat($i dd->fd, &sbuf);
    if (tmp || !sbuf.st_blocks)
    {
        close(dd->fd);
        free(dd);
        return (disk *)0;
    }
    dd->blocks = ($2 sbuf.st_blocks) - 1;
    dd->drive = drive;

    attached |= drive;
    return dd;
}