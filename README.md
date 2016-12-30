# smart-radio
An ESP8266 and VS1053 driven WebRadio and Internet music player.

## Features

+ Internet radio
+ Internet music player
+ OLED display
+ Remote control over computers or mobile devices
+ OTA(Over The Air) firmware update

## Enclosure

+ 3D printed case
+ Specially design for Bose Sound Mini II as dock

## Services
+ **FM / Radio** - Support MP3 and ACC. In my presets, I use a Chinese WebRadio service provider named   [QingtingFM](http://qingting.fm/)
+ **Music** - Support MP3, ACC and wma. By default, it will automatically load my favourite music collection from [Netease Music](http://music.163.com).

## Hardwares

+ **ESP8266** (ESP12 / NodeMCU / WeMos)

  Yet, the most popular WiFi MCU.

  >   **US$2** on AliExpress

  ![](http://www.cnx-software.com/wp-content/uploads/2016/02/Wemos_D1_mini.jpg)

+ **VS1053** - US$6.50

  A powerful MP3 / ACC / WMA decoder.

  > Less than **US$7** on AliExpress.

  ![](http://img.dxcdn.com/productimages/sku_221526_1.jpg)


## Wiring
| ESP8266 / ESP-12 | Wired to VS1053 | Wired to Rest |
| ---------------- | --------------- | ------------- |
| D0               | XDCS            |               |
| D1               | XCS             |               |
| D2               | DREQ            |               |
| D3               |                 |               |
| D4               |                 |               |
| D5               | SCLK            |               |
| D6               | MISO            |               |
| D7               | MOSI            |               |
| D8               |                 |               |
| D9               |                 |               |
| D10              |                 |               |
| RST              | XRST            |               |