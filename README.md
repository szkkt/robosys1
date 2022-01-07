# 課題1

ラズパイ4を使ってLEDを光らせる。

16までの数を二進数で表現した。

便宜的に1、0をLEDの点滅で表現した。1が点灯で0が消灯である。

一定の間隔で数が増えていく。


# 操作手順
入力が0の時、上記の挙動が行われる。入力が1の時、すべてのLEDが消灯される。

$make

$sudo insmod myled.ko

$sudo chmod 666 /dev/myled0

$echo 0 > /dev/myled0

$echo 1 > /dev/myled0
$ vi myled.c



# 動画URL
https://youtu.be/PCoHDXEsVpM
