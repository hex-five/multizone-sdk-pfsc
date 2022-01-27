# multizone-sdk-pfsc
MultiZone® Security TEE for Microchip PolarFire SoC

**MultiZone® Security** is the quick and safe way to add security and separation to RISC-V processors. MultiZone software can retrofit existing designs. If you don’t have TrustZone-like hardware, or if you require finer granularity than one secure world, you can take advantage of high security separation without the need for hardware and software redesign, eliminating the complexity associated with managing a hybrid hardware/software security scheme. RISC-V standard ISA doesn't define TrustZone-like primitives to provide hardware separation. To shield critical functionality from untrusted third-party components, MultiZone provides hardware-enforced, software-defined separation of multiple equally secure worlds. Unlike antiquated hypervisor-like solutions, MultiZone is self-contained, presents an extremely small attack surface, and it is policy driven, meaning that no coding is required – and in fact even allowed.

MultiZone works with any 32-bit or 64-bit RISC-V processors with standard Physical Memory Protection unit (PMP) and “U” mode.

This release of the MultiZone SDK supports the following development boards:
- [Digilent Arty A7 Development Board (Xilinx Artix-7 FPGA)](https://digilent.com/shop/arty-a7-artix-7-fpga-development-board/)
- [Andes Corvette-F1 R1.0 (Xilinx Artix-7 FPGA)](http://www.andestech.com/en/products-solutions/andeshape-platforms/corvette-f1-r1/)
- [Microchip Icicle Kit (PolarFire SoC)](https://www.microsemi.com/existing-parts/parts/152514)
- [SiFive HiFive1 Rev B (Freedom E310 SoC)](https://www.sifive.com/boards/hifive1-rev-b)
- [SiFive Unleashed (Freedom U540 SoC)](https://www.sifive.com/boards/hifive-unleashed)

This repository is for the Microchip Icicle Kit board reference design [2021.02](https://github.com/polarfire-soc/icicle-kit-reference-design/releases/tag/2021.02), [2021.04](https://github.com/polarfire-soc/icicle-kit-reference-design/releases/tag/2021.04), [2021.08](https://github.com/polarfire-soc/icicle-kit-reference-design/releases/tag/2021.08), [2021.11](https://github.com/polarfire-soc/icicle-kit-reference-design/releases/tag/2021.11)

### MultiZone SDK Installation ###

The MultiZone SDK works with any versions of Linux, Windows, and Mac capable of running Java 1.8 or greater. The directions in this readme have been carefully verified with fresh installations of Ubuntu 20.04, Ubuntu 19.10, Ubuntu 18.04.5, and Debian 10.5. Other Linux distros are similar. Windows developers may want to install a Linux emulation environment like MYSYS2/MinGW64 or, even better, Windows Subsystem for Linux. Hex Five's precompiled gnu toolchain and openOCD for Windows are available at https://hex-five.com/download/

Note: GtkTerm is optional and required only to connect to the reference application via UART. It is not required to build, debug, and load the MultiZone software. Any other serial terminal application of choice would do.


**Microchip prerequisites**

- [Microchip SoftConsole (RISC-V Toolchain and OpenOCD)](https://www.microsemi.com/product-directory/design-tools/4879-softconsole#downloads)
- [Microchip FlashPro Programmer (fpgenprog)](https://www.microsemi.com/product-directory/programming-and-debug/4977-flashpro)

**MultiZone Security SDK**

```
cd ~
git clone --recursive https://github.com/hex-five/multizone-sdk-pfsc.git
cd multizone-sdk-pfsc
git apply -p1 ext/bare-metal-lib.patch --directory=ext/bare-metal-lib
```


### Build & load the MultiZone reference application ###

```
export RISCV=.../SoftConsole-v2021.1/riscv-unknown-elf-gcc
export OPENOCD=.../SoftConsole-v2021.1/openocd
export SC_INSTALL_DIR=.../microsemi/SoftConsole-v2021.1
export FPGENPROG=.../Libero_SoC_v2021.1/Libero/bin64/fpgenprog
```
build and load to ram for debug (boot mode 0):

```
make BOARD=PFSC-LIM 
make BOARD=PFSC-LIM load
```

build and load to flash for production (boot mode 1):

```
make BOARD=PFSC-ENVM 
make BOARD=PFSC-ENVM load
```

### Run the MultiZone reference application ###

Connect the UART port (Icicle Kit micro USB J11) as indicated in the user manual.

On your computer, start a serial terminal console (GtkTerm) and connect to /dev/ttyUSB0 at 115200-8-N-1

Hit the enter key a few times until the cursor 'Z1 >' appears on the screen

Enter 'restart' to display the splash screen

Hit enter again to show the list of available commands

```
=====================================================================
      	             Hex Five MultiZone® Security    
    Copyright© 2020 Hex Five Security, Inc. - All Rights Reserved
=====================================================================
This version of MultiZone® Security is meant for evaluation purposes
only. As such, use of this software is governed by the Evaluation
License. There may be other functional limitations as described in
the evaluation SDK documentation. The commercial version of the
software does not have these restrictions.
=====================================================================
Machine ISA   : 0x00101105 RV64 ACIMU
Vendor        : 0x00000000
Architecture  : 0x00000000
Implementation: 0x00000000
Hart id       : 0x0
CPU clock     : 600 MHz
RTC clock     : 1 MHz

PLIC @0x0c000000
DMAC @0x20009000
GPIO @0x20122000

Z1 > Commands: yield send recv pmp load store exec stats timer restart dma
```
- observe Zone 2 heartbeat LED2 (red)
- press SW2 to toggle LED4 (yellow)
- press SW3 to toggle LED3 (yellow)

On your computer, start a new serial terminal console (GtkTerm) and connect to /dev/ttyUSB1 at 115200-8-N-1

Hit the enter key a few times until the cursor 'H1 >' appears on the screen

Hit enter again to show the list of available commands

```
=====================================================================
      	             Hex Five MultiZone® Security                    
    Copyright© 2020 Hex Five Security, Inc. - All Rights Reserved    
=====================================================================
This version of MultiZone® Security is meant for evaluation purposes 
only. As such, use of this software is governed by the Evaluation    
License. There may be other functional limitations as described in   
the evaluation SDK documentation. The commercial version of the      
software does not have these restrictions.                           
=====================================================================

H1 > Commands: load store exec send recv pmp
```
- verify separation policies (commands pmp, load, store, exec)
- exchange IPC messages (ping) across TEE zones and app cluster (send/recv)


### Optional: Eclipse CDT Project ###
This repository includes an optional Eclipse CDT project for developers familiar with this IDE. No additional plugins are required to build and upload MultiZone to the target. The [OpenOCD debugging plug-in](https://eclipse-embed-cdt.github.io/debug/openocd) is optional and recommended.

**Eclipse project Setup**

File > Open Projects from File System > Import source: ~/multizone-sdk-pfsc

Project > Properties > C/C++ Build > Environment: set variables according to your installation

![alt text](https://hex-five.com/wp-content/uploads/multizone-eclipse-proj.png)

### MultiZone TEE Technical Specs ###
| |
|---|
| Up to 4 hardware threads (zones) hardware-enforced, software-defined                  |
| Up to 16 memory mapped resources per zone – i.e. flash, ram, rom, i/o, etc.           |
| Scheduler: preemptive, cooperative, round robin, configurable tick or tickless        |
| Secure interzone communications based on messages – no shared memory                  |
| Built-in support for secure shared Timer interrupt                                    |
| Built-in support for secure shared PLIC interrupt                                     |
| Built-in support for secure DMA transfers                                             |
| Built-in support for CLIC, CLINT, and PLIC interrupt controllers                      |
| Built-in trap & emulation for all privileged instructions – csrr, csrw, ecall, etc.   |
| Support for secure user-mode interrupt handlers mapped to zones – up to 32/64 sources |
| Support for CPU deep-sleep suspend mode for low power applications - wfi              |
| Formally verifiable runtime ~4KB, 100% written in assembly, no 3rd-party dependencies |
| C macro wrappers for protected mode execution – optional for high speed low-latency   |
| Hardware requirements: RV32, RV32e, RV64 cpu with Memory Protection Unit and 'U' mode | 
| System requirements: 8KB FLASH, 4KB ITIM, 2KB DTIM - CPU overhead < 0.01%             | 
| Development environment: any versions of Linux, Windows, Mac running Java 1.8 or newer|


### Additional Resources ###

- [MultiZone Reference Manual](http://github.com/hex-five/multizone-sdk/blob/master/manual.pdf)
- [MultiZone Datasheet](https://hex-five.com/wp-content/uploads/2020/01/multizone-datasheet-20200109.pdf)
- [MultiZone Website](https://hex-five.com/multizone-security-sdk/)
- [Frequently Asked Questions](http://hex-five.com/faq/)
- [Contact Hex Five http://hex-five.com/contact](http://hex-five.com/contact)


### Legalities ###

Please remember that export/import and/or use of strong cryptography software, providing cryptography hooks, or even just communicating technical details about cryptography software is illegal in some parts of the world. So when you import this software to your country, re-distribute it from there or even just email technical suggestions or even source patches to the authors or other people you are strongly advised to pay close attention to any laws or regulations which apply to you. Hex Five Security, Inc. and the authors of the software included in this repository are not liable for any violations you make here. So be careful, it is your responsibility.

_MultiZone and HEX-Five are registered trademarks of Hex Five Security, Inc._

_MultiZone technology is protected by patents US 11,151,262 and PCT/US2019/038774_
