# RYLR998 STM32 Library

This repository provides a comprehensive library for interfacing with the **RYLR998 LoRa module** using an **STM32** microcontroller. It includes functions for configuring various parameters and performing communication tasks using AT commands.

## Features:
- **Send data** to a specific address using the AT command protocol.
- **Set network ID**, **module address**, and other configuration parameters.
- Functions to **reset the module**, **set baud rate**, **frequency band**, and more.
- Supports configuring **spreading factor (SF)**, **bandwidth (BW)**, **coding rate (CR)**, and other LoRa-specific settings.


## Requirements:
- STM32 microcontroller
- **RYLR998 LoRa module** connected via UART1.

## Quickstart
* Enable the DMA UART Rx in circular mode

