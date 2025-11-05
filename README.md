# MPLAB® Device Blocks for Simulink®

<img align="right" src="https://raw.githubusercontent.com/MPLAB-Blockset/MPLAB-Device-Blocks-for-Simulink/master/mplab-deviceblocksforsimulink-whitebackground.png" width="150">

### Model-Based Design for Microchip Microcontrollers

**Automatic embedded C code generation from Simulink® models for dsPIC®, PIC32®, and SAM microcontrollers**

![MATLAB Versions](https://img.shields.io/badge/MATLAB-R2015a%20to%20R2024b-blue)
![Devices](https://img.shields.io/badge/Devices-710%20MCUs-green)
![License](https://img.shields.io/badge/License-Microchip-red)

[![View MPLAB® Device Blocks for Simulink®: dsPIC® DSCs, PIC32® and SAM MCUs on File Exchange](https://www.mathworks.com/matlabcentral/images/matlab-file-exchange.svg)](https://fr.mathworks.com/matlabcentral/fileexchange/71892-mplab-device-blocks-for-simulink-dspic-pic32-and-sam-mcu)

---

## 🚀 Quick Start

### Installation

**Recommended Method (MATLAB® Toolstrip):**

1. Open MATLAB® → **Add-Ons** → **Get Add-Ons**
2. Search for **"MPLAB® Device Blocks for Simulink"**
3. Click **Install**

**Alternative Method (GitHub Release):**

For older MATLAB® versions (pre-R2016b) or manual installation:

1. Download the latest release from [GitHub Releases](https://github.com/MPLAB-Blockset/MPLAB-Device-Blocks-for-Simulink/releases)
2. Run the `install.p` script in MATLAB®

The installer provides flexible destination configuration for systems with restricted permissions.

### Essential Commands

| MATLAB® Command | Description |
|-----------------|-------------|
| `picsetup` | Configure or repair toolbox installation paths |
| `picclean` | Remove previous installations from MATLAB® path |
| `picInfo('check')` | Verify compilers and programming tools installation |
| `picInfo('examples')` | Copy example models to current working folder |
| `picInfo('cmd')` | Display all available commands |

### Getting Started Resources

- 📘 **[Full Documentation](https://mplab-blockset.github.io/MPLAB-Device-Blocks-for-Simulink/)** - Comprehensive guides and tutorials
- 🎥 **[Video Tutorials](#-resources--support)** - Webinars from MathWorks and Microchip
- 💬 **[Microchip MATLAB® Forum](https://forum.microchip.com/s/sub-forums?&subForumId=a553l000000J2rNAAS&forumId=a553l000000J2pvAAC&subForumName=MATLAB&page=1&offset=0)** - Community support and discussions

---

## ✨ Key Features

### 🎯 Wide Device Support
- **710+ Microcontrollers** supported across 12 device families
- **dsPIC®** DSCs: 30F, 33F, 33E, 33C, 33CH, **33A** (NEW - 32-bit with FPU)
- **PIC32®** MCUs: MK, MZ, MX, AK series
- **SAM** MCUs: SAME5x, SAME7x, SAMC2x, SAMD2x (ARM® Cortex®-M)

### ⚡ Automatic Code Generation
- **One-click deployment**: Simulink model → Embedded C → .hex file
- **MPLAB® X IDE** project generation
- **Optimized code** with assembly replacements for DSP operations (dsPIC®)
- **CMSIS DSP** library support for ARM® devices

### 🔧 Peripheral Configuration
Graphical interface blocks for all major peripherals:
- **PWM** (High-Resolution with Fine Edge Placement for dsPIC® 33A)
- **ADC** (synchronized with PWM, multiple conversion modes)
- **Communication**: UART, SPI, I2C, CAN
- **Timers**, Input Capture, Output Compare, QEI
- **Op-Amps**, Comparators, DAC
- **Change Notification**, GPIO

### 🔄 Real-Time Testing
- **External Mode**: Real-time parameter tuning and data visualization
- **Processor-in-the-Loop (PIL)**: On-target code verification
- **picgui Interface**: MATLAB®-based data logging and visualization

### 📊 Advanced Scheduler
- **Single-tasking** mode for simple applications
- **Multi-tasking** rate monotonic scheduler with:
  - Pre-emptive prioritization (fast loops interrupt slow loops)
  - Background task support for system monitoring
  - Configurable overload handling

### 💻 Code Optimization
- **Code Replacement**: Optimized sin/cos/atan2 for dsPIC® DSP engine
- **Fast memory operations**: Assembly-optimized memcpy
- **Fixed-point math**: Hardware MAC utilization
- **Link-time optimization** for reduced code size

---

## 📦 What's Included

<img align="right" src="https://raw.githubusercontent.com/MPLAB-Blockset/MPLAB-Device-Blocks-for-Simulink/master/MCHP_BlockLibrary.png" width="450">

### Simulink® Block Library

Complete peripheral driver blocks with intuitive GUI configuration. No low-level programming required.

### Board Templates

<img src="https://raw.githubusercontent.com/MPLAB-Blockset/MPLAB-Device-Blocks-for-Simulink/master/Templates.png" width="450">

Pre-configured models for popular development boards:
- dsPIC® 33A/33C/33CK Curiosity boards
- MCLV-48V-300W Motor Control board
- MCLV-2 Motor Control board
- LVMC Low-Voltage Motor Control board
- Explorer 16/32 development boards
- SAME70/SAME54 Xplained Ultra boards
- PIC32MZ/PIC32MK Curiosity boards

### Examples & Tutorials
- Motor control (FOC, sensorless, field weakening)
- Peripheral demonstrations (UART, SPI, I2C, CAN)
- External mode and PIL examples
- University research projects (INSA Lyon collaboration)

### Third-Party Integration
**UxV Blocks** for UAV/drone applications (GPS, MAVLink, telemetry) - type `picInfo` for installation instructions.

---

## 📋 Requirements

### MathWorks Products (R2015a to R2024b)
- MATLAB®
- Simulink®
- Embedded Coder™
- MATLAB® Coder™
- Simulink® Coder

### Microchip Development Tools

**IDE:**
- MPLAB® X IDE - [Download](https://www.microchip.com/MPLABX)

**Compilers:**
- **XC-DSC** for dsPIC® DSCs - [Download](https://www.microchip.com/xcdsc)
- **XC32** for PIC32® and SAM MCUs - [Download](https://www.microchip.com/xc32)

**Programmers/Debuggers:**
- PICkit™ 5, 4, 3
- MPLAB® ICD 5, 4, 3
- MPLAB® SNAP
- MPLAB® REAL-ICE™
- J-32 Debug Probe
- PICkit™ On-Board (PKOB4)

---

## 📚 Resources & Support

### 🎥 Video Tutorials

**Recent Webinars:**
- **[Power Factor Correction on dsPIC® 33A](https://www.mathworks.com/videos/implementing-power-factor-correction-on-a-microchip-dspic33a-digital-signal-controller-1734709897715.html)** (2023) - Latest dsPIC® 33A with FPU
- **[Motor Control Deployment](https://www.mathworks.com/videos/motor-control-with-embedded-coder-for-microchip-mcus-1488570451176.html)** (2023) - FOC algorithms for dsPIC®/PIC32®/SAM
- **[PMSM Position Control](https://www.mathworks.com/videos/position-control-of-a-pmsm-with-simulink-and-microchip-32-bit-mcus-1679516692608.html)** (2021) - 32-bit motor control
- **[External Mode Tutorial](https://www.microchip.com/en-us/about/media-center/videos/wuzLmORk1M0)** (2016) - Real-time debugging with dsPIC®

### 📖 Documentation
- **[Complete Documentation Portal](https://mplab-blockset.github.io/MPLAB-Device-Blocks-for-Simulink/)** - User guides, block reference, examples
- **[ctrl-elec Motor Control Tutorials](https://www.ctrl-elec.fr)** - Advanced FOC techniques by INSA Lyon

### 💬 Community Support
- **[Microchip MATLAB® Forum](https://forum.microchip.com/s/sub-forums?&subForumId=a553l000000J2rNAAS&forumId=a553l000000J2pvAAC&subForumName=MATLAB&page=1&offset=0)** - Ask questions and share experiences
- **[GitHub Repository](https://github.com/LubinKerhuel/MPLAB-Device-Blocks-for-Simulink)** - Examples and wiki

---

## 📝 Recent Release Notes

### v3.62 (Current)
- Fixed external mode for single-tasking scheduler
- Fixed PWM mid-point trigger for center-aligned mode (dsPIC® 33A/33C)
- Removed superfluous debug messages

### v3.61
- **Added**: DMA support on UART for SAMx54
- **Added**: SAME54 motor control FOC examples
- **Fixed**: UART code generation for dsPIC® 33C
- **Fixed**: PWM trigger settings for dsPIC® 33C/A center-aligned mode
- **Added**: Comparator with slope for dsPIC® 33C/A
- Dropped support for R2017a and earlier

### v3.60
- **Added**: Support for **PIC32CZ**, **PIC32M_MCA**, **PIC32A** families
- **Added**: Support for **dsPIC® 33AK MPS** series
- **Added**: DMA support on SPI (SAMx7)
- **Added**: Motor control examples for PIC32MK® & SAME54
- **Added**: VSCode project generation with MPLAB® VSCode Extension
- **Added**: PIL stack profiling for dsPIC®
- **Fixed**: PWM HS FEP timing issues at very low frequencies
- **Improved**: UART rtiostream driver throughput for dsPIC®

<details>
<summary><b>View Complete Release History (v3.45 - v3.59)</b></summary>

### v3.59
- Fixed template Op-Amp settings
- Updated dsPIC® A DFP for compiler v3.20
- Fixed dual-core dsPIC® pin configuration

### v3.58
- Added board templates: dsPIC® 33A Curiosity, dsPIC® 33CDV Motor Control
- Fixed integration with MATLAB® R2024b
- Improved MCLV-2/LVMC/MCLV-300W templates

### v3.57
- **Added**: [dsPIC® 33A family](https://www.microchip.com/dspic33a) - 32-bit CPU, 200MHz, FPU
- Fixed external mode for single-tasking models
- Improved programming interface

### v3.55-v3.56
- Added picgui.log2mat for offline log decoding
- Pin configuration: initial state, pull-up/down, open-drain
- Fixed masked subsystem variable access
- Support for MATLAB® R2024a improvements

*For complete history, see [Full Release Notes](https://github.com/MPLAB-Blockset/MPLAB-Device-Blocks-for-Simulink/releases)*

</details>

---

## 🖥️ Supported Microcontrollers (710 Devices)

### Device Family Summary

| Family | Count | Key Features | Representative Devices |
|--------|-------|--------------|----------------------|
| **dsPIC® 33A** | 38 | 32-bit CPU, 200MHz, FPU, Hi-Res PWM | 33AK128MC106, 33AK512MPS512 |
| **dsPIC® 33C/CH/CK/CDV** | 160+ | 16-bit DSC, Dual-core options | 33CK256MP508, 33CH512MP305 |
| **dsPIC® 33E/EP** | 100+ | 16-bit DSC, Hi-Speed ADC | 33EP256MC506, 33EP512GM710 |
| **dsPIC® 33F/FJ** | 80+ | 16-bit DSC, Motor Control PWM | 33FJ256GP710, 33FJ128MC506 |
| **dsPIC® 30F** | 16 | 16-bit DSC, Legacy support | 30F6015, 30F4013 |
| **PIC32® AK/MK/MZ** | 90+ | 32-bit MIPS, Motor Control | 32MK1024MCF064, 32MZ2048EFM100 |
| **PIC32® CZ** | 18 | 32-bit Arm® Cortex®-M7 | PIC32CZ5125CA70144 |
| **SAM C/D/E/S/V** | 200+ | Arm® Cortex®-M0+/M4/M7 | SAME54P20A, SAME70Q21B, SAMC21J18A |

<details>
<summary><b>View Complete Device List (710 MCUs)</b></summary>

### dsPIC® 30F Family (16 devices)
30F2010, 30F2011, 30F2012, 30F3010, 30F3011, 30F3012, 30F3013, 30F3014, 30F4011, 30F4012, 30F4013, 30F5011, 30F5013, 30F5015, 30F5016, 30F6010, 30F6011, 30F6012, 30F6013, 30F6014, 30F6015

### dsPIC® 33A Family (38 devices)

**dsPIC® 33AK MC Series**
33AK32MC102, 33AK32MC103, 33AK32MC105, 33AK32MC106, 33AK64MC102, 33AK64MC103, 33AK64MC105, 33AK64MC106, 33AK128MC102, 33AK128MC103, 33AK128MC105, 33AK128MC106, 33AK256MC205, 33AK256MC206, 33AK256MC208, 33AK256MC210, 33AK256MC505, 33AK256MC506, 33AK256MC508, 33AK256MC510, 33AK512MC205, 33AK512MC206, 33AK512MC208, 33AK512MC210, 33AK512MC505, 33AK512MC506, 33AK512MC508, 33AK512MC510

**dsPIC® 33AK MPS Series**
33AK256MPS205, 33AK256MPS206, 33AK256MPS208, 33AK256MPS210, 33AK256MPS212, 33AK256MPS505, 33AK256MPS506, 33AK256MPS508, 33AK256MPS510, 33AK256MPS512, 33AK512MPS205, 33AK512MPS206, 33AK512MPS208, 33AK512MPS210, 33AK512MPS212, 33AK512MPS505, 33AK512MPS506, 33AK512MPS508, 33AK512MPS510, 33AK512MPS512

### dsPIC® 33C Family (170+ devices)

**dsPIC® 33CDV/CDVC/CDVL Series**
33CDV64MC106, 33CDV128MP206, 33CDV128MP506, 33CDV256MP206, 33CDV256MP506, 33CDVC128MP506, 33CDVC256MP506, 33CDVL64MC106

**dsPIC® 33CH Series (Dual-Core)**
33CH64MP202, 33CH64MP203, 33CH64MP205, 33CH64MP206, 33CH64MP208, 33CH64MP502, 33CH64MP503, 33CH64MP505, 33CH64MP506, 33CH64MP508, 33CH128MP202, 33CH128MP203, 33CH128MP205, 33CH128MP206, 33CH128MP208, 33CH128MP502, 33CH128MP503, 33CH128MP505, 33CH128MP506, 33CH128MP508, 33CH256MP205, 33CH256MP206, 33CH256MP208, 33CH256MP218, 33CH256MP505, 33CH256MP506, 33CH256MP508, 33CH512MP205, 33CH512MP206, 33CH512MP208, 33CH512MP305, 33CH512MP306, 33CH512MP308, 33CH512MP405, 33CH512MP406, 33CH512MP408, 33CH512MP410, 33CH512MP412, 33CH512MP505, 33CH512MP506, 33CH512MP508, 33CH512MP605, 33CH512MP606, 33CH512MP608, 33CH512MP705, 33CH512MP706, 33CH512MP708, 33CH512MP710, 33CH512MP712, 33CH1024MP305, 33CH1024MP306, 33CH1024MP308, 33CH1024MP405, 33CH1024MP406, 33CH1024MP408, 33CH1024MP410, 33CH1024MP412, 33CH1024MP605, 33CH1024MP606, 33CH1024MP608, 33CH1024MP705, 33CH1024MP706, 33CH1024MP708, 33CH1024MP710, 33CH1024MP712

**dsPIC® 33CK Series**
33CK32MP102, 33CK32MP103, 33CK32MP105, 33CK32MP202, 33CK32MP203, 33CK32MP205, 33CK32MP206, 33CK32MP502, 33CK32MP503, 33CK32MP505, 33CK32MP506, 33CK64MP102, 33CK64MP103, 33CK64MP105, 33CK64MP202, 33CK64MP203, 33CK64MP205, 33CK64MP206, 33CK64MP208, 33CK64MP502, 33CK64MP503, 33CK64MP505, 33CK64MP506, 33CK64MP508, 33CK128MP202, 33CK128MP203, 33CK128MP205, 33CK128MP206, 33CK128MP208, 33CK128MP502, 33CK128MP503, 33CK128MP505, 33CK128MP506, 33CK128MP508, 33CK256MP202, 33CK256MP203, 33CK256MP205, 33CK256MP206, 33CK256MP208, 33CK256MP305, 33CK256MP306, 33CK256MP308, 33CK256MP405, 33CK256MP406, 33CK256MP408, 33CK256MP410, 33CK256MP502, 33CK256MP503, 33CK256MP505, 33CK256MP506, 33CK256MP508, 33CK256MP605, 33CK256MP606, 33CK256MP608, 33CK256MP705, 33CK256MP706, 33CK256MP708, 33CK256MP710, 33CK512MP305, 33CK512MP306, 33CK512MP308, 33CK512MP405, 33CK512MP406, 33CK512MP408, 33CK512MP410, 33CK512MP605, 33CK512MP606, 33CK512MP608, 33CK512MP705, 33CK512MP706, 33CK512MP708, 33CK512MP710, 33CK1024MP405, 33CK1024MP406, 33CK1024MP408, 33CK1024MP410, 33CK1024MP705, 33CK1024MP706, 33CK1024MP708, 33CK1024MP710

**dsPIC® 33CK MC Series**
33CK32MC102, 33CK32MC103, 33CK32MC105, 33CK64MC102, 33CK64MC103, 33CK64MC105, 33CK128MC102, 33CK128MC103, 33CK128MC105, 33CK128MC106, 33CK128MC502, 33CK128MC503, 33CK128MC505, 33CK128MC506, 33CK256MC102, 33CK256MC103, 33CK256MC105, 33CK256MC106, 33CK256MC502, 33CK256MC503, 33CK256MC505, 33CK256MC506

**dsPIC® 33CK MPT Series**
33CK256MPT608, 33CK512MPT608

### dsPIC® 33E/33EP Family (100+ devices)

**dsPIC® 33EP GM/GP/GS/MC/MU Series**
*(Complete list available - 100+ devices across GM, GP, GS, MC, MU subfamilies)*

**dsPIC® 33EV GM Series**
33EV32GM002, 33EV32GM003, 33EV32GM004, 33EV32GM006, 33EV32GM102, 33EV32GM103, 33EV32GM104, 33EV32GM106, 33EV64GM002, 33EV64GM003, 33EV64GM004, 33EV64GM006, 33EV64GM102, 33EV64GM103, 33EV64GM104, 33EV64GM106, 33EV128GM002, 33EV128GM003, 33EV128GM004, 33EV128GM006, 33EV128GM102, 33EV128GM103, 33EV128GM104, 33EV128GM106, 33EV256GM002, 33EV256GM003, 33EV256GM004, 33EV256GM006, 33EV256GM102, 33EV256GM103, 33EV256GM104, 33EV256GM106

### dsPIC® 33F/33FJ Family (80+ devices)

**dsPIC® 33FJ GP/MC Series**
33FJ12GP201, 33FJ12GP202, 33FJ16GP304, 33FJ32GP202, 33FJ32GP204, 33FJ32GP302, 33FJ32GP304, 33FJ64GP202, 33FJ64GP204, 33FJ64GP206, 33FJ64GP206A, 33FJ64GP306, 33FJ64GP306A, 33FJ64GP310, 33FJ64GP310A, 33FJ64GP706, 33FJ64GP706A, 33FJ64GP708, 33FJ64GP708A, 33FJ64GP710, 33FJ64GP710A, 33FJ64GP802, 33FJ64GP804, 33FJ128GP202, 33FJ128GP204, 33FJ128GP206, 33FJ128GP206A, 33FJ128GP306, 33FJ128GP306A, 33FJ128GP310, 33FJ128GP310A, 33FJ128GP706, 33FJ128GP706A, 33FJ128GP708, 33FJ128GP708A, 33FJ128GP710, 33FJ128GP710A, 33FJ128GP802, 33FJ128GP804, 33FJ256GP506, 33FJ256GP506A, 33FJ256GP510, 33FJ256GP510A, 33FJ256GP710, 33FJ256GP710A
*(Plus 33FJ MC series - motor control variants)*

### PIC32® Family (100+ devices)

**PIC32® AK GC Series**
32AK1216GC41036, 32AK1216GC41048, 32AK1216GC41064, 32AK3208GC41036, 32AK3208GC41048, 32AK3208GC41064, 32AK6416GC41036, 32AK6416GC41048, 32AK6416GC41064

**PIC32® MK Series (Motor Control)**
32MK0128MCA028, 32MK0128MCA032, 32MK0128MCA048, 32MK0256GPG048, 32MK0256GPG064, 32MK0256MCJ048, 32MK0256MCJ064, 32MK0512GPG048, 32MK0512GPG064, 32MK0512GPK064, 32MK0512GPK100, 32MK0512MCF064, 32MK0512MCF100, 32MK0512MCJ048, 32MK0512MCJ064, 32MK0512MCM064, 32MK0512MCM100, 32MK1024GPK064, 32MK1024GPK100, 32MK1024MCF064, 32MK1024MCF100, 32MK1024MCM064, 32MK1024MCM100

**PIC32® MZ Series (High-Performance)**
32MZ0512EFE064, 32MZ0512EFE100, 32MZ0512EFE124, 32MZ0512EFE144, 32MZ0512EFF064, 32MZ0512EFF100, 32MZ0512EFF124, 32MZ0512EFF144, 32MZ0512EFK064, 32MZ0512EFK100, 32MZ0512EFK124, 32MZ0512EFK144
*(Plus 1024KB and 2048KB variants across EFE/EFF/EFK/EFG/EFH/EFM subfamilies)*

**PIC32® CZ Series (Cortex®-M7)**
PIC32CZ1038CA70064, PIC32CZ1038CA70100, PIC32CZ1038CA70144, PIC32CZ1038MC70064, PIC32CZ1038MC70100, PIC32CZ2051CA70064, PIC32CZ2051CA70100, PIC32CZ2051CA70144, PIC32CZ2051MC70064, PIC32CZ2051MC70100, PIC32CZ5125CA70064, PIC32CZ5125CA70100, PIC32CZ5125CA70144, PIC32CZ5125MC70064, PIC32CZ5125MC70100

**PIC32® CM Series**
PIC32CM1216MC00032, PIC32CM1216MC00048, PIC32CM6408MC00032, PIC32CM6408MC00048

### SAM Family (ARM® Cortex®-M) (200+ devices)

**SAMC20/SAMC21 Series (Cortex®-M0+)**
SAMC20E15A through SAMC20N18A (16 devices)
SAMC21E15A through SAMC21N18A (16 devices)

**SAMD20/SAMD21 Series (Cortex®-M0+)**
SAMD20E14 through SAMD20J18 (26 devices)
SAMD21E15A through SAMD21J18A (40 devices)

**SAME51/SAME53/SAME54 Series (Cortex®-M4F)**
SAME51G18A through SAME51N20A (7 devices)
SAME53J18A through SAME53N20A (5 devices)
SAME54N19A through SAME54P20A (4 devices)

**SAME70/SAMS70/SAMV70/SAMV71 Series (Cortex®-M7)**
SAME70J19B through SAME70Q21B (9 devices)
SAMS70J19B through SAMS70Q21B (9 devices)
SAMV70J19B through SAMV70Q20B (6 devices)
SAMV71J19B through SAMV71Q21B (9 devices)

**SAMRH Series (Radiation-Hardened)**
SAMRH707F18A, SAMRH71F20B, SAMRH71F20C

</details>

---

## 🎯 Target Applications

### ⚡ Motor Control
PMSM/BLDC control, Field-Oriented Control (FOC), sensorless algorithms, field weakening

### 🔋 Power Electronics
Digital power supplies, Power Factor Correction (PFC), inverters, DC-DC converters

### 🏭 Industrial Automation
PLC applications, sensor interfaces, communication protocols, real-time control

### 🎛️ Embedded Systems
Signal processing, data acquisition, control loops, IoT applications

---

## 📄 License & Credits

**Developed by:** Microchip Technology Inc.

**Contributors:**
- Lubin KERHUEL (Microchip Technology Inc.)
- Romain DELPOUX (INSA Lyon, Laboratoire Ampère) - Motor control examples

**License:** Microchip Technology End User License Agreement

---

## 🔗 Important Links

- **[Full Documentation](https://mplab-blockset.github.io/MPLAB-Device-Blocks-for-Simulink/)** - Comprehensive guides
- **[GitHub Repository](https://github.com/LubinKerhuel/MPLAB-Device-Blocks-for-Simulink)** - Source code and examples
- **[MathWorks File Exchange](https://www.mathworks.com/matlabcentral/fileexchange/71892)** - Download and reviews
- **[Microchip Forums](https://forum.microchip.com/s/sub-forums?&subForumId=a553l000000J2rNAAS)** - Community support
- **[ctrl-elec Project](https://www.ctrl-elec.fr)** - Advanced motor control tutorials
- **[Microchip.com](https://www.microchip.com)** - Official product information

---

<div align="center">

**MPLAB® Device Blocks for Simulink®**

*Empowering embedded systems development with Model-Based Design*

[![GitHub](https://img.shields.io/badge/GitHub-Repository-black?logo=github)](https://github.com/LubinKerhuel/MPLAB-Device-Blocks-for-Simulink)
[![File Exchange](https://img.shields.io/badge/MATLAB-File%20Exchange-orange?logo=mathworks)](https://www.mathworks.com/matlabcentral/fileexchange/71892)
[![Forum](https://img.shields.io/badge/Microchip-Forum-blue?logo=microchip)](https://forum.microchip.com/s/sub-forums?&subForumId=a553l000000J2rNAAS)

</div>
