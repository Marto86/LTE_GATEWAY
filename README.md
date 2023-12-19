# Injeli LTE gateway

The project is firemware for ESP32-S3 SoC, which is used to read data over Modebus protocol and publish periodically the changes to a MQTT broker by using 4G Cat.1 modem connected to the USB host interface of ESP32S3. The USB LTE connection is done through PPP to access the Internet, with the help of ESP32-SX Wi-Fi softAP function.

**Features Supported:**

* Modbus communication to a RAK heatpump
* USB CDC host communication to a 4G LTE USB modem
* Compatible with mainstream 4G module AT commands
* PPP dial-up
* Wi-Fi hotspot
* 4G module status management
* Status led indicator
* Secure MQTT connection to a remote broker

## Hardware requirement

**Supported ESP Soc:** 

* ESP32-S2
* ESP32-S3

> We recommend using ESP modules or chips that integrate 4MB above Flash, and 2MB above PSRAM. 
> The project does not enable PSRAM by default, but this can be enabled. In theory, increasing the Wi-Fi buffer size can increase the average data throughput rate.

**Supported 4G Cat.1 Module:** 

* Huawei A3372H
* ML302-DNLM/CNLM
* Air724UG-NFM
* EC600N-CNLA-N05
* EC600N-CNLC-N06
* SIMCom A7600C1

> There are different submodels of the above modules. The communication stands may be different. For example, LTE-FDD 5(UL)/10(DL), LTE-TDD 1(UL)/8(DL). And endpoint address may different also, if you encounter problems, try modifying parameters using a custom device mode.

**Hardware wiring:**

Default GPIO as follows:

|        Functions         |  GPIO   |     Notes     |
| :----------------------: | :-----: | :-----------: |
|    **USB D+ (green)**    | **20**  | **Necessary** |
|    **USB D- (white)**    | **19**  | **Necessary** |
|     **GND (black)**      | **GND** | **Necessary** |
|      **+5V (red)**       | **+5V** | **Necessary** |
|   Modem Power Control    |   12    | Not Necessary |
| **Modem Reset Control**  | **13**  | **Necessary** |
| System Status LED (red)  |   15    | Not Necessary |
| Wi-Fi Status LED (blue)  |   17    | Not Necessary |
| Modem Status LED (green) |   16    | Not Necessary |

> User can change GPIO in `menuconfig -> 4G Modem Configuration -> gpio config`

## User Guide

**Wi-Fi SSID and Password:**

User can modify SSID and Password in `menuconfig -> 4G Modem Configuration -> WiFi soft AP `

1. Default Wi-Fi: `LTEgateway`
2. Default Password: `12345678`

**LED Indicator Status:**

|     Indicator     |    Blink    |                Status                 |
| :---------------: | :---------: | :-----------------------------------: |
| **System (Red)**  | extinguish  |                  NA                   |
|                   | quick blink |           restart 4G Modem            |
|                   |    solid    | internal error(check SIM card please) |
| **Wi-Fi (Blue)**  | extinguish  |                  NA                   |
|                   | slow blink  |     waiting for device connection     |
|                   |    solid    |           device connected            |
| **Modem (Green)** | extinguish  |                  NA                   |
|                   | slow blink  |    waiting for internet connection    |
|                   |    solid    |          internet connected           |

## How to build the project


1. Clone/upgrade ESP-IDF GIT repository and switch to the IDF `release/v4.4` branch. If needed update all the submodules recursively and install the dependancies by running `install.sh` script
   
2. To setup the project in VScode, open it in VScode and run the "ESP-IDF: Add vscode configuration folder" from the ESP-IDF extension command.

3. Setup the `ESP-IDF` environment either by running the `export.sh` script, or by setting the paths and variables in the ESP-IDF VScode extension.

