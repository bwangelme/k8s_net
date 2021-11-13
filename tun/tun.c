#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int tun_alloc(int flags) {
    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";

    fd = open(clonedev, O_RDWR);
    if (fd < 0) {
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;

    err = ioctl(fd, TUNSETIFF, (void *) &ifr);
    if (err < 0) {
        close(fd);
        return err;
    }

    printf("打开了 tun/tap 设备 %s，开始读取数据\n", ifr.ifr_name);
    return fd;
}

int main(int argc, char *argv[])
{
    int tun_fd, nread;
    char buffer[1500];

    // FLAGS:
    //  IFF_TUN: TUN 设备（没有 Ethernet 头）
    //  IFF_TAP: TAP 设备
    //  IFF_NO_PI: 不提供包信息
    tun_fd = tun_alloc(IFF_TUN | IFF_NO_PI);

    if (tun_fd < 0) {
        perror("分配接口失败");
        exit(1);
    }

    while(1) {
        nread = read(tun_fd, buffer, sizeof(buffer));
        if (nread < 0) {
            perror("从接口读取数据失败");
            close(tun_fd);
            exit(1);
        }
        printf("从 tnp/tap 设备读取了 %d 字节\n", nread);
    }

    return 0;
}
