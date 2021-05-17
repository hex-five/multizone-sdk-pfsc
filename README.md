# multizone-sdk-pfsc (Beta Version)
MultiZone® Trusted Firmware for Microchip Polarfire SoC

**MultiZone® Security** is the quick and safe way to add security and separation to RISC-V processors. MultiZone software can retrofit existing designs. If you don’t have TrustZone-like hardware, or if you require finer granularity than one secure world, you can take advantage of high security separation without the need for hardware and software redesign, eliminating the complexity associated with managing a hybrid hardware/software security scheme. RISC-V standard ISA doesn't define TrustZone-like primitives to provide hardware separation. To shield critical functionality from untrusted third-party components, MultiZone provides hardware-enforced, software-defined separation of multiple equally secure worlds. Unlike antiquated hypervisor-like solutions, MultiZone is self-contained, presents an extremely small attack surface, and it is policy driven, meaning that no coding is required – and in fact even allowed.

MultiZone works with any 32-bit or 64-bit RISC-V standard processors with Physical Memory Protection unit and U mode.

This version of the GNU-based SDK supports the following development hardware:

- [Microchip PolarFire SoC Icicle Kit](https://www.microsemi.com/existing-parts/parts/152514) ([Reference Design 2021.02](https://github.com/polarfire-soc/icicle-kit-reference-design/releases/tag/2021.02))


### MultiZone SDK Installation ###

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


### Build and load the MultiZone Trusted Firmware ###

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

alternatively, use the Eclipse CDT project included in this repo.


### Run the MultiZone Trusted Firmware ###

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
CPU clock     : 78 MHz 
RTC clock     : 1000 KHz 

Z1 > Commands: yield send recv pmp load store exec dma stats timer restart 

Z1 > 
```
- observe Zone 2 heartbeat LED2 (red)
- press SW2 to toggle LED4 (yellow)
- press SW3 to toggle LED3 (yellow)

TBD ...


### Technical Specs ###

TBD ...


### Additional Resources ###

TBD ...


### Legalities ###

Please remember that export/import and/or use of strong cryptography software, providing cryptography hooks, or even just communicating technical details about cryptography software is illegal in some parts of the world. So when you import this software to your country, re-distribute it from there or even just email technical suggestions or even source patches to the authors or other people you are strongly advised to pay close attention to any laws or regulations which apply to you. Hex Five Security, Inc. and the authors of the software included in this repository are not liable for any violations you make here. So be careful, it is your responsibility.

MultiZone and HEX-Five are registered trademarks of Hex Five Security, Inc.

MultiZone technology is patent pending US 16450826, PCT US1938774.