The Linux method is as follows, other platforms please refer [Set up the environment variables](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#step-4-set-up-the-environment-variables)

    ```bash
    . $HOME/esp/esp-idf/export.sh
    ```

4. Set the IDF build target to `esp32s3`

    ```bash
    idf.py set-target esp32s2
    ```

or use the VScode "Set IDF-ESP Target" task or click at the label at the bottom ribbon.

5. Optional: make any project configuration, e.g. select the Cat.1 module model `Menuconfig → Component config → ESP-MODEM → Choose Modem Board`, if the your module is not in the list, please refer to `Other 4G Cat.1 Module Adaptation Methods` to configure.

    ![choose_modem](./_static/choose_modem.png)

6. Build, download, check log output

    ```bash
    idf.py build flash monitor
    ```

Optionally in VScode either use "Build" task or press "CTRL-SHIFT-B" to build the project.

**Log**

```
I (0) cpu_start: Starting scheduler on APP CPU.
W (385) main: Force reset 4g board
I (389) gpio: GPIO[13]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (3915) main: ====================================
I (3915) main:      Ingeli 4G LTE-Modbus Gateway
I (3915) main: ====================================
| I (3920) gpio: GPIO[15] | InputEn: 0 | OutputEn: 1 | OpenDrain: 0 | Pullup: 0 | Pulldown: 0 | Intr:0 |
| I (3929) gpio: GPIO[17] | InputEn: 0 | OutputEn: 1 | OpenDrain: 0 | Pullup: 0 | Pulldown: 0 | Intr:0 |
| I (3939) gpio: GPIO[16] | InputEn: 0 | OutputEn: 1 | OpenDrain: 0 | Pullup: 0 | Pulldown: 0 | Intr:0 |
I (3998) USB_HCDC: usb driver install succeed
I (3998) USB_HCDC: Waitting Device Connection
I (4028) USB_HCDC: USB Port=1 init succeed
I (4028) USB_HCDC: Waitting USB Connection
I (4028) USB_HCDC: Port power: ON
I (4281) USB_HCDC: line 263 HCD_PORT_EVENT_CONNECTION
I (4281) USB_HCDC: Resetting Port
I (4341) USB_HCDC: Port reset succeed
I (4341) USB_HCDC: Getting Port Speed
I (4341) USB_HCDC: Port speed = 1
I (4343) USB_HCDC: USB Speed: full-speed
I (4347) USB_HCDC: Set Device Addr = 1
I (4352) USB_HCDC: Set Device Addr Done
I (4356) USB_HCDC: Set Device Configuration = 1
I (4362) USB_HCDC: Set Device Configuration Done
I (4367) USB_HCDC: Creating bulk in pipe
I (4371) USB_HCDC: Creating bulk out pipe
I (4378) USB_HCDC: Device Connected
| I (4380) gpio: GPIO[12] | InputEn: 0 | OutputEn: 1 | OpenDrain: 0 | Pullup: 0 | Pulldown: 0 | Intr:0 |
| I (4390) gpio: GPIO[13] | InputEn: 0 | OutputEn: 1 | OpenDrain: 0 | Pullup: 0 | Pulldown: 0 | Intr:0 |
I (7375) modem_board: Modem PPP Started
I (7376) modem_board: re-start ppp, retry=1
I (7386) esp-netif_lwip-ppp: Connected
I (7386) esp-netif_lwip-ppp: Name Server1: 114.114.114.114
I (7387) esp-netif_lwip-ppp: Name Server2: 0.0.0.0
I (7391) modem_board: IP event! 6
I (7395) modem_board: Modem Connected to PPP Server
I (7401) modem_board: ppp ip: 10.250.188.169, mask: 255.255.255.255, gw: 192.168.0.1
I (7409) modem_board: Main DNS: 114.114.114.114
I (7415) modem_board: Backup DNS: 0.0.0.0
I (7420) main: ap dns addr(auto): 114.114.114.114
I (7425) pp: pp rom version: e7ae62f
I (7429) net80211: net80211 rom version: e7ae62f
I (7435) wifi:wifi driver task: 600fee20, prio:23, stack:6656, core=0
I (7440) system_api: Base MAC address is not set
I (7446) system_api: read default base MAC address from EFUSE
I (7462) wifi:wifi firmware version: cca42a5
I (7462) wifi:wifi certification version: v7.0
I (7462) wifi:config NVS flash: enabled
I (7464) wifi:config nano formating: disabled
I (7468) wifi:Init data frame dynamic rx buffer num: 32
I (7473) wifi:Init management frame dynamic rx buffer num: 32
I (7478) wifi:Init management short buffer num: 32
I (7483) wifi:Init dynamic tx buffer num: 32
I (7487) wifi:Init static tx FG buffer num: 2
I (7491) wifi:Init static rx buffer size: 1600
I (7495) wifi:Init static rx buffer num: 10
I (7499) wifi:Init dynamic rx buffer num: 32
I (7503) wifi_init: tcpip mbox: 32
I (7507) wifi_init: udp mbox: 6
I (7511) wifi_init: tcp mbox: 6
I (7515) wifi_init: tcp tx win: 5744
I (7519) wifi_init: tcp rx win: 5744
I (7523) wifi_init: tcp mss: 1440
I (7527) wifi_init: WiFi IRAM OP enabled
I (7532) wifi_init: WiFi RX IRAM OP enabled
I (7537) wifi_init: LWIP IRAM OP enabled
I (7542) phy_init: phy_version 302,291a31f,Oct 22 2021,19:22:08
I (7596) wifi:mode : softAP (7c:df:a1:e0:32:cd)
I (7598) wifi:Total power save buffer number: 16
I (7598) wifi:Init max length of beacon: 752/752
I (7598) wifi:Init max length of beacon: 752/752
I (7607) wifi:Total power save buffer number: 16
I (7607) modem_wifi: softap ssid: LTEgateway password: 12345678
I (7613) modem_wifi: NAT is enabled
```

## Other 4G Cat.1 module adaptation methods

1. Confirm whether the 4G module **supports USB Fullspeed mode**;
2. Confirm whether the 4G module **supports USB PPP dial-up interface**;
3. Confirm that the **4G SIM card is activated** and the Internet access is turned on;
4. Confirm that the **necessary signal wires have been connected** in accordance with the hardware wiring;
5. Confirm the module's PPP interface input endpoint (IN) and output endpoint (OUT) addresses, and modify the following options in `menuconfig`: 

   * Choose a `User Defined` board:
   ```
   Component config → ESP-MODEM → Choose Modem Board → User Defined
                                
   ```
   * Configure the endpoint address of 4G Modem:
   ```
   Component config → ESP-MODEM → USB CDC endpoint address config
                                        → Modem USB CDC IN endpoint address
                                        → Modem USB CDC OUT endpoint address
   ```

6. Check outputs log to confirm that the `AT` command can be executed;

> The basic AT commands supported by different Cat.1 chip platforms are roughly the same, but there may be some special commands that need to be supported by users.


## Debugging method 

**1. Debugging mode**

Enable the `4G Modem Configuration -> Dump system task status` option in `menuconfig` to print task detailed information, or open the `Component config → USB Host CDC ->Trace internal memory status` option to print usb internal buffer usage information.

	```
    I (79530) main: Task dump
    I (79531) main: Load    Stack left      Name    PRI
    I (79531) main: 3.24    1080    main    1
    I (79532) main: 95.25   1248    IDLE    0
    I (79536) main: 0.03    1508    bulk-out        6
    I (79541) main: 0.03    1540    port    9
    I (79546) main: 0.01    1752    Tmr Svc         1
    I (79550) main: 0.04    2696    tiT     18
    I (79554) main: 1.21    1352    usb_event       4
    I (79559) main: 0.01    1532    bulk-in         5
    I (79564) main: 0.05    3540    esp_timer       22
    I (79569) main: 0.13    4632    wifi    23
    I (79573) main: 0.00    1532    dflt    8
    I (79577) main: 0.00    1092    sys_evt         20
    I (79582) main: Free heap=37088 bigst=16384, internal=36968 bigst=16384
    I (79589) main: ..............
    
    I (79593) USB_HCDC: USBH CDC Transfer Buffer Dump:
    I (79599) USB_HCDC: usb transfer Buffer size, out = 3000, in = 1500
    I (79606) USB_HCDC: usb transfer Max packet size, out = 46, in = 48
    
    I (79613) USB_HCDC: USBH CDC Ringbuffer Dump:
    I (79618) USB_HCDC: usb ringbuffer size, out = 15360, in = 15360
    I (79625) USB_HCDC: usb ringbuffer High water mark, out = 46, in = 48
	```

**2. Performance optimization**

1. If there is a requirement for throughput, please select the `FDD` standard module and operator;
2. Modify `APN` to the name provided by the operator `menuconfig -> 4G Modem Configuration -> Set Modem APN`;
3. Configure ESP32-Sx CPU to 240MHz (`Component config → ESP32S2-specific → CPU frequency`), if it supports dual cores, please turn on both cores at the same time;
4. Add and enable PSRAM (`Component config → ESP32S2-specific → Support for external`), and increase the PSRAM clock frequency (`Component config → ESP32S2-specific → Support for external → SPI RAM config → Set RAM clock speed`) select 80MHz. And open `Try to allocate memories of WiFi and LWIP in SPIRAM firstly.` in this directory;
5. Increase the FreeRTOS Tick frequency `Component config → FreeRTOS → Tick rate` to `1000 Hz`;
6. Optimization of other application layers.

## Performance test

**Test environment:**

* ESP32-S3, CPU 240 MHz
* 4 MB flash, no PSRAM
* ML302-DNLM module
* A1 Bulgaria 4G SIM card
* Normal office environment

**Test result:**

| Test item |   Peak   | Average |
| :-------: | :------: | :-----: |
| Download  | 6.4 Mbps | 4 Mbps  |
|  Upload   |  5 Mbps  | 2 Mbps  |

> **4G Cat.1 theoretical peak download rate is 10 Mbps, peak upload rate is 5 Mbps**
> The actual communication rate is affected by the operator's network, test software, Wi-Fi interference, and the number of terminal connections, etc.


**Ingeli provisioning**
1. Modify nvs_ingeli_provisioning.csv with the parameters for the device on the assembly line
2. Execute command:
python3 /home/georgi/esp/esp-idf/components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py generate provisioning.csv provisioning.bin 0x4000
3. Flash the data to the device with command:
python3 /home/georgi/esp/esp-idf/components/partition_table/parttool.py --port "/dev/ttyUSB0" write_partition --partition-name=provisioning --input "./provisioning.bin"


python3 /home/georgi/esp/esp-idf/components/partition_table/parttool.py --port "/dev/ttyUSB0" read_partition --partition-type=data --partition-subtype=nvs "./nvs_ingeli_provisioning_read_from_flash.bin"


**OTA**
0. We need to discuss the OTA server certificate implementation if OTA server is going to be https
1. Upload new firmware binary to web server
2. Execute start OTA command to topic  

1.Install mosquitto-2.0.14 (If not already done)
2.Paste certificate in to installation folder (If not already done)
3.In new terminal subscribe for all topics in mosquitto with command:
mosquitto_sub -h mqtts.pool.mytech-connect.dev --cafile ingeliDevCA01.crt  -t "#" -p 28883 -F "@Y-@m-@d @H:@M:@S : %t : %x" --insecure -u "wildcard0" -P "39d544f0ac583e1" -i wildcard0
4.In new terminal execute get firmware version command:
mosquitto_pub -h mqtts.pool.mytech-connect.dev --cafile ingeliDevCA01.crt -t "poly/cmd/1/30123/info" -p 28883 --insecure -u "wildcard0" -P "39d544f0ac583e1" -i wildcard0 -d -m "get_firmware"
5.In the terminal with all topics subscription (point 3) must receive message with the firmware version. Example:
"Firmware version: 0.0.1"
6.Prepare OTA update binary(new firmware version) and upload to web server
7.Run OTA update command with mosquitto_pub:
mosquitto_pub -h mqtts.pool.mytech-connect.dev --cafile ingeliDevCA01.crt -t "poly/cmd/1/30123/otalink" -p 28883 --insecure -u "wildcard0" -P "39d544f0ac583e1" -i wildcard0 -d -m "http://rnd.bg/ingeli/LTEgateway.bin"
8.After around 2-3 minutes in new terminal execute get firmware version command:
mosquitto_pub -h mqtts.pool.mytech-connect.dev --cafile ingeliDevCA01.crt -t "poly/cmd/1/30123/info" -p 28883 --insecure -u "wildcard0" -P "39d544f0ac583e1" -i wildcard0 -d -m "get_firmware"
7.In the terminal with all topics subscription (point 3) must receive message with the new firmware version. Example:
"Firmware version: 0.0.2"