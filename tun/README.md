## Feat

1. 执行 `gcc -o tun tun.c && sudo ./tun` 启动进程监听 /dev/net/tun 设备。进程启动后，会输出设备的名称。
2. 为 tun0 添加 IP，并打开设备

```sh
# 查看设备状态，设备现在处于 DOWN 状态
# POINTOPOINT 表示 tun0 是个虚拟点对点设备
ø> ip addr show dev tun0
19: tun0: <POINTOPOINT,MULTICAST,NOARP> mtu 1500 qdisc noop state DOWN group default qlen 500
    link/none

# 为 tun0 设备添加 ip 地址
# TODO: 我的程序中并没有做任何发送响应包的事情，但是为设备加上 IP 地址后就可以 ping 通了
ø> sudo ip addr add 192.128.1.2/24 dev tun0

# 启动设备
ø> sudo ip link set tun0 up

# 查看设备，此时设备不是 DOWN 状态了
ø> ip addr show dev tun0
20: tun0: <POINTOPOINT,MULTICAST,NOARP,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UNKNOWN group default qlen 500
    link/none
    inet 192.128.1.2/24 scope global tun0
       valid_lft forever preferred_lft forever
    inet6 fe80::e385:1d71:97f5:6864/64 scope link stable-privacy
       valid_lft forever preferred_lft forever
```

3. 抓包

```
# 在一个终端开启了抓包程序
ø> sudo tcpdump -i tun0
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on tun0, link-type RAW (Raw IP), capture size 262144 bytes

03:09:13.762467 IP6 ubuntu-bionic > ip6-allrouters: ICMP6, router solicitation, length 8
03:09:50.365657 IP6 ubuntu-bionic > ip6-allrouters: ICMP6, router solicitation, length 8

# 在另一个终端中 ping 该设备
# ping 包全部都响应了，但是抓包程序中没看到对应的包，怀疑 ping 包发错位置了
ø> ping -c 4 192.128.1.2
PING 192.128.1.2 (192.128.1.2) 56(84) bytes of data.
64 bytes from 192.128.1.2: icmp_seq=1 ttl=64 time=0.026 ms
64 bytes from 192.128.1.2: icmp_seq=2 ttl=64 time=0.028 ms
64 bytes from 192.128.1.2: icmp_seq=3 ttl=64 time=0.026 ms
64 bytes from 192.128.1.2: icmp_seq=4 ttl=64 time=0.027 ms

--- 192.128.1.2 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3145ms
rtt min/avg/max/mdev = 0.026/0.026/0.028/0.006 ms
```
