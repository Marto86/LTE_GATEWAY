1) Connection

  The board is using the wire/not USB/other JTAG connection on the IO pins.
  The used GPIOs and their connection is shown here:
  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-guides/jtag-debugging/configure-other-jtag.html
   
2) JTAG adapter
The vscode example configuration is for J-Link adapter with WinUSB driver (for OpenOCD), installed by zadig-2.7.exe
 OpenOCD JTAG adapter from Olimex should be easier to install and setup (Natevly)

3) OpenOCD

Run "OpenOCD: Start openOCD" task from the example vscode task configuration

openocd -f interface\jlink.cfg -f target\esp32s3.cfg -c "adapter speed 8000"

4) VScode

Various GDB extensions were tested (espidf debug adapter, cppdbg,  gdb), but only "gdb" type was working.
It is installed by VScode "Native Debug" extension

In launch.json се it is described in the following way
    {
      "type": "gdb",
      "request": "launch",
      "name": "Debug ESP32",
      "target": "${workspaceFolder}/build/${command:espIdf.getProjectName}.elf",
      "cwd": "${workspaceFolder}",
      "gdbpath": "${command:espIdf.getXtensaGdb}", // Path of the toolchain     
      "autorun": [
        "target remote :3333", // Connect to OpenOCD
        "mon reset halt", // Reset the chip and keep the CPUs halted
        "set remote hardware-watchpoint-limit 2", // Restrict GDB to using two hardware watchpoints supported by ESP32
        "flushregs", // Force GDB to get new state from the target
        "thb app_main", // Insert a temporary hardware breakpoint at the main function
        "c" // Resume the program. It will then stop at breakpoint inserted at app_main
      ]
    }

5) ESP32 chip configuration for external/IO JTAG debug

ESP32S3 JTAG by default is connected to the USB interface. The external/IO JTAG pins are deactivated and do not work. One option to enable it is by the bootstrap GPIO3 pin, but there's easier way by programming the EFUSE.To disable the USB JTAG and activate the other/external/IO JTAG, the DIS_USB_JTAG efuse should be programmed in the following way:

python C:\Work\ESP\esp-idf\components\esptool_py\esptool\espefuse.py -p COM12 burn_efuse DIS_USB_JTAG 1

6) Useful documentation source
   
https://gist.github.com/motla/ab7fdcf14303208996c40ca7fefe6f11