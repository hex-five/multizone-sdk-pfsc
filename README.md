# multizone-sdk-pfsc (Beta Version)
MultiZone® Security for Microsemi Polarfire SoC

**MultiZone® Security** is the quick and safe way to add security and separation to RISC-V processors. MultiZone software can retrofit existing designs. If you don’t have TrustZone-like hardware, or if you require finer granularity than one secure world, you can take advantage of high security separation without the need for hardware and software redesign, eliminating the complexity associated with managing a hybrid hardware/software security scheme. RISC-V standard ISA doesn't define TrustZone-like primitives to provide hardware separation. To shield critical functionality from untrusted third-party components, MultiZone provides hardware-enforced, software-defined separation of multiple equally secure worlds. Unlike antiquated hypervisor-like solutions, MultiZone is self-contained, presents an extremely small attack surface, and it is policy driven, meaning that no coding is required – and in fact even allowed.

MultiZone works with any 32-bit or 64-bit RISC-V standard processors with Physical Memory Protection unit and U mode.

This version of the GNU-based SDK supports the following development hardware:

- [Microchip PolarFire SoC FPGA Icicle Kit](https://www.microsemi.com/existing-parts/parts/152514)

TBD ...


### MultiZone SDK Installation ###

- [RISC-V Prebuilt Toolchain](https://hex-five.com/wp-content/uploads/riscv-gnu-toolchain-20200613.tar.xz)
- [Microchip OpenOCD](https://www.microsemi.com/product-directory/design-tools/4879-softconsole#downloads)

TBD ...


### Build & load the MultiZone reference application ###

```
cd ~/multizone-sdk-pfsc
export RISCV=~/riscv-gnu-toolchain-20200613
export OPENOCD=~/SoftConsole-v6.4/openocd
make
make load
```

TBD ...


### Run the MultiZone reference application ###

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

TBD ...


### Technical Specs ###

TBD ...


### Additional Resources ###

TBD ...


### Legalities ###

Please remember that export/import and/or use of strong cryptography software, providing cryptography hooks, or even just communicating technical details about cryptography software is illegal in some parts of the world. So when you import this software to your country, re-distribute it from there or even just email technical suggestions or even source patches to the authors or other people you are strongly advised to pay close attention to any laws or regulations which apply to you. Hex Five Security, Inc. and the authors of the software included in this repository are not liable for any violations you make here. So be careful, it is your responsibility.

MultiZone and HEX-Five are registered trademarks of Hex Five Security, Inc.

MultiZone technology is patent pending US 16450826, PCT US1938774.


