# Raspberry Pi Pico Infrared Remote Project

This project uses a raspberry pi pico to act as a universal infrared remote control. It uses an Android app to send commands to the pico over a USB connection, and the pico flashes an infrared LED (soldered onto a GPIO pin). This repo also contains a modified pico PCB replacing the female micro USB connector with a male USB C connector so the pi can plug directly into an android device with no cable.

The motivation for this project was to create a tiny and lightweight device that can perform most of the infrared functions that the Flipper Zero can. The Flipper Zero is a fantastic multi-tool, but I mostly use it for it's infrared capabilities. The pico is much smaller and doesn't require it's own battery as it can piggy back off of the android device I already carry.

## Repo structure

1. android-app: A program based off the SimpleUsbTerminal open source project to send serial data to a pico over USB. Contained are infrared files for a variety of TVs and other devices in the same format as the Flipper Zero uses.
2. pcb: A fork of the project-piCo project that has various pico boards with USB-C female connectors. I modified this to use a male USB-C connector instead. This project doesn't require you to print your own PCB - you can definitely use a stock pico with an adapter.
3. pico-firmware: The firmware for the pico. Some of the Flipper Zero infrared firmware was ported to support various infrared protocols and sending raw signals.