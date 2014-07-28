stm32f103
=========

Сделать `cd` в папку `exec`. Скомпилить все командой `make`. Заливка программы во флеш производится при нормальном положении boot-джамперов и через несколько миллисекунд после втыкания USB в порт. `dfu-util` натравить на `main.bin`:
`sudo dfu-util -d 1337:1eaf -a 1 -D ./main.bin -R dfu-util 0.7`
Процедура подтыкания и запуска загрузки не до конца ясна :grin:

Если USB-бутлоадер слетает, то `mesg | tail` начинает выдавать ошибки и `lsusb` не видит устройство `1337:1eaf`.
В этом случае придется перезалить его через stm32flash. Сделать `cd` в папку `boot-flasher`. Собрать `make`ом всё в папке `bootloader`. Подключить плату через FTDI конвертер к ПК, переставить джампер BOOT0 в правую позицию и запустить такое:
`sudo ./stm32flash -w ./main.bin -v -g 0x0 /dev/ttyUSB0`
После заливки переставить джампер обратно, отключить всё от ПК, подключить плату напрямую в USB
