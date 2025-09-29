# Switchtec CLI Command Reference

## Table of Contents

- [help](#help)
- [version](#version)
- [list](#list)
- [info](#info)
- [gui](#gui)
- [status](#status)
- [bw](#bw)
- [latency](#latency)
- [events](#events)
- [event-wait](#event-wait)
- [log-dump](#log-dump)
- [log-parse](#log-parse)
- [test](#test)
- [temp](#temp)
- [port-bind-info](#port-bind-info)
- [port-bind](#port-bind)
- [port-unbind](#port-unbind)
- [hard-reset](#hard-reset)
- [fw-update](#fw-update)
- [fw-info](#fw-info)
- [fw-toggle](#fw-toggle)
- [fw-read](#fw-read)
- [fw-img-info](#fw-img-info)
- [evcntr](#evcntr)
- [evcntr-setup](#evcntr-setup)
- [evcntr-show](#evcntr-show)
- [evcntr-del](#evcntr-del)
- [evcntr-wait](#evcntr-wait)
- [diag](#diag)
  - [diag w2h](#diag-w2h)
  - [diag eye](#diag-eye)
  - [diag list-mrpc](#diag-list-mrpc)
  - [diag loopback](#diag-loopback)
  - [diag pattern](#diag-pattern)
  - [diag port-eq-txcoeff](#diag-port-eq-txcoeff)
  - [diag port-eq-txfslf](#diag-port-eq-txfslf)
  - [diag port-eq-txtable](#diag-port-eq-txtable)
  - [diag rcvr-extended](#diag-rcvr-extended)
  - [diag rcvr-obj](#diag-rcvr-obj)
  - [diag ltssm-log](#diag-ltssm-log)
- [gas](#gas)
  - [gas dump](#gas-dump)
  - [gas read](#gas-read)
  - [gas write](#gas-write)
- [mfg](#mfg)
  - [mfg ping](#mfg-ping)
  - [mfg info](#mfg-info)
  - [mfg secure-version-get](#mfg-secure-version-get)
  - [mfg mailbox](#mfg-mailbox)
  - [mfg image-list](#mfg-image-list)
  - [mfg image-select](#mfg-image-select)
  - [mfg fw-transfer](#mfg-fw-transfer)
  - [mfg fw-execute](#mfg-fw-execute)
  - [mfg boot-resume](#mfg-boot-resume)
  - [mfg state-set](#mfg-state-set)
  - [mfg config-set](#mfg-config-set)
  - [mfg kmsk-entry-add](#mfg-kmsk-entry-add)
  - [mfg debug-unlock](#mfg-debug-unlock)
  - [mfg debug-lock-update](#mfg-debug-lock-update)
  - [mfg debug-unlock-token](#mfg-debug-unlock-token)
  - [mfg die-trace-prog](#mfg-die-trace-prog)
  - [mfg chip-serial-prog](#mfg-chip-serial-prog)
  - [mfg sku-set](#mfg-sku-set)
  - [mfg sku-get](#mfg-sku-get)
  - [mfg otp-write-prot](#mfg-otp-write-prot)
  - [mfg sjtag-mode-set](#mfg-sjtag-mode-set)
  - [mfg sjtag-uuid-set](#mfg-sjtag-uuid-set)
  - [mfg sjtag-key-set](#mfg-sjtag-key-set)
  - [mfg sjtag-key-verify](#mfg-sjtag-key-verify)
  - [mfg sjtag-key-lock](#mfg-sjtag-key-lock)
  - [mfg selftest-pol-set](#mfg-selftest-pol-set)
  - [mfg otp-prog-mode-set](#mfg-otp-prog-mode-set)
  - [mfg otp-smprg-set](#mfg-otp-smprg-set)
  - [mfg otp-mode-status-get](#mfg-otp-mode-status-get)
  - [mfg otp-cust-id-prog](#mfg-otp-cust-id-prog)
  - [mfg otp-config-read](#mfg-otp-config-read)
  - [mfg otp-config-prog](#mfg-otp-config-prog)
  - [mfg sjtag-unlock](#mfg-sjtag-unlock)
  - [mfg otp-ini2bin](#mfg-otp-ini2bin)

## help
<a name="help"></a>

```text
switchtec-5.7
Usage: ./switchtec <command> [<device>] [OPTIONS]

The <device> must be a Switchtec device (ex: /dev/switchtec0)

The following are all implemented sub-commands:
  list                           list all Switchtec devices on this machine
  info                           display switch information
  gui                            display a simple ncurses GUI
  status                         display switch port status information
  bw                             measure the traffic bandwidth through each port
  latency                        measure the latency of a port
  events                         display events that have occurred
  event-wait                     wait for an event to occur
  log-dump                       dump the firmware log to a file
  log-parse                      parse a binary app log or mailbox log to a text file
  test                           test if the Switchtec interface is working
  temp                           display the die temperature
  port-bind-info                 display physical port binding information
  port-bind                      bind a logical port to a physical port
  port-unbind                    unbind a logical port from a physical port
  hard-reset                     perform a hard reset of the switch
  fw-update                      upload a new firmware image to flash
  fw-info                        return information on the currently flashed firmware
  fw-toggle                      toggle the active and inactive firmware partitions
  fw-read                        read a firmware image from flash
  fw-img-info                    display information for a firmware image
  evcntr                         display event counters
  evcntr-setup                   configure an event counter
  evcntr-show                    display an event counter's configuration
  evcntr-del                     deconfigure an event counter
  evcntr-wait                    wait for an event counter to exceed its threshold
  version                        show the program version
  help                           display this help

See './switchtec help <command>' for more information on a specific command

The following are all installed extensions:
  diag            Diagnostic Information
  gas             Global Address Space Access (dangerous)
  mfg             Manufacturing Process Commands

See './switchtec <extension> help' for more information on a extension
```

## version
<a name="version"></a>

```text
switchtec cli version 5.7
```

## list
<a name="list"></a>

```text
Usage: ./switchtec help./switchtec list [OPTIONS]

list all Switchtec devices on this machine


Options:
  [  --verbose, -v ]                    --- print additional device
                                            information
```

## info
<a name="info"></a>

```text
Usage: ./switchtec help./switchtec info <device> [OPTIONS]

display switch information


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
```

## gui
<a name="gui"></a>

```text
Usage: ./switchtec help./switchtec gui <device> [OPTIONS]

display a simple ncurses GUI


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --all_ports, -a ]                  --- show all ports (including downed
                                            links)
  [  --reset, -r ]                      --- reset byte counters
  [  --refresh=<>, -f <> ]              --- GUI refresh period in seconds
                                            (default: 1 second)
  [  --duration=<>, -d <> ]             --- GUI duration in seconds (-1 =
                                            forever)
  [  --bw_type=<TYPE>, -b <TYPE> ]      --- GUI bandwidth type

Choices for TYPE:
    RAW                                 --- get the raw bandwidth
    PAYLOAD                             --- get the payload bandwidth
```

## status
<a name="status"></a>

```text
Usage: ./switchtec help./switchtec status <device> [OPTIONS]

display switch port status information


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --format=<FMT>, -f <FMT> ]         --- output format (default: normal)
  [  --reset, -r ]                      --- reset byte counters
  [  --verbose, -v ]                    --- print additional information
                                            (only with 'normal' format)

Choices for FMT:
    normal                              --- Human Readable Output
    table                               --- Tabular Output
```

## bw
<a name="bw"></a>

```text
Usage: ./switchtec help./switchtec bw <device> [OPTIONS]

measure the traffic bandwidth through each port


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --time=<NUM>, -t <NUM> ]           --- measurement time in seconds
  [  --verbose, -v ]                    --- print posted, non-posted and
                                            completion results
  [  --bw_type=<TYPE>, -b <TYPE> ]      --- bandwidth type

Choices for TYPE:
    RAW                                 --- get the raw bandwidth
    PAYLOAD                             --- get the payload bandwidth
```

## latency
<a name="latency"></a>

```text
Usage: ./switchtec help./switchtec latency <device> --egress=<NUM> [OPTIONS]

measure the latency of a port


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --time=<NUM>, -t <NUM> ]           --- measurement time in seconds
  [  --egress=<NUM>, -e <NUM> ]         --- physical port ID for the egress
                                            side
  [  --ingress=<NUM>, -i <NUM> ]        --- physical port ID for the ingress
                                            side (default: use all ports)
```

## events
<a name="events"></a>

```text
Usage: ./switchtec help./switchtec events <device> [OPTIONS]

display events that have occurred


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --all, -a ]                        --- show events in all partitions
  [  --reset, -r ]                      --- clear all events
  [  --event=<EVENT>, -e <EVENT> ]      --- clear all events of a specified
                                            type

Choices for EVENT:
    STACK_ERROR                         --- Stack Error
    PPU_ERROR                           --- PPU Error
    ISP_ERROR                           --- ISP Error
    SYS_RESET                           --- System Reset
    FW_EXC                              --- Firmware Exception
    FW_NMI                              --- Firmware Non-Maskable Interrupt
    FW_NON_FATAL                        --- Firmware Non-Fatal Error
    FW_FATAL                            --- Firmware Fatal Error
    TWI_MRPC_COMP                       --- TWI MRPC Completion
    TWI_MRPC_COMP_ASYNC                 --- TWI MRPC Async Completion
    CLI_MRPC_COMP                       --- CLI MRPC Completion
    CLI_MRPC_COMP_ASYNC                 --- CLI MRPC Async Completion
    GPIO_INT                            --- GPIO Interrupt
    GFMS                                --- Global Fabric Management Server
                                            Event
    PART_RESET                          --- Partition Reset
    MRPC_COMP                           --- MRPC Completion
    MRPC_COMP_ASYNC                     --- MRPC Async Completion
    DYN_PART_BIND_COMP                  --- Dynamic Partition Binding
                                            Completion
    AER_IN_P2P                          --- Advanced Error Reporting in P2P
                                            Port
    AER_IN_VEP                          --- Advanced Error Reporting in vEP
    DPC                                 --- Downstream Port Containment Event
    CTS                                 --- Completion Timeout Synthesis
                                            Event
    UEC                                 --- Upstream Error Containment Event
    HOTPLUG                             --- Hotplug Event
    IER                                 --- Internal Error Reporting Event
    THRESH                              --- Event Counter Threshold Reached
    POWER_MGMT                          --- Power Management Event
    TLP_THROTTLING                      --- TLP Throttling Event
    FORCE_SPEED                         --- Force Speed Error
    CREDIT_TIMEOUT                      --- Credit Timeout
    LINK_STATE                          --- Link State Change Event
```

## event-wait
<a name="event-wait"></a>

```text
Usage: ./switchtec help./switchtec event-wait <device> [OPTIONS]

wait for an event to occur


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --event=<EVENT>, -e <EVENT> ]      --- event to wait on
  [  --partition=<NUM>, -p <NUM> ]      --- partition ID for the event
  [  --port=<NUM>, -q <NUM> ]           --- logical port ID for the event
  [  --timeout=<MS>, -t <MS> ]          --- timeout in milliseconds (-1 =
                                            forever)

Choices for EVENT:
    STACK_ERROR                         --- Stack Error
    PPU_ERROR                           --- PPU Error
    ISP_ERROR                           --- ISP Error
    SYS_RESET                           --- System Reset
    FW_EXC                              --- Firmware Exception
    FW_NMI                              --- Firmware Non-Maskable Interrupt
    FW_NON_FATAL                        --- Firmware Non-Fatal Error
    FW_FATAL                            --- Firmware Fatal Error
    TWI_MRPC_COMP                       --- TWI MRPC Completion
    TWI_MRPC_COMP_ASYNC                 --- TWI MRPC Async Completion
    CLI_MRPC_COMP                       --- CLI MRPC Completion
    CLI_MRPC_COMP_ASYNC                 --- CLI MRPC Async Completion
    GPIO_INT                            --- GPIO Interrupt
    GFMS                                --- Global Fabric Management Server
                                            Event
    PART_RESET                          --- Partition Reset
    MRPC_COMP                           --- MRPC Completion
    MRPC_COMP_ASYNC                     --- MRPC Async Completion
    DYN_PART_BIND_COMP                  --- Dynamic Partition Binding
                                            Completion
    AER_IN_P2P                          --- Advanced Error Reporting in P2P
                                            Port
    AER_IN_VEP                          --- Advanced Error Reporting in vEP
    DPC                                 --- Downstream Port Containment Event
    CTS                                 --- Completion Timeout Synthesis
                                            Event
    UEC                                 --- Upstream Error Containment Event
    HOTPLUG                             --- Hotplug Event
    IER                                 --- Internal Error Reporting Event
    THRESH                              --- Event Counter Threshold Reached
    POWER_MGMT                          --- Power Management Event
    TLP_THROTTLING                      --- TLP Throttling Event
    FORCE_SPEED                         --- Force Speed Error
    CREDIT_TIMEOUT                      --- Credit Timeout
    LINK_STATE                          --- Link State Change Event
```

## log-dump
<a name="log-dump"></a>

```text
Usage: ./switchtec help./switchtec log-dump <device> [<output_file>] [OPTIONS]

dump the firmware log to a file


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            
  [<output_file>]                       --- log output file


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --log_def=<DEF_FILE>, -d <DEF_FILE> ] --- parse log output using specified
                                            log definition file (app log
                                            only)
  [  --type=<TYPE>, -t <TYPE> ]         --- log type to dump
  [  --format=<FORMAT>, -f <FORMAT> ]   --- output log file format

Choices for TYPE:
    RAM                                 --- dump the app log from RAM
    FLASH                               --- dump the app log from flash
    FTDC                                --- dump the FTDC firmware log
    MEMLOG                              --- dump the Memlog info from flash
                                            in the last fatal error handling
                                            dump
    REGS                                --- dump the Generic Registers
                                            context from flash in the last
                                            fatal error handling dump
    THRD_STACK                          --- dump the thread stack info from
                                            flash in the last fatal error
                                            handling dump
    SYS_STACK                           --- dump the system stack info from
                                            flash in the last fatal error
                                            handling dump
    THRDS                               --- dump all thread info from flash
                                            in the last fatal error handling
                                            dump
    NVHDR                               --- dump NVLog header information in
                                            the last fatal error handling
                                            dump

Choices for FORMAT:
    BIN                                 --- output binary log data (default)
    TXT                                 --- output text log data
```

## log-parse
<a name="log-parse"></a>

```text
Usage: ./switchtec help./switchtec log-parse <log_input> <log_def> [<parsed_output>] [OPTIONS]

parse a binary app log or mailbox log to a text file


Positional Arguments:
   <log_input>                          --- binary app log input file
   <log_def>                            --- log definition file
  [<parsed_output>]                     --- parsed output file


Options:
  [  --type=<TYPE>, -t <TYPE> ]         --- log type to parse (default: APP)
  [  --device_gen=<GEN>, -g <GEN> ]     --- device generation (Only needed
                                            when parsing earlier log files
                                            which do not contain device
                                            generation information. Default:
                                            UNKNOWN)

Choices for TYPE:
    APP                                 --- app log
    MAILBOX                             --- mailbox log
    FTDC                                --- ftdc

Choices for GEN:
    GEN3                                --- GEN3
    GEN4                                --- GEN4
    GEN5                                --- GEN5
    UNKNOWN                             --- UNKNOWN
```

## test
<a name="test"></a>

```text
Usage: ./switchtec help./switchtec test <device> [OPTIONS]

test if the Switchtec interface is working


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
```

## temp
<a name="temp"></a>

```text
Usage: ./switchtec help./switchtec temp <device> [OPTIONS]

display the die temperature


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
```

## port-bind-info
<a name="port-bind-info"></a>

```text
Usage: ./switchtec help./switchtec port-bind-info <device> [OPTIONS]

display physical port binding information


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --physical=<>, -f <> ]             --- physical port ID
```

## port-bind
<a name="port-bind"></a>

```text
Usage: ./switchtec help./switchtec port-bind <device> [OPTIONS]

bind a logical port to a physical port


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --partition=<>, -p <> ]            --- partition ID
  [  --logical=<>, -l <> ]              --- logical port ID
  [  --physical=<>, -f <> ]             --- physical port ID
```

## port-unbind
<a name="port-unbind"></a>

```text
Usage: ./switchtec help./switchtec port-unbind <device> [OPTIONS]

unbind a logical port from a physical port


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --partition=<>, -p <> ]            --- partition ID
  [  --logical=<>, -l <> ]              --- logical port ID
```

## hard-reset
<a name="hard-reset"></a>

```text
Usage: ./switchtec help./switchtec hard-reset <device> [OPTIONS]

perform a hard reset of the switch


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --yes, -y ]                        --- assume yes when prompted
```

## fw-update
<a name="fw-update"></a>

```text
Usage: ./switchtec help./switchtec fw-update <device> <img_file> [OPTIONS]

upload a new firmware image to flash

This command only supports flashing firmware when the device is in the BL2
or MAIN boot phase. To transfer an image in the BL1 boot phase, use the 'mfg
fw-transfer' command instead.

NOTE - A device can be in one of these three boot phases: 
BOOTLOADER1 (BL1): in this phase, a device runs a BL1 image that resides on
the device's on-chip boot ROM. The BL1 image is implemented to facilitate
device recovery -- it supports transferring and executing a BOOTLOADER2
image. To enter the BL1 boot phase, set the device's BOOT_RECOVERY PIN 0 to
LOW and reset the device.

BOOTLOADER2 (BL2): in this phase, a device runs the BL2 image stored in
flash or transferred during the BL1 boot phase. BL2 is the phase for device
recovery -- it provides commands to update and activate device partitions.
To enter the BL2 boot phase, set the device's BOOT_RECOVERY PIN[0] to HIGH
and PIN[1] to LOW and reset the device.

MAIN FIRMWARE (MAIN): this is the full-featured firmware that runs on your
device during normal operation.




Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            
   <img_file>                           --- image file to upload


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --yes, -y ]                        --- assume yes when prompted
  [  --dont-activate, -A ]              --- don't activate the new image, use
                                            fw-toggle to do so when it is
                                            safe
  [  --force, -f ]                      --- force interrupting an existing
                                            fw-update command in case
                                            firmware is stuck in a busy
                                            state
  [  --set-boot-rw, -W ]                --- set the bootloader and map
                                            partition as RW (only valid for
                                            BOOT and MAP images)
  [  --no-progress, -p ]                --- don't print progress to stdout
```

## fw-info
<a name="fw-info"></a>

```text
Usage: ./switchtec help./switchtec fw-info <device> [OPTIONS]

return information on the currently flashed firmware


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
```

## fw-toggle
<a name="fw-toggle"></a>

```text
Usage: ./switchtec help./switchtec fw-toggle <device> [OPTIONS]

toggle the active and inactive firmware partitions


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --bl2, -b ]                        --- toggle BL2 firmware
  [  --key, -k ]                        --- toggle Key manifest
  [  --firmware, -f ]                   --- toggle IMG firmware
  [  --config, -c ]                     --- toggle CFG data
```

## fw-read
<a name="fw-read"></a>

```text
Usage: ./switchtec help./switchtec fw-read <device> [<filename>] [OPTIONS]

read a firmware image from flash


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            
  [<filename>]                          --- image file to display information
                                            for


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --yes, -y ]                        --- assume yes when prompted
  [  --inactive, -i ]                   --- read the inactive partition
  [  --data, -d ]                       --- read the data/config partiton
                                            instead of the main firmware
  [  --config, -c ]                     --- read the data/config partiton
                                            instead of the main firmware
  [  --bl2, -b ]                        --- read the BL2 partiton instead of
                                            the main firmware
  [  --key, -k ]                        --- read the key manifest partiton
                                            instead of the main firmware
  [  --no-progress, -p ]                --- don't print progress to stdout
```

## fw-img-info
<a name="fw-img-info"></a>

```text
Usage: ./switchtec help./switchtec fw-img-info <img_file>

display information for a firmware image


Positional Arguments:
   <img_file>                           --- image file to display information
                                            for
```

## evcntr
<a name="evcntr"></a>

```text
Usage: ./switchtec help./switchtec evcntr <device> [OPTIONS]

display event counters


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --reset, -r ]                      --- reset counters back to zero
  [  --stack=<NUM>, -s <NUM> ]          --- stack to show the counters for
```

## evcntr-setup
<a name="evcntr-setup"></a>

```text
Usage: ./switchtec help./switchtec evcntr-setup <device> --stack=<NUM> --event=<EVENT> [OPTIONS]

configure an event counter


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --stack=<NUM>, -s <NUM> ]          --- stack to create the counter in
  [  --event=<EVENT>, -e <EVENT> ]      --- event to count on, may specify
                                            this argument multiple times to
                                            count on multiple events
  [  --counter=<NUM>, -c <NUM> ]        --- counter index, default is to use
                                            the next unused index
  [  --egress, -g ]                     --- measure egress TLPs instead of
                                            ingress -- only meaningful for
                                            POSTED_TLP, COMP_TLP and
                                            NON_POSTED_TLP counts
  [  --port_mask=<0xXX|#,#,#-#,#>,
      -p <0xXX|#,#,#-#,#> ]             --- ports to capture events on,
                                            default is all ports
  [  --thresh=<NUM>, -t <NUM> ]         --- threshold to trigger an event
                                            notification

Choices for EVENT:
    ALL                                 --- all events
    ALL_TLPS                            --- all TLPs
    ALL_ERRORS                          --- all errors
    UNSUP_REQ_ERR                       --- Unsupported Request error
    ECRC_ERR                            --- ECRC error
    MALFORM_TLP_ERR                     --- Malformed TLP error
    RCVR_OFLOW_ERR                      --- Receiver Overflow error
    CMPLTR_ABORT_ERR                    --- Completer Abort error
    POISONED_TLP_ERR                    --- Poisoned TLP error
    SURPRISE_DOWN_ERR                   --- Surprise Down error
    DATA_LINK_PROTO_ERR                 --- Data Link Protocol error
    HDR_LOG_OFLOW_ERR                   --- Header Log Overflow error
    UNCOR_INT_ERR                       --- Uncorrectable Internal Error
    REPLAY_TMR_TIMEOUT                  --- Replay Timer Timeout
    REPLAY_NUM_ROLLOVER                 --- Replay Number Rollover
    BAD_DLLP                            --- Bad DLLP
    BAD_TLP                             --- Bad TLP
    RCVR_ERR                            --- Receiver Error
    RCV_FATAL_MSG                       --- Fatal Error Message received
    RCV_NON_FATAL_MSG                   --- Non-fatal Error Message received
    RCV_CORR_MSG                        --- Correctable Error Message
                                            received
    NAK_RCVD                            --- NAK received
    RULE_TABLE_HIT                      --- Rule Search Table rule hit
    POSTED_TLP                          --- Posted TLP
    COMP_TLP                            --- Completion TLP
    NON_POSTED_TLP                      --- Non-Posted TLP
```

## evcntr-show
<a name="evcntr-show"></a>

```text
Usage: ./switchtec help./switchtec evcntr-show <device> --stack=<NUM> --counter=<NUM> [OPTIONS]

display an event counter's configuration


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --stack=<NUM>, -s <NUM> ]          --- stack to show the configuration
                                            for
  [  --counter=<NUM>, -c <NUM> ]        --- counter index
```

## evcntr-del
<a name="evcntr-del"></a>

```text
Usage: ./switchtec help./switchtec evcntr-del <device> --stack=<NUM> --counter=<NUM> [OPTIONS]

deconfigure an event counter


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --stack=<NUM>, -s <NUM> ]          --- stack to deconfigure the counter
                                            in
  [  --counter=<NUM>, -c <NUM> ]        --- counter index
```

## evcntr-wait
<a name="evcntr-wait"></a>

```text
Usage: ./switchtec help./switchtec evcntr-wait <device> [OPTIONS]

wait for an event counter to exceed its threshold


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --timeout=<MS>, -t <MS> ]          --- timeout in milliseconds (-1 =
                                            forever)
```

## diag
<a name="diag"></a>

```text
switchtec-5.7
Usage: ./switchtec diag <command> [<device>] [OPTIONS]

The <device> must be a Switchtec device (ex: /dev/switchtec0)

Diagnostic Information

These functions provide diagnostic information from the switch

The following are all implemented sub-commands:
  w2h                            Capture w2h Score
  eye                            Capture eye data
  list-mrpc                      List permissible MRPC commands
  loopback                       Enable Loopback on specified ports
  pattern                        Enable pattern generation and monitor
  port-eq-txcoeff                Dump port equalization coefficients
  port-eq-txfslf                 Dump FS/LF output data
  port-eq-txtable                Dump far end port equalization table
  rcvr-extended                  Dump RX mode and DTCLK
  rcvr-obj                       Dump analog RX coefficients/adaptation objects
  ltssm-log                      Display LTSSM log
  version                        show the program version
  help                           display this help

See './switchtec diag help <command>' for more information on a specific command
```

### diag w2h
<a name="diag-w2h"></a>

```text
Usage: ./switchtec diag help./switchtec diag w2h <device> [OPTIONS]

Capture w2h Score


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --format=<FMT>, -f <FMT> ]         --- output format (default: curses)
  [  --lane=<LANE_ID>, -l <LANE_ID> ]   --- lane id within the port to
                                            observe
  [  --error threshold[=<ERROR THRESHOLD>],
      -t [<ERROR THRESHOLD>] ]          --- Maximum allowed errors
  [  --t-step=<NUM>, -s <NUM> ]         --- time step (default 1)
  [  --v-step=<NUM>, -S <NUM> ]         --- voltage step (default: 5)

Choices for FMT:
    curses                              --- Display data in a curses
                                            scrollable window
    text                                --- Display data in a simplified text
                                            format
    csv                                 --- Raw Data in CSV format
```

### diag eye
<a name="diag-eye"></a>

```text
Usage: ./switchtec diag help./switchtec diag eye <device> [OPTIONS]

Capture eye data


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --format=<FMT>, -f <FMT> ]         --- output format (default: curses)
  [  --lane=<LANE_ID>, -l <LANE_ID> ]   --- lane id within the port to
                                            observe
  [  --error threshold[=<ERROR THRESHOLD>],
      -t [<ERROR THRESHOLD>] ]          --- Maximum allowed errors
  [  --t-step=<NUM>, -s <NUM> ]         --- time step (default 1)
  [  --v-step=<NUM>, -S <NUM> ]         --- voltage step (default: 5)

Choices for FMT:
    curses                              --- Display data in a curses
                                            scrollable window
    text                                --- Display data in a simplified text
                                            format
    csv                                 --- Raw Data in CSV format
```

### diag list-mrpc
<a name="diag-list-mrpc"></a>

```text
Usage: ./switchtec diag help./switchtec diag list-mrpc <device> [OPTIONS]

List permissible MRPC commands


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --all, -a ]                        --- print all MRPC commands,
                                            including ones that are unknown
```

### diag loopback
<a name="diag-loopback"></a>

```text
Usage: ./switchtec diag help./switchtec diag loopback <device> [OPTIONS]

Enable Loopback on specified ports


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to set/get
                                            loopback for
  [  --disable, -d ]                    --- Disable all loopback modes
  [  --ltssm, -l ]                      --- Enable LTSSM loopback mode
  [  --rx-to-tx, -r ]                   --- Enable RX->TX loopback mode
  [  --tx-to-rx, -t ]                   --- Enable TX->RX loopback mode
  [  --speed=<GEN>, -s <GEN> ]          --- LTSSM Speed (if enabling the
                                            LTSSM loopback mode), default:
                                            GEN4

Choices for GEN:
    GEN1                                --- GEN1 LTSSM Speed
    GEN2                                --- GEN2 LTSSM Speed
    GEN3                                --- GEN3 LTSSM Speed
    GEN4                                --- GEN4 LTSSM Speed
```

### diag pattern
<a name="diag-pattern"></a>

```text
Usage: ./switchtec diag help./switchtec diag pattern <device> [OPTIONS]

Enable pattern generation and monitor


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to set/get
                                            loopback for
  [  --disable, -d ]                    --- Disable all generators and
                                            monitors
  [  --inject=<NUM>, -i <NUM> ]         --- Inject the specified number of
                                            errors into all lanes of the TX
                                            port
  [  --generate, -g ]                   --- Enable Pattern Generator on
                                            specified port
  [  --monitor, -m ]                    --- Enable Pattern Monitor on
                                            specified port
  [  --pattern=<PATTERN>, -t <PATTERN> ] --- pattern to generate or monitor
                                            for (default: PRBS31)

Choices for PATTERN:
    PRBS7                               --- PRBS 7
    PRBS11                              --- PRBS 11
    PRBS23                              --- PRBS 23
    PRBS31                              --- PRBS 31
    PRBS9                               --- PRBS 9
    PRBS15                              --- PRBS 15
```

### diag port-eq-txcoeff
<a name="diag-port-eq-txcoeff"></a>

```text
Usage: ./switchtec diag help./switchtec diag port-eq-txcoeff <device> [OPTIONS]

Dump port equalization coefficients


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --far-end, -f ]                    --- get the far-end coefficients
                                            instead of the local ones
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### diag port-eq-txfslf
<a name="diag-port-eq-txfslf"></a>

```text
Usage: ./switchtec diag help./switchtec diag port-eq-txfslf <device> [OPTIONS]

Dump FS/LF output data


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --far-end, -f ]                    --- get the far-end coefficients
                                            instead of the local ones
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### diag port-eq-txtable
<a name="diag-port-eq-txtable"></a>

```text
Usage: ./switchtec diag help./switchtec diag port-eq-txtable <device> [OPTIONS]

Dump far end port equalization table


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### diag rcvr-extended
<a name="diag-rcvr-extended"></a>

```text
Usage: ./switchtec diag help./switchtec diag rcvr-extended <device> [OPTIONS]

Dump RX mode and DTCLK


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### diag rcvr-obj
<a name="diag-rcvr-obj"></a>

```text
Usage: ./switchtec diag help./switchtec diag rcvr-obj <device> [OPTIONS]

Dump analog RX coefficients/adaptation objects


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### diag ltssm-log
<a name="diag-ltssm-log"></a>

```text
Usage: ./switchtec diag help./switchtec diag ltssm-log <device> [OPTIONS]

Display LTSSM log


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
```

## gas
<a name="gas"></a>

```text
switchtec-5.7
Usage: ./switchtec gas <command> [<device>] [OPTIONS]

The <device> must be a Switchtec device (ex: /dev/switchtec0)

Global Address Space Access (dangerous)

These functions should be used with extreme caution only if you know what you
are doing. Any register accesses through this interface are unsupported by
Microsemi unless specifically otherwise specified.

The following are all implemented sub-commands:
  dump                           dump all Global Address Space registers
  read                           read a register from the Global Address Space
  write                          write a register in the Global Address Space
  version                        show the program version
  help                           display this help

See './switchtec gas help <command>' for more information on a specific command
```

### gas dump
<a name="gas-dump"></a>

```text
Usage: ./switchtec gas help./switchtec gas dump <device> [OPTIONS]

dump all Global Address Space registers


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --count=<NUM>, -n <NUM> ]          --- number of bytes to dump (default
                                            is the entire GAS space)
  [  --text, -t ]                       --- force outputting data in text
                                            format, default is to output in
                                            text unless the output is a
                                            pipe, in which case binary is
                                            output
```

### gas read
<a name="gas-read"></a>

```text
Usage: ./switchtec gas help./switchtec gas read <device> [OPTIONS]

read a register from the Global Address Space


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --addr=<ADDR>, -a <ADDR> ]         --- address to read
  [  --bytes=<NUM>, -b <NUM> ]          --- number of bytes to read per
                                            access (default 4)
  [  --count=<NUM>, -n <NUM> ]          --- number of accesses to perform
                                            (default 1)
  [  --print=<STYLE>, -p <STYLE> ]      --- printing style

Choices for STYLE:
    hex                                 --- print in hexadecimal
    dec                                 --- print in decimal
    str                                 --- print as an ASCII string
```

### gas write
<a name="gas-write"></a>

```text
Usage: ./switchtec gas help./switchtec gas write <device> [OPTIONS]

write a register in the Global Address Space


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pax=<arg>, -x <arg> ]            --- PAX ID within a PAX fabric. Only
                                            valid on Switchtec PAX devices
  [  --addr=<ADDR>, -a <ADDR> ]         --- address to write
  [  --bytes=<NUM>, -b <NUM> ]          --- number of bytes to write (default
                                            4)
  [  --value=<VAL>, -v <VAL> ]          --- value to write
  [  --yes, -y ]                        --- assume yes when prompted
```

## mfg
<a name="mfg"></a>

```text
switchtec-5.7
Usage: ./switchtec mfg <command> [<device>] [OPTIONS]

The <device> must be a Switchtec device (ex: /dev/switchtec0)

Manufacturing Process Commands

These commands control and manage mfg settings.

The following are all implemented sub-commands:
  ping                           ping device and get current boot phase
  info                           display security settings
  secure-version-get             display chip serial number, customer ID, secure versions of Key manifest, BL2, Main FW and SUV
  mailbox                        retrieve mailbox logs
  image-list                     display active image list (BL1 only)
  image-select                   select active image index (BL1 only)
  fw-transfer                    transfer a firmware image to device (BL1 only)
  fw-execute                     execute previously transferred firmware image (BL1 only)
  boot-resume                    resume device boot process (BL1 and BL2 only)
  state-set                      set device secure state (BL1 and Main Firmware only)
  config-set                     set device security settings (BL1 and Main Firmware only)
  kmsk-entry-add                 add a KSMK entry (BL1 and Main Firmware only)
  debug-unlock                   unlock firmware debug features
  debug-lock-update              update debug feature secure unlock version
  debug-unlock-token             generate debug unlock token file for the device
  die-trace-prog                 program die trace into OTP
  chip-serial-prog               program chip serial number into OTP
  sku-set                        write SKU info settings into OTP
  sku-get                        read SKU info settings from OTP
  otp-write-prot                 write protect OTP Regions
  sjtag-mode-set                 write SJTAG mode into OTP
  sjtag-uuid-set                 write SJTAG UUID into OTP
  sjtag-key-set                  write SJTAG key into OTP
  sjtag-key-verify               verify given SJTAG key and that of written in OTP
  sjtag-key-lock                 lock SJTAG key access
  selftest-pol-set               set self test policy for cryptoblock
  otp-prog-mode-set              set bit or byte programming mode for OTP
  otp-smprg-set                  set smart programming mode for OTP
  otp-mode-status-get            get status about programming modes of OTP
  otp-cust-id-prog               program given customer id in OTP
  otp-config-read                read config data from OTP from specified address and length
  otp-config-prog                program the config data into OTP
  sjtag-unlock                   SJTAG unlock command
  otp-ini2bin                    generate bin from ini file
  version                        show the program version
  help                           display this help

See './switchtec mfg help <command>' for more information on a specific command
```

### mfg ping
<a name="mfg-ping"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg ping <device> [OPTIONS]

ping device and get current boot phase


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --extended_cmd, -x ]               --- print extended ping command
                                            information
```

### mfg info
<a name="mfg-info"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg info <device> [OPTIONS]

display security settings


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --verbose, -v ]                    --- print additional chip information
```

### mfg secure-version-get
<a name="mfg-secure-version-get"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg secure-version-get <device>

display chip serial number, customer ID, secure versions of Key manifest,
BL2, Main FW and SUV


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### mfg mailbox
<a name="mfg-mailbox"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg mailbox <device> [<filename>]

retrieve mailbox logs


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<filename>]                          --- file to log mailbox data


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### mfg image-list
<a name="mfg-image-list"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg image-list <device>

display active image list (BL1 only)


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### mfg image-select
<a name="mfg-image-select"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg image-select <device> [OPTIONS]

select active image index (BL1 only)


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --bl2=<>, -b <> ]                  --- active image index for BL2
  [  --firmware=<>, -m <> ]             --- active image index for FIRMWARE
  [  --config=<>, -c <> ]               --- active image index for CONFIG
  [  --keyman=<>, -k <> ]               --- active image index for KEY
                                            MANIFEST
```

### mfg fw-transfer
<a name="mfg-fw-transfer"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg fw-transfer <device> <img_file> [OPTIONS]

transfer a firmware image to device (BL1 only)

This command only supports transferring a firmware image when the device is
in the BL1 boot phase. Use 'fw-execute' after this command to excute the
transferred image. Note that the image is stored in device RAM and is lost
after device reboot.

To update an image in the BL2 or MAIN boot phase, use the 'fw-update'
command instead.

NOTE - A device can be in one of these three boot phases: 
BOOTLOADER1 (BL1): in this phase, a device runs a BL1 image that resides on
the device's on-chip boot ROM. The BL1 image is implemented to facilitate
device recovery -- it supports transferring and executing a BOOTLOADER2
image. To enter the BL1 boot phase, set the device's BOOT_RECOVERY PIN 0 to
LOW and reset the device.

BOOTLOADER2 (BL2): in this phase, a device runs the BL2 image stored in
flash or transferred during the BL1 boot phase. BL2 is the phase for device
recovery -- it provides commands to update and activate device partitions.
To enter the BL2 boot phase, set the device's BOOT_RECOVERY PIN[0] to HIGH
and PIN[1] to LOW and reset the device.

MAIN FIRMWARE (MAIN): this is the full-featured firmware that runs on your
device during normal operation.




Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
   <img_file>                           --- firmware image file to transfer


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --yes, -y ]                        --- assume yes when prompted
  [  --force, -f ]                      --- force interrupting an existing
                                            fw-update command in case
                                            firmware is stuck in a busy
                                            state
```

### mfg fw-execute
<a name="mfg-fw-execute"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg fw-execute <device> [OPTIONS]

execute previously transferred firmware image (BL1 only)

This command is only supported when the device is in the BL1 boot phase. The
firmware image must have been transferred using the 'fw-transfer' command.
After firmware initializes, it listens for activity from I2C, UART (XModem),
or both interfaces for input. Once activity is detected from an interface,
firmware falls into recovery mode on that interface. The interface to listen
on can be specified using the 'bl2_recovery_mode' option. 

To activate an image in the BL2 or MAIN boot phase, use the 'fw-toggle'
command instead.

NOTE - A device can be in one of these three boot phases: 
BOOTLOADER1 (BL1): in this phase, a device runs a BL1 image that resides on
the device's on-chip boot ROM. The BL1 image is implemented to facilitate
device recovery -- it supports transferring and executing a BOOTLOADER2
image. To enter the BL1 boot phase, set the device's BOOT_RECOVERY PIN 0 to
LOW and reset the device.

BOOTLOADER2 (BL2): in this phase, a device runs the BL2 image stored in
flash or transferred during the BL1 boot phase. BL2 is the phase for device
recovery -- it provides commands to update and activate device partitions.
To enter the BL2 boot phase, set the device's BOOT_RECOVERY PIN[0] to HIGH
and PIN[1] to LOW and reset the device.

MAIN FIRMWARE (MAIN): this is the full-featured firmware that runs on your
device during normal operation.




Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --yes, -y ]                        --- assume yes when prompted
  [  --bl2_recovery_mode=<MODE>,
      -m <MODE> ]                       --- BL2 recovery mode

Choices for MODE:
    I2C                                 --- I2C
    XMODEM                              --- XModem
    BOTH                                --- both I2C and XModem (default)
```

### mfg boot-resume
<a name="mfg-boot-resume"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg boot-resume <device> [OPTIONS]

resume device boot process (BL1 and BL2 only)

A normal device boot process includes the BL1, BL2 and Main Firmware boot
phases. In the case when the boot process is paused at the BL1 or BL2 phase
(due to boot failure or BOOT_RECOVERY PIN[0:1] being set to LOW), sending
this command requests the device to try resuming a normal boot
process.

NOTE: if your system does not support hotplug, your device might not be
immediately accessible after a normal boot process. In this case, be sure to
reboot your system after sending this command.


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg state-set
<a name="mfg-state-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg state-set <device> [OPTIONS]

set device secure state (BL1 and Main Firmware only)

This command can only be used when the device secure state is
UNINITIALIZED_UNSECURED.

NOTE - A device can be in one of these three secure states:

UNINITIALIZED_UNSECURED: this is the default state when the chip is shipped.
All security-related settings are 'uninitialized', and the chip is in the
'unsecured' state. 
INITIALIZED_UNSECURED: this is the state when security-related settings are
'initialized', and the chip is set to the 'unsecured' state.

INITIALIZED_SECURED: this is the state when security-related settings are
'initialized', and the chip is set to the 'secured' state. 

Use 'config-set' or other mfg commands to initialize security settings when
the chip is in the UNINITIALIZED_UNSECURED state, then use this command to
switch the chip to the INITIALIZED_SECURED or INITIALIZED_UNSECURED state.


WARNING: ONCE THE CHIP STATE IS SUCCESSFULLY SET, IT CAN NO LONGER BE
CHANGED. USE CAUTION WHEN ISSUING THIS COMMAND.


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --state=<state>, -t <state> ]      --- secure state
  [  --yes, -y ]                        --- assume yes when prompted

Choices for state:
    INITIALIZED_UNSECURED               --- unsecured state
    INITIALIZED_SECURED                 --- secured state
```

### mfg config-set
<a name="mfg-config-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg config-set <device> <setting_file> [OPTIONS]

set device security settings (BL1 and Main Firmware only)


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
   <setting_file>                       --- security setting file


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg kmsk-entry-add
<a name="mfg-kmsk-entry-add"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg kmsk-entry-add <device> [OPTIONS]

add a KSMK entry (BL1 and Main Firmware only)

KMSK stands for Key Manifest Secure Key. It is a key used to verify the Key
Manifest partition, which contains keys used to verify all other
partitions.



Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pub_key_file=<arg>, -p <arg> ]   --- public key file
  [  --signature_file=<arg>, -s <arg> ] --- signature file
  [  --kmsk_entry_file=<arg>, -k <arg> ] --- KMSK entry file
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg debug-unlock
<a name="mfg-debug-unlock"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg debug-unlock <device> [OPTIONS]

unlock firmware debug features

This command unlocks the EJTAG port, Command Line Interface (CLI), MRPC
command and Global Address Space (GAS) registers.


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pub_key=<arg>, -p <arg> ]        --- public key file
  [  --serial_number=<arg>, -n <arg> ]  --- device serial number
  [  --unlock_version=<arg>, -v <arg> ] --- unlock version
  [  --signature_file=<arg>, -s <arg> ] --- signature file
```

### mfg debug-lock-update
<a name="mfg-debug-lock-update"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg debug-lock-update <device> [OPTIONS]

update debug feature secure unlock version


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --pub_key=<arg>, -p <arg> ]        --- public key file
  [  --serial_number=<arg>, -n <arg> ]  --- device serial number
  [  --new_unlock_version=<arg>,
      -v <arg> ]                        --- new unlock version
  [  --signature_file=<arg>, -s <arg> ] --- signature file
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg debug-unlock-token
<a name="mfg-debug-unlock-token"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg debug-unlock-token <device> [<token_file>]

generate debug unlock token file for the device

Use the generated token file on your security management system to generate
the signature file required for command 'mfg debug-unlock'


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<token_file>]                        --- debug unlock token file


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### mfg die-trace-prog
<a name="mfg-die-trace-prog"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg die-trace-prog <device> [OPTIONS]

program die trace into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --die_trace=<die_trace>, -d <die_trace> ] --- Die Trace file.bin
                                            Should contain Die Trace
                                            settings
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg chip-serial-prog
<a name="mfg-chip-serial-prog"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg chip-serial-prog <device> [OPTIONS]

program chip serial number into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --chip_serial_num=<chip_serial_num>,
      -c <chip_serial_num> ]            --- Enter 4-byte Chip Serial Number
```

### mfg sku-set
<a name="mfg-sku-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sku-set <device> [OPTIONS]

write SKU info settings into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --block_num=<block_num>, -b <block_num> ] --- Block Number
                                            Valid Range : 0 - 3
                                            
  [  --sku_info=<sku_info>, -s <sku_info> ] --- Set SKU Info
                                            Valid Range : 0 - 4
```

### mfg sku-get
<a name="mfg-sku-get"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sku-get <device>

read SKU info settings from OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### mfg otp-write-prot
<a name="mfg-otp-write-prot"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-write-prot <device> [OPTIONS]

write protect OTP Regions


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --sku_info=<sku_info>, -s <sku_info> ] --- Enter 1 to Write Protect SKU Info
                                            Region
                                            
  [  --otp_config=<otp_config>,
      -c <otp_config> ]                 --- Enter 1 to Write Protect OTP
                                            Config Region
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg sjtag-mode-set
<a name="mfg-sjtag-mode-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sjtag-mode-set <device> [OPTIONS]

write SJTAG mode into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --sjtag_mode=<sjtag_mode>,
      -m <sjtag_mode> ]                 --- Specify SJTAG Mode
                                            Supported SJTAG Modes are
                                            * 0 - Open
                                            * 1 - Secure Mode 1
                                            * 2 - Secure Mode 2
                                            * 3 - Closed
```

### mfg sjtag-uuid-set
<a name="mfg-sjtag-uuid-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sjtag-uuid-set <device> [OPTIONS]

write SJTAG UUID into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --uuid_file=<arg>, -u <arg> ]      --- UUID Bin File(.bin)
                                            Should contain 10 bytes of SJTAG
                                            UUID to be set
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg sjtag-key-set
<a name="mfg-sjtag-key-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sjtag-key-set <device> [OPTIONS]

write SJTAG key into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --sjtag_key_file=<sjtag_key_file>,
      -k <sjtag_key_file> ]             --- SJTAG Key Bin File(.bin)
                                            Should contain 16 Bytes of SJTAG
                                            Key to be set
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg sjtag-key-verify
<a name="mfg-sjtag-key-verify"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sjtag-key-verify <device> [OPTIONS]

verify given SJTAG key and that of written in OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --sjtag_key_file=<sjtag_key_file>,
      -k <sjtag_key_file> ]             --- SJTAG Key Bin File(.bin)
                                            Should contain the SJTAG Key to
                                            be verified with that in OTP
```

### mfg sjtag-key-lock
<a name="mfg-sjtag-key-lock"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sjtag-key-lock <device> [OPTIONS]

lock SJTAG key access


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg selftest-pol-set
<a name="mfg-selftest-pol-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg selftest-pol-set <device> [OPTIONS]

set self test policy for cryptoblock


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --self_test_policy=<self_test_policy>,
      -s <self_test_policy> ]           --- Self Test Policy Bin
                                            File(.bin)
                                            Should contain Self Test Policy
                                            settings
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg otp-prog-mode-set
<a name="mfg-otp-prog-mode-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-prog-mode-set <device> [OPTIONS]

set bit or byte programming mode for OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --bit_or_byte_mode=<bit_or_byte_mode>,
      -m <bit_or_byte_mode> ]           --- Selects OTP bit or byte mode
                                            * 0 - Enable Bit Mode
                                            * 1 - Enable Byte Mode
```

### mfg otp-smprg-set
<a name="mfg-otp-smprg-set"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-smprg-set <device> [OPTIONS]

set smart programming mode for OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --otp_smart_prog_mode=<otp_smart_prog_mode>,
      -m <otp_smart_prog_mode> ]        --- OTP Smart Programming Mode
                                            * 0 - Disable Smart Programming
                                            Mode
                                            * 1 - Enable Smart Programming
                                            Mode
```

### mfg otp-mode-status-get
<a name="mfg-otp-mode-status-get"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-mode-status-get <device>

get status about programming modes of OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### mfg otp-cust-id-prog
<a name="mfg-otp-cust-id-prog"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-cust-id-prog <device> [OPTIONS]

program given customer id in OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --customer_id=<customer_id>,
      -c <customer_id> ]                --- Customer ID
```

### mfg otp-config-read
<a name="mfg-otp-config-read"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-config-read <device> [<bin_file>] [OPTIONS]

set smart programming mode for OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<bin_file>]                          --- OTP Config Read


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --start_address=<Config Start address>,
      -s <Config Start address> ]       --- Start Address from the OTP to
                                            read Config data
                                             Config region starts from
                                            0x1800
                                            
  [  --len=<Config Length>, -l <Config Length> ] --- Length in bytes to read Config
                                            data from OTP
```

### mfg otp-config-prog
<a name="mfg-otp-config-prog"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-config-prog <device> [OPTIONS]

program the config data into OTP


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --is_firmware_halt=<is_firmware_halt>,
      -h <is_firmware_halt> ]           --- Enter 1 to Halt the firmware in
                                            while(1)
                                            
  [  --config_data_bin=<otp_config_data_bin>,
      -f <otp_config_data_bin> ]        --- OTP Config Data Bin
                                            File(.bin)
                                            Should contain Config data to be
                                            written into OTP
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### mfg sjtag-unlock
<a name="mfg-sjtag-unlock"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg sjtag-unlock <device> [<sjtag_debug_token_file>] [OPTIONS]

read SJTAG unlock status & mode


Positional Arguments:
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<sjtag_debug_token_file>]            --- Optional Argument. If not
                                            provided, the Debug Token will
                                            be generated
                                            SJTAG Debug Token
                                            File(.bin)
                                            Generated by the HSM Server
                                            


Environment Variables:
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


Options:
  [  --verbose, -v ]                    --- print additional sjtag-unlock
                                            information
  [  --force_hr, -f ]                   --- send HR for all SJTAG mode
```

### mfg otp-ini2bin
<a name="mfg-otp-ini2bin"></a>

```text
Usage: ./switchtec mfg help./switchtec mfg otp-ini2bin [<bin_file>] [OPTIONS]

generate bin from ini file


Positional Arguments:
  [<bin_file>]                          --- OTP Bin file


Options:
  [  --ini_file=<arg>, -i <arg> ]       --- ini file as an input
```
