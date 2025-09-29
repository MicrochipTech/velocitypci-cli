# Switchtec CLI Command Reference

## Table of Contents

- [switchtec help](#switchtec-help)
- [switchtec version](#switchtec-version)
- [./switchtec list](#switchtec-list)
- [./switchtec info](#switchtec-info)
- [./switchtec gui](#switchtec-gui)
- [./switchtec status](#switchtec-status)
- [./switchtec bw](#switchtec-bw)
- [./switchtec latency](#switchtec-latency)
- [./switchtec events](#switchtec-events)
- [./switchtec event-wait](#switchtec-event-wait)
- [./switchtec log-dump](#switchtec-log-dump)
- [./switchtec log-parse](#switchtec-log-parse)
- [./switchtec test](#switchtec-test)
- [./switchtec temp](#switchtec-temp)
- [./switchtec port-bind-info](#switchtec-port-bind-info)
- [./switchtec port-bind](#switchtec-port-bind)
- [./switchtec port-unbind](#switchtec-port-unbind)
- [./switchtec hard-reset](#switchtec-hard-reset)
- [./switchtec fw-update](#switchtec-fw-update)
- [./switchtec fw-info](#switchtec-fw-info)
- [./switchtec fw-toggle](#switchtec-fw-toggle)
- [./switchtec fw-read](#switchtec-fw-read)
- [./switchtec fw-img-info](#switchtec-fw-img-info)
- [./switchtec evcntr](#switchtec-evcntr)
- [./switchtec evcntr-setup](#switchtec-evcntr-setup)
- [./switchtec evcntr-show](#switchtec-evcntr-show)
- [./switchtec evcntr-del](#switchtec-evcntr-del)
- [./switchtec evcntr-wait](#switchtec-evcntr-wait)
- [./switchtec diag](#switchtec-diag)
  - [./switchtec diag help w2h](#switchtec-diag-help-w2h)
  - [./switchtec diag help eye](#switchtec-diag-help-eye)
  - [./switchtec diag help list-mrpc](#switchtec-diag-help-list-mrpc)
  - [./switchtec diag help loopback](#switchtec-diag-help-loopback)
  - [./switchtec diag help pattern](#switchtec-diag-help-pattern)
  - [./switchtec diag help port-eq-txcoeff](#switchtec-diag-help-port-eq-txcoeff)
  - [./switchtec diag help port-eq-txfslf](#switchtec-diag-help-port-eq-txfslf)
  - [./switchtec diag help port-eq-txtable](#switchtec-diag-help-port-eq-txtable)
  - [./switchtec diag help rcvr-extended](#switchtec-diag-help-rcvr-extended)
  - [./switchtec diag help rcvr-obj](#switchtec-diag-help-rcvr-obj)
  - [./switchtec diag help ltssm-log](#switchtec-diag-help-ltssm-log)
- [./switchtec gas](#switchtec-gas)
  - [./switchtec gas help dump](#switchtec-gas-help-dump)
  - [./switchtec gas help read](#switchtec-gas-help-read)
  - [./switchtec gas help write](#switchtec-gas-help-write)
- [./switchtec mfg](#switchtec-mfg)
  - [./switchtec mfg help ping](#switchtec-mfg-help-ping)
  - [./switchtec mfg help info](#switchtec-mfg-help-info)
  - [./switchtec mfg help secure-version-get](#switchtec-mfg-help-secure-version-get)
  - [./switchtec mfg help mailbox](#switchtec-mfg-help-mailbox)
  - [./switchtec mfg help image-list](#switchtec-mfg-help-image-list)
  - [./switchtec mfg help image-select](#switchtec-mfg-help-image-select)
  - [./switchtec mfg help fw-transfer](#switchtec-mfg-help-fw-transfer)
  - [./switchtec mfg help fw-execute](#switchtec-mfg-help-fw-execute)
  - [./switchtec mfg help boot-resume](#switchtec-mfg-help-boot-resume)
  - [./switchtec mfg help state-set](#switchtec-mfg-help-state-set)
  - [./switchtec mfg help config-set](#switchtec-mfg-help-config-set)
  - [./switchtec mfg help kmsk-entry-add](#switchtec-mfg-help-kmsk-entry-add)
  - [./switchtec mfg help debug-unlock](#switchtec-mfg-help-debug-unlock)
  - [./switchtec mfg help debug-lock-update](#switchtec-mfg-help-debug-lock-update)
  - [./switchtec mfg help debug-unlock-token](#switchtec-mfg-help-debug-unlock-token)
  - [./switchtec mfg help die-trace-prog](#switchtec-mfg-help-die-trace-prog)
  - [./switchtec mfg help chip-serial-prog](#switchtec-mfg-help-chip-serial-prog)
  - [./switchtec mfg help sku-set](#switchtec-mfg-help-sku-set)
  - [./switchtec mfg help sku-get](#switchtec-mfg-help-sku-get)
  - [./switchtec mfg help otp-write-prot](#switchtec-mfg-help-otp-write-prot)
  - [./switchtec mfg help sjtag-mode-set](#switchtec-mfg-help-sjtag-mode-set)
  - [./switchtec mfg help sjtag-uuid-set](#switchtec-mfg-help-sjtag-uuid-set)
  - [./switchtec mfg help sjtag-key-set](#switchtec-mfg-help-sjtag-key-set)
  - [./switchtec mfg help sjtag-key-verify](#switchtec-mfg-help-sjtag-key-verify)
  - [./switchtec mfg help sjtag-key-lock](#switchtec-mfg-help-sjtag-key-lock)
  - [./switchtec mfg help selftest-pol-set](#switchtec-mfg-help-selftest-pol-set)
  - [./switchtec mfg help otp-prog-mode-set](#switchtec-mfg-help-otp-prog-mode-set)
  - [./switchtec mfg help otp-smprg-set](#switchtec-mfg-help-otp-smprg-set)
  - [./switchtec mfg help otp-mode-status-get](#switchtec-mfg-help-otp-mode-status-get)
  - [./switchtec mfg help otp-cust-id-prog](#switchtec-mfg-help-otp-cust-id-prog)
  - [./switchtec mfg help otp-config-read](#switchtec-mfg-help-otp-config-read)
  - [./switchtec mfg help otp-config-prog](#switchtec-mfg-help-otp-config-prog)
  - [./switchtec mfg help sjtag-unlock](#switchtec-mfg-help-sjtag-unlock)
  - [./switchtec mfg help otp-ini2bin](#switchtec-mfg-help-otp-ini2bin)

## switchtec help
<a name="switchtec-help"></a>

```text
switchtec-5.7
[1mUsage:[0m ./switchtec <command> [<device>] [OPTIONS]

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

## switchtec version
<a name="switchtec-version"></a>

```text
switchtec cli version 5.7
```

## ./switchtec list
<a name="switchtec-list"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec list [OPTIONS]

list all Switchtec devices on this machine


[1mOptions:[0m
  [  --verbose, -v ]                    --- print additional device
                                            information
```

## ./switchtec info
<a name="switchtec-info"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec info <device>

display switch information


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

## ./switchtec gui
<a name="switchtec-gui"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec gui <device> [OPTIONS]

display a simple ncurses GUI


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --all_ports, -a ]                  --- show all ports (including downed
                                            links)
  [  --reset, -r ]                      --- reset byte counters
  [  --refresh=<>, -f <> ]              --- GUI refresh period in seconds
                                            (default: 1 second)
  [  --duration=<>, -d <> ]             --- GUI duration in seconds (-1 =
                                            forever)
  [  --bw_type=<TYPE>, -b <TYPE> ]      --- GUI bandwidth type

[1mChoices for TYPE:[0m
    RAW                                 --- get the raw bandwidth
    PAYLOAD                             --- get the payload bandwidth
```

## ./switchtec status
<a name="switchtec-status"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec status <device> [OPTIONS]

display switch port status information


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --format=<FMT>, -f <FMT> ]         --- output format (default: normal)
  [  --reset, -r ]                      --- reset byte counters
  [  --verbose, -v ]                    --- print additional information
                                            (only with 'normal' format)

[1mChoices for FMT:[0m
    normal                              --- Human Readable Output
    table                               --- Tabular Output
```

## ./switchtec bw
<a name="switchtec-bw"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec bw <device> [OPTIONS]

measure the traffic bandwidth through each port


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --time=<NUM>, -t <NUM> ]           --- measurement time in seconds
  [  --verbose, -v ]                    --- print posted, non-posted and
                                            completion results
  [  --bw_type=<TYPE>, -b <TYPE> ]      --- bandwidth type

[1mChoices for TYPE:[0m
    RAW                                 --- get the raw bandwidth
    PAYLOAD                             --- get the payload bandwidth
```

## ./switchtec latency
<a name="switchtec-latency"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec latency <device> --egress=<NUM> [OPTIONS]

measure the latency of a port


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --time=<NUM>, -t <NUM> ]           --- measurement time in seconds
  [  --egress=<NUM>, -e <NUM> ]         --- physical port ID for the egress
                                            side
  [  --ingress=<NUM>, -i <NUM> ]        --- physical port ID for the ingress
                                            side (default: use all ports)
```

## ./switchtec events
<a name="switchtec-events"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec events <device> [OPTIONS]

display events that have occurred


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --all, -a ]                        --- show events in all partitions
  [  --reset, -r ]                      --- clear all events
  [  --event=<EVENT>, -e <EVENT> ]      --- clear all events of a specified
                                            type

[1mChoices for EVENT:[0m
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

## ./switchtec event-wait
<a name="switchtec-event-wait"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec event-wait <device> [OPTIONS]

wait for an event to occur


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --event=<EVENT>, -e <EVENT> ]      --- event to wait on
  [  --partition=<NUM>, -p <NUM> ]      --- partition ID for the event
  [  --port=<NUM>, -q <NUM> ]           --- logical port ID for the event
  [  --timeout=<MS>, -t <MS> ]          --- timeout in milliseconds (-1 =
                                            forever)

[1mChoices for EVENT:[0m
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

## ./switchtec log-dump
<a name="switchtec-log-dump"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec log-dump <device> [<output_file>] [OPTIONS]

dump the firmware log to a file


[1mPositional Arguments:[0m
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


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --log_def=<DEF_FILE>, -d <DEF_FILE> ] --- parse log output using specified
                                            log definition file (app log
                                            only)
  [  --type=<TYPE>, -t <TYPE> ]         --- log type to dump
  [  --format=<FORMAT>, -f <FORMAT> ]   --- output log file format

[1mChoices for TYPE:[0m
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

[1mChoices for FORMAT:[0m
    BIN                                 --- output binary log data (default)
    TXT                                 --- output text log data
```

## ./switchtec log-parse
<a name="switchtec-log-parse"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec log-parse <log_input> <log_def> [<parsed_output>] [OPTIONS]

parse a binary app log or mailbox log to a text file


[1mPositional Arguments:[0m
   <log_input>                          --- binary app log input file
   <log_def>                            --- log definition file
  [<parsed_output>]                     --- parsed output file


[1mOptions:[0m
  [  --type=<TYPE>, -t <TYPE> ]         --- log type to parse (default: APP)
  [  --device_gen=<GEN>, -g <GEN> ]     --- device generation (Only needed
                                            when parsing earlier log files
                                            which do not contain device
                                            generation information. Default:
                                            UNKNOWN)

[1mChoices for TYPE:[0m
    APP                                 --- app log
    MAILBOX                             --- mailbox log
    FTDC                                --- ftdc

[1mChoices for GEN:[0m
    GEN3                                --- GEN3
    GEN4                                --- GEN4
    GEN5                                --- GEN5
    UNKNOWN                             --- UNKNOWN
```

## ./switchtec test
<a name="switchtec-test"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec test <device>

test if the Switchtec interface is working


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

## ./switchtec temp
<a name="switchtec-temp"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec temp <device>

display the die temperature


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

## ./switchtec port-bind-info
<a name="switchtec-port-bind-info"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec port-bind-info <device> [OPTIONS]

display physical port binding information


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --physical=<>, -f <> ]             --- physical port ID
```

## ./switchtec port-bind
<a name="switchtec-port-bind"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec port-bind <device> [OPTIONS]

bind a logical port to a physical port


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --partition=<>, -p <> ]            --- partition ID
  [  --logical=<>, -l <> ]              --- logical port ID
  [  --physical=<>, -f <> ]             --- physical port ID
```

## ./switchtec port-unbind
<a name="switchtec-port-unbind"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec port-unbind <device> [OPTIONS]

unbind a logical port from a physical port


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --partition=<>, -p <> ]            --- partition ID
  [  --logical=<>, -l <> ]              --- logical port ID
```

## ./switchtec hard-reset
<a name="switchtec-hard-reset"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec hard-reset <device> [OPTIONS]

perform a hard reset of the switch


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --yes, -y ]                        --- assume yes when prompted
```

## ./switchtec fw-update
<a name="switchtec-fw-update"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec fw-update <device> <img_file> [OPTIONS]

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




[1mPositional Arguments:[0m
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


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
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

## ./switchtec fw-info
<a name="switchtec-fw-info"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec fw-info <device>

return information on the currently flashed firmware


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

## ./switchtec fw-toggle
<a name="switchtec-fw-toggle"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec fw-toggle <device> [OPTIONS]

toggle the active and inactive firmware partitions


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --bl2, -b ]                        --- toggle BL2 firmware
  [  --key, -k ]                        --- toggle Key manifest
  [  --firmware, -f ]                   --- toggle IMG firmware
  [  --config, -c ]                     --- toggle CFG data
```

## ./switchtec fw-read
<a name="switchtec-fw-read"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec fw-read <device> [<filename>] [OPTIONS]

read a firmware image from flash


[1mPositional Arguments:[0m
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


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
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

## ./switchtec fw-img-info
<a name="switchtec-fw-img-info"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec fw-img-info <img_file>

display information for a firmware image


[1mPositional Arguments:[0m
   <img_file>                           --- image file to display information
                                            for
```

## ./switchtec evcntr
<a name="switchtec-evcntr"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec evcntr <device> [OPTIONS]

display event counters


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --reset, -r ]                      --- reset counters back to zero
  [  --stack=<NUM>, -s <NUM> ]          --- stack to show the counters for
```

## ./switchtec evcntr-setup
<a name="switchtec-evcntr-setup"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec evcntr-setup <device> --stack=<NUM> --event=<EVENT> [OPTIONS]

configure an event counter


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
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

[1mChoices for EVENT:[0m
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

## ./switchtec evcntr-show
<a name="switchtec-evcntr-show"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec evcntr-show <device> --stack=<NUM> --counter=<NUM> [OPTIONS]

display an event counter's configuration


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --stack=<NUM>, -s <NUM> ]          --- stack to show the configuration
                                            for
  [  --counter=<NUM>, -c <NUM> ]        --- counter index
```

## ./switchtec evcntr-del
<a name="switchtec-evcntr-del"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec evcntr-del <device> --stack=<NUM> --counter=<NUM> [OPTIONS]

deconfigure an event counter


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --stack=<NUM>, -s <NUM> ]          --- stack to deconfigure the counter
                                            in
  [  --counter=<NUM>, -c <NUM> ]        --- counter index
```

## ./switchtec evcntr-wait
<a name="switchtec-evcntr-wait"></a>

```text
[1mUsage:[0m ./switchtec help./switchtec evcntr-wait <device> [OPTIONS]

wait for an event counter to exceed its threshold


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --timeout=<MS>, -t <MS> ]          --- timeout in milliseconds (-1 =
                                            forever)
```

## ./switchtec diag
<a name="switchtec-diag"></a>

```text
switchtec-5.7
[1mUsage:[0m ./switchtec diag <command> [<device>] [OPTIONS]

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

### ./switchtec diag help w2h
<a name="switchtec-diag-help-w2h"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag w2h <device> [OPTIONS]

Capture w2h Score


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --format=<FMT>, -f <FMT> ]         --- output format (default: curses)
  [  --lane=<LANE_ID>, -l <LANE_ID> ]   --- lane id within the port to
                                            observe
  [  --error threshold[=<ERROR THRESHOLD>],
      -t [<ERROR THRESHOLD>] ]          --- Maximum allowed errors
  [  --t-step=<NUM>, -s <NUM> ]         --- time step (default 1)
  [  --v-step=<NUM>, -S <NUM> ]         --- voltage step (default: 5)

[1mChoices for FMT:[0m
    curses                              --- Display data in a curses
                                            scrollable window
    text                                --- Display data in a simplified text
                                            format
    csv                                 --- Raw Data in CSV format
```

### ./switchtec diag help eye
<a name="switchtec-diag-help-eye"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag eye <device> [OPTIONS]

Capture eye data


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --format=<FMT>, -f <FMT> ]         --- output format (default: curses)
  [  --lane=<LANE_ID>, -l <LANE_ID> ]   --- lane id within the port to
                                            observe
  [  --error threshold[=<ERROR THRESHOLD>],
      -t [<ERROR THRESHOLD>] ]          --- Maximum allowed errors
  [  --t-step=<NUM>, -s <NUM> ]         --- time step (default 1)
  [  --v-step=<NUM>, -S <NUM> ]         --- voltage step (default: 5)

[1mChoices for FMT:[0m
    curses                              --- Display data in a curses
                                            scrollable window
    text                                --- Display data in a simplified text
                                            format
    csv                                 --- Raw Data in CSV format
```

### ./switchtec diag help list-mrpc
<a name="switchtec-diag-help-list-mrpc"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag list-mrpc <device> [OPTIONS]

List permissible MRPC commands


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --all, -a ]                        --- print all MRPC commands,
                                            including ones that are unknown
```

### ./switchtec diag help loopback
<a name="switchtec-diag-help-loopback"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag loopback <device> [OPTIONS]

Enable Loopback on specified ports


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to set/get
                                            loopback for
  [  --disable, -d ]                    --- Disable all loopback modes
  [  --ltssm, -l ]                      --- Enable LTSSM loopback mode
  [  --rx-to-tx, -r ]                   --- Enable RX->TX loopback mode
  [  --tx-to-rx, -t ]                   --- Enable TX->RX loopback mode
  [  --speed=<GEN>, -s <GEN> ]          --- LTSSM Speed (if enabling the
                                            LTSSM loopback mode), default:
                                            GEN4

[1mChoices for GEN:[0m
    GEN1                                --- GEN1 LTSSM Speed
    GEN2                                --- GEN2 LTSSM Speed
    GEN3                                --- GEN3 LTSSM Speed
    GEN4                                --- GEN4 LTSSM Speed
```

### ./switchtec diag help pattern
<a name="switchtec-diag-help-pattern"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag pattern <device> [OPTIONS]

Enable pattern generation and monitor


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
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

[1mChoices for PATTERN:[0m
    PRBS7                               --- PRBS 7
    PRBS11                              --- PRBS 11
    PRBS23                              --- PRBS 23
    PRBS31                              --- PRBS 31
    PRBS9                               --- PRBS 9
    PRBS15                              --- PRBS 15
```

### ./switchtec diag help port-eq-txcoeff
<a name="switchtec-diag-help-port-eq-txcoeff"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag port-eq-txcoeff <device> [OPTIONS]

Dump port equalization coefficients


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --far-end, -f ]                    --- get the far-end coefficients
                                            instead of the local ones
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### ./switchtec diag help port-eq-txfslf
<a name="switchtec-diag-help-port-eq-txfslf"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag port-eq-txfslf <device> [OPTIONS]

Dump FS/LF output data


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --far-end, -f ]                    --- get the far-end coefficients
                                            instead of the local ones
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### ./switchtec diag help port-eq-txtable
<a name="switchtec-diag-help-port-eq-txtable"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag port-eq-txtable <device> [OPTIONS]

Dump far end port equalization table


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### ./switchtec diag help rcvr-extended
<a name="switchtec-diag-help-rcvr-extended"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag rcvr-extended <device> [OPTIONS]

Dump RX mode and DTCLK


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### ./switchtec diag help rcvr-obj
<a name="switchtec-diag-help-rcvr-obj"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag rcvr-obj <device> [OPTIONS]

Dump analog RX coefficients/adaptation objects


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
  [  --prev, -P ]                       --- return the data for the previous
                                            link
```

### ./switchtec diag help ltssm-log
<a name="switchtec-diag-help-ltssm-log"></a>

```text
[1mUsage:[0m ./switchtec diag help./switchtec diag ltssm-log <device> [OPTIONS]

Display LTSSM log


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --port=<PORT_ID>, -p <PORT_ID> ]   --- physical port ID to dump data for
```

## ./switchtec gas
<a name="switchtec-gas"></a>

```text
switchtec-5.7
[1mUsage:[0m ./switchtec gas <command> [<device>] [OPTIONS]

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

### ./switchtec gas help dump
<a name="switchtec-gas-help-dump"></a>

```text
[1mUsage:[0m ./switchtec gas help./switchtec gas dump <device> [OPTIONS]

dump all Global Address Space registers


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --count=<NUM>, -n <NUM> ]          --- number of bytes to dump (default
                                            is the entire GAS space)
  [  --text, -t ]                       --- force outputting data in text
                                            format, default is to output in
                                            text unless the output is a
                                            pipe, in which case binary is
                                            output
```

### ./switchtec gas help read
<a name="switchtec-gas-help-read"></a>

```text
[1mUsage:[0m ./switchtec gas help./switchtec gas read <device> [OPTIONS]

read a register from the Global Address Space


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --addr=<ADDR>, -a <ADDR> ]         --- address to read
  [  --bytes=<NUM>, -b <NUM> ]          --- number of bytes to read per
                                            access (default 4)
  [  --count=<NUM>, -n <NUM> ]          --- number of accesses to perform
                                            (default 1)
  [  --print=<STYLE>, -p <STYLE> ]      --- printing style

[1mChoices for STYLE:[0m
    hex                                 --- print in hexadecimal
    dec                                 --- print in decimal
    str                                 --- print as an ASCII string
```

### ./switchtec gas help write
<a name="switchtec-gas-help-write"></a>

```text
[1mUsage:[0m ./switchtec gas help./switchtec gas write <device> [OPTIONS]

write a register in the Global Address Space


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                             * a UART path (/dev/ttyUSB0)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --addr=<ADDR>, -a <ADDR> ]         --- address to write
  [  --bytes=<NUM>, -b <NUM> ]          --- number of bytes to write (default
                                            4)
  [  --value=<VAL>, -v <VAL> ]          --- value to write
  [  --yes, -y ]                        --- assume yes when prompted
```

## ./switchtec mfg
<a name="switchtec-mfg"></a>

```text
switchtec-5.7
[1mUsage:[0m ./switchtec mfg <command> [<device>] [OPTIONS]

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

### ./switchtec mfg help ping
<a name="switchtec-mfg-help-ping"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg ping <device> [OPTIONS]

ping device and get current boot phase


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --extended_cmd, -x ]               --- print extended ping command
                                            information
```

### ./switchtec mfg help info
<a name="switchtec-mfg-help-info"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg info <device> [OPTIONS]

display security settings


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --verbose, -v ]                    --- print additional chip information
```

### ./switchtec mfg help secure-version-get
<a name="switchtec-mfg-help-secure-version-get"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg secure-version-get <device>

display chip serial number, customer ID, secure versions of Key manifest,
BL2, Main FW and SUV


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### ./switchtec mfg help mailbox
<a name="switchtec-mfg-help-mailbox"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg mailbox <device> [<filename>]

retrieve mailbox logs


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<filename>]                          --- file to log mailbox data


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### ./switchtec mfg help image-list
<a name="switchtec-mfg-help-image-list"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg image-list <device>

display active image list (BL1 only)


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### ./switchtec mfg help image-select
<a name="switchtec-mfg-help-image-select"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg image-select <device> [OPTIONS]

select active image index (BL1 only)


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --bl2=<>, -b <> ]                  --- active image index for BL2
  [  --firmware=<>, -m <> ]             --- active image index for FIRMWARE
  [  --config=<>, -c <> ]               --- active image index for CONFIG
  [  --keyman=<>, -k <> ]               --- active image index for KEY
                                            MANIFEST
```

### ./switchtec mfg help fw-transfer
<a name="switchtec-mfg-help-fw-transfer"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg fw-transfer <device> <img_file> [OPTIONS]

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




[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
   <img_file>                           --- firmware image file to transfer


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --yes, -y ]                        --- assume yes when prompted
  [  --force, -f ]                      --- force interrupting an existing
                                            fw-update command in case
                                            firmware is stuck in a busy
                                            state
```

### ./switchtec mfg help fw-execute
<a name="switchtec-mfg-help-fw-execute"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg fw-execute <device> [OPTIONS]

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




[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --yes, -y ]                        --- assume yes when prompted
  [  --bl2_recovery_mode=<MODE>,
      -m <MODE> ]                       --- BL2 recovery mode

[1mChoices for MODE:[0m
    I2C                                 --- I2C
    XMODEM                              --- XModem
    BOTH                                --- both I2C and XModem (default)
```

### ./switchtec mfg help boot-resume
<a name="switchtec-mfg-help-boot-resume"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg boot-resume <device> [OPTIONS]

resume device boot process (BL1 and BL2 only)

A normal device boot process includes the BL1, BL2 and Main Firmware boot
phases. In the case when the boot process is paused at the BL1 or BL2 phase
(due to boot failure or BOOT_RECOVERY PIN[0:1] being set to LOW), sending
this command requests the device to try resuming a normal boot
process.

NOTE: if your system does not support hotplug, your device might not be
immediately accessible after a normal boot process. In this case, be sure to
reboot your system after sending this command.


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help state-set
<a name="switchtec-mfg-help-state-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg state-set <device> [OPTIONS]

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


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --state=<state>, -t <state> ]      --- secure state
  [  --yes, -y ]                        --- assume yes when prompted

[1mChoices for state:[0m
    INITIALIZED_UNSECURED               --- unsecured state
    INITIALIZED_SECURED                 --- secured state
```

### ./switchtec mfg help config-set
<a name="switchtec-mfg-help-config-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg config-set <device> <setting_file> [OPTIONS]

set device security settings (BL1 and Main Firmware only)


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
   <setting_file>                       --- security setting file


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help kmsk-entry-add
<a name="switchtec-mfg-help-kmsk-entry-add"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg kmsk-entry-add <device> [OPTIONS]

add a KSMK entry (BL1 and Main Firmware only)

KMSK stands for Key Manifest Secure Key. It is a key used to verify the Key
Manifest partition, which contains keys used to verify all other
partitions.



[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --pub_key_file=<arg>, -p <arg> ]   --- public key file
  [  --signature_file=<arg>, -s <arg> ] --- signature file
  [  --kmsk_entry_file=<arg>, -k <arg> ] --- KMSK entry file
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help debug-unlock
<a name="switchtec-mfg-help-debug-unlock"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg debug-unlock <device> [OPTIONS]

unlock firmware debug features

This command unlocks the EJTAG port, Command Line Interface (CLI), MRPC
command and Global Address Space (GAS) registers.


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --pub_key=<arg>, -p <arg> ]        --- public key file
  [  --serial_number=<arg>, -n <arg> ]  --- device serial number
  [  --unlock_version=<arg>, -v <arg> ] --- unlock version
  [  --signature_file=<arg>, -s <arg> ] --- signature file
```

### ./switchtec mfg help debug-lock-update
<a name="switchtec-mfg-help-debug-lock-update"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg debug-lock-update <device> [OPTIONS]

update debug feature secure unlock version


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --pub_key=<arg>, -p <arg> ]        --- public key file
  [  --serial_number=<arg>, -n <arg> ]  --- device serial number
  [  --new_unlock_version=<arg>,
      -v <arg> ]                        --- new unlock version
  [  --signature_file=<arg>, -s <arg> ] --- signature file
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help debug-unlock-token
<a name="switchtec-mfg-help-debug-unlock-token"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg debug-unlock-token <device> [<token_file>]

generate debug unlock token file for the device

Use the generated token file on your security management system to generate
the signature file required for command 'mfg debug-unlock'


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<token_file>]                        --- debug unlock token file


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### ./switchtec mfg help die-trace-prog
<a name="switchtec-mfg-help-die-trace-prog"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg die-trace-prog <device> [OPTIONS]

program die trace into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --die_trace=<die_trace>, -d <die_trace> ] --- Die Trace file.bin
                                            Should contain Die Trace
                                            settings
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help chip-serial-prog
<a name="switchtec-mfg-help-chip-serial-prog"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg chip-serial-prog <device> [OPTIONS]

program chip serial number into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --chip_serial_num=<chip_serial_num>,
      -c <chip_serial_num> ]            --- Enter 4-byte Chip Serial Number
```

### ./switchtec mfg help sku-set
<a name="switchtec-mfg-help-sku-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sku-set <device> [OPTIONS]

write SKU info settings into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --block_num=<block_num>, -b <block_num> ] --- Block Number
                                            Valid Range : 0 - 3
                                            
  [  --sku_info=<sku_info>, -s <sku_info> ] --- Set SKU Info
                                            Valid Range : 0 - 4
```

### ./switchtec mfg help sku-get
<a name="switchtec-mfg-help-sku-get"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sku-get <device>

read SKU info settings from OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### ./switchtec mfg help otp-write-prot
<a name="switchtec-mfg-help-otp-write-prot"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-write-prot <device> [OPTIONS]

write protect OTP Regions


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --sku_info=<sku_info>, -s <sku_info> ] --- Enter 1 to Write Protect SKU Info
                                            Region
                                            
  [  --otp_config=<otp_config>,
      -c <otp_config> ]                 --- Enter 1 to Write Protect OTP
                                            Config Region
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help sjtag-mode-set
<a name="switchtec-mfg-help-sjtag-mode-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sjtag-mode-set <device> [OPTIONS]

write SJTAG mode into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --sjtag_mode=<sjtag_mode>,
      -m <sjtag_mode> ]                 --- Specify SJTAG Mode
                                            Supported SJTAG Modes are
                                            * 0 - Open
                                            * 1 - Secure Mode 1
                                            * 2 - Secure Mode 2
                                            * 3 - Closed
```

### ./switchtec mfg help sjtag-uuid-set
<a name="switchtec-mfg-help-sjtag-uuid-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sjtag-uuid-set <device> [OPTIONS]

write SJTAG UUID into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --uuid_file=<arg>, -u <arg> ]      --- UUID Bin File(.bin)
                                            Should contain 10 bytes of SJTAG
                                            UUID to be set
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help sjtag-key-set
<a name="switchtec-mfg-help-sjtag-key-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sjtag-key-set <device> [OPTIONS]

write SJTAG key into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --sjtag_key_file=<sjtag_key_file>,
      -k <sjtag_key_file> ]             --- SJTAG Key Bin File(.bin)
                                            Should contain 16 Bytes of SJTAG
                                            Key to be set
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help sjtag-key-verify
<a name="switchtec-mfg-help-sjtag-key-verify"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sjtag-key-verify <device> [OPTIONS]

verify given SJTAG key and that of written in OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --sjtag_key_file=<sjtag_key_file>,
      -k <sjtag_key_file> ]             --- SJTAG Key Bin File(.bin)
                                            Should contain the SJTAG Key to
                                            be verified with that in OTP
```

### ./switchtec mfg help sjtag-key-lock
<a name="switchtec-mfg-help-sjtag-key-lock"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sjtag-key-lock <device> [OPTIONS]

lock SJTAG key access


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help selftest-pol-set
<a name="switchtec-mfg-help-selftest-pol-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg selftest-pol-set <device> [OPTIONS]

set self test policy for cryptoblock


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --self_test_policy=<self_test_policy>,
      -s <self_test_policy> ]           --- Self Test Policy Bin
                                            File(.bin)
                                            Should contain Self Test Policy
                                            settings
                                            
  [  --yes, -y ]                        --- assume yes when prompted
```

### ./switchtec mfg help otp-prog-mode-set
<a name="switchtec-mfg-help-otp-prog-mode-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-prog-mode-set <device> [OPTIONS]

set bit or byte programming mode for OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --bit_or_byte_mode=<bit_or_byte_mode>,
      -m <bit_or_byte_mode> ]           --- Selects OTP bit or byte mode
                                            * 0 - Enable Bit Mode
                                            * 1 - Enable Byte Mode
```

### ./switchtec mfg help otp-smprg-set
<a name="switchtec-mfg-help-otp-smprg-set"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-smprg-set <device> [OPTIONS]

set smart programming mode for OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --otp_smart_prog_mode=<otp_smart_prog_mode>,
      -m <otp_smart_prog_mode> ]        --- OTP Smart Programming Mode
                                            * 0 - Disable Smart Programming
                                            Mode
                                            * 1 - Enable Smart Programming
                                            Mode
```

### ./switchtec mfg help otp-mode-status-get
<a name="switchtec-mfg-help-otp-mode-status-get"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-mode-status-get <device>

get status about programming modes of OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument
```

### ./switchtec mfg help otp-cust-id-prog
<a name="switchtec-mfg-help-otp-cust-id-prog"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-cust-id-prog <device> [OPTIONS]

program given customer id in OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --customer_id=<customer_id>,
      -c <customer_id> ]                --- Customer ID
```

### ./switchtec mfg help otp-config-read
<a name="switchtec-mfg-help-otp-config-read"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-config-read <device> [<bin_file>] [OPTIONS]

set smart programming mode for OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            
  [<bin_file>]                          --- OTP Config Read


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --start_address=<Config Start address>,
      -s <Config Start address> ]       --- Start Address from the OTP to
                                            read Config data
                                             Config region starts from
                                            0x1800
                                            
  [  --len=<Config Length>, -l <Config Length> ] --- Length in bytes to read Config
                                            data from OTP
```

### ./switchtec mfg help otp-config-prog
<a name="switchtec-mfg-help-otp-config-prog"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-config-prog <device> [OPTIONS]

program the config data into OTP


[1mPositional Arguments:[0m
   <device>                             --- Switchtec device to operate on.
                                            Can be any of:
                                             * a device path
                                            (/dev/switchtec0)
                                             * an index (0, 1, 2)
                                             * a PCI address (3:00.1)
                                             * an I2C path with slave
                                            address (/dev/i2c-1@0x20)
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
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

### ./switchtec mfg help sjtag-unlock
<a name="switchtec-mfg-help-sjtag-unlock"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg sjtag-unlock <device> [<sjtag_debug_token_file>] [OPTIONS]

read SJTAG unlock status & mode


[1mPositional Arguments:[0m
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
                                            


[1mEnvironment Variables:[0m
    SWITCHTEC_DEV                       --- if set, the value will be used
                                            for the <device> argument


[1mOptions:[0m
  [  --verbose, -v ]                    --- print additional sjtag-unlock
                                            information
  [  --force_hr, -f ]                   --- send HR for all SJTAG mode
```

### ./switchtec mfg help otp-ini2bin
<a name="switchtec-mfg-help-otp-ini2bin"></a>

```text
[1mUsage:[0m ./switchtec mfg help./switchtec mfg otp-ini2bin [<bin_file>] [OPTIONS]

generate bin from ini file


[1mPositional Arguments:[0m
  [<bin_file>]                          --- OTP Bin file


[1mOptions:[0m
  [  --ini_file=<arg>, -i <arg> ]       --- ini file as an input
```
