# paper-shoot-camera--
ESP32-CAM based camera system that captures images using a button trigger and saves them to an SD card

# ESP32-CAM Photo Capture System

A simple embedded camera system built using the ESP32-CAM module.  
The device captures an image when a button is pressed and saves the photo directly to a microSD card.

This project demonstrates how microcontrollers can interact with camera modules and storage devices to create a basic standalone imaging system.

---

## Overview

The ESP32-CAM module integrates a microcontroller, camera sensor, and microSD storage interface in a compact board.  

In this project, a push button is used as a trigger. When the button is pressed, the ESP32-CAM captures an image and stores it on the SD card.

This creates a simple standalone camera device.

---

## Hardware Used

- ESP32-CAM module
- OV2640 camera sensor (built into ESP32-CAM)
- Push button
- MicroSD card
- Jumper wires
- Power supply

---

## Working Principle

1. The ESP32 initializes the camera module.
2. The SD card interface is mounted for storage.
3. The system continuously monitors the button input.
4. When the button is pressed:
   - The camera captures a frame.
   - The image is saved to the SD card.
5. The system returns to idle mode and waits for the next capture.

---

## Code Features

- Camera initialization using ESP32 camera libraries
- SD card file storage
- Button-triggered image capture
- Basic embedded system control loop

---

## Applications

Projects like this form the foundation for:

- Embedded camera systems
- Security cameras
- Robotics vision modules
- IoT monitoring devices
- Automated inspection systems

---

## Future Improvements

- WiFi image transfer
- Compact size
- Web-based camera interface
- Motion detection
- Cloud storage integration
  
