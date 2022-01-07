# 課題1

ラズパイ4を使ってLEDを光らせる。

16までの数を二進数で表現した。

便宜的に1、0をLEDの点滅で表現した。1が点灯で0が消灯である。

一定の間隔で数が増えていく。

# 環境
Raspberry Pi 4 Model B

抵抗(150Ω) ×4

LED ×4　

ジャンパー線 ×8

ブレッドボード ×1

![回路](https://github.com/szkkt/robosys1/blob/main/5.jpg)

LEDのアノード側を画像正面左から順番にGPIO25,18,22,23,24のピンに、カソード側は抵抗を挟みGNDのピンにそれぞれジャンパー線を経由して接続している。



# インストール
    $make
    $sudo insmod myled.ko
    $sudo chmod 666 /dev/myled0


# アンインストール
    $sudo rmmodo myled


# 操作方法
    $echo 0 > /dev/myled0
    $echo 1 > /dev/myled0

入力が0の時、上記の挙動が行われる。入力が1の時、すべてのLEDが消灯される。


# 動画URL
https://youtu.be/PCoHDXEsVpM
