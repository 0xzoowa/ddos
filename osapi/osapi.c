/* osapi.c*/
#include "osapi.h"

public error errnumber = 0;
public bool initialized = false;

private int8 fds[256];
struct stat _;

private int16 stringlen(int8 *str)
{
    int16 count;
    int8 *p;
    for (p = str, count = 0; *p; p++, count++)
        ;
    return count;
}

internal void copy(int8 *dest, int8 *src, int16 len)
{
    int8 *dp;
    int8 *sp;
    int8 c;

    for (dp = dest, sp = src, c = len; c; dp++, sp++, c--)
    {
        *dp = *sp;
    }
    return;
}
internal int8 *strnum(int8 *str, int8 num)
{
    static int8 buff[256];
    int16 n;
    int16 c;

    n = stringlen(str);
    if (!n)
        return str;
    else if (num > 250)
        return str;
    else if (num > 9)
        return str;

    c = num + 0x30;

    zero($1 buff, sizeof(buff)); // initialize array
    copy($1 buff, str, n);
    buff[n++] = c;
    buff[n] = 0;

    return buff;
}

/**
 *  fd=0 -> error
 *  fd=1 -> stdin
 *  fd=2 -> stdout
 */
private bool isopen(fd file)
{
    signed int posixfd;

    if (file < 3) // 3+ points to user file
        return false;
    posixfd = getposixfd(file);
    if (!posixfd) // if posixfd is 0 - error     fds[0] = 1;  fds[1] = 2;
        return false;
    if ((fstat(posixfd, &_)) == -1)
        return false;
    return true;
}

public bool load(fd file, int8 c)
{
    int8 buf[2];
    signed int n;
    signed int posixfd;

    assert_initialized();

    if (file > 1)
    {
        if (!isopen(file))
            reterr(ErrBadFD);
    }
    posixfd = getposixfd(file);
    if (!posixfd)
        reterr(ErrBadFD);

    posixfd = (posixfd == 1) ? 0 : (posixfd == 2) ? 1
                                                  : (posixfd);
    *buf = *(buf + 1) = (int8)0;
    *buf = c;
    n = write(posixfd, $c buf, 1);
    if (n != 1)
        reterr(ErrIO);
    return true;
}

public int8 store(fd file)
{

    // store((int8)0);
    int8 buf[2];
    signed int n;
    signed int posixfd;

    assert_initialized();

    if (file > 2)
    {
        if (!isopen(file))
            reterr(ErrBadFD);
    }
    posixfd = getposixfd(file); //    fds[0] = 1;
    if (!posixfd)
        reterr(ErrBadFD);

    posixfd = (posixfd == 1) ? 0 : (posixfd == 2) ? 1
                                                  : (posixfd);
    *buf = *(buf + 1) = (int8)0;
    n = read(posixfd, $c buf, 1);
    if (n != 1)
        reterr(ErrIO);
    return (int8)*buf;
}

private void setupfds()
{
    zero($1 fds, sizeof(fds));
    fds[0] = 1;
    fds[1] = 2;
    return;
}

internal void zero(int8 *str, int16 size)
{
    int8 *p;
    int16 n;

    for (n = size, p = str; n; n--, p++)
    {
        *p = (int8)0;
    }
    return;
}

void init(void)
{
    setupfds();
    initialized = true;
}