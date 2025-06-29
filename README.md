# Virtual Sensor Linux Kernel Module

A simple Linux kernel module that registers a character device (/dev/virtual_sensor) and simulates dynamic sensor readings using kernel-space random data.

## Features
- Character device driver with auto "/dev/virtual_sensor" creation
- Dynamic sensor values with each read
- Kernel-to-user communication using "copy_to_user()"
- User-space C program to read sensor values

## Build and Test

```bash
make
sudo insmod virtual_sensor.ko
cat /dev/virtual_sensor
```
## Sample Output
```bash
Sensor reading: 42
```

### Read from C Program (read_sensor.c)
```bash
gcc read_sensor.c -o read_sensor
./read_sensor
```
### Unload
```bash
sudo rmmod virtual_sensor
make clean
```

Enoch Owoade – Engineering Technology @ Grambling State University


