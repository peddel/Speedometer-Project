# Client-Server Speedometer System

## Overview:
The Client-Server Speedometer System is designed to provide a real-time dashboard for monitoring speed, temperature, battery level, and light signals. Built using the Qt framework, the server controls and updates the displayed values, facilitating effective communication through a robust abstract communication interface. This project is ideal for understanding client-server architecture, multithreading, and real-time data management.

![Client](../images/client.png) ![Server](../images/server.png)

## Key Features:

**Server Control:**

Manages and updates values for speed (0-240), temperature (-60°C to 60°C), and battery level (0-100%).
        
Implements sliders on the server GUI to adjust these parameters.

Contains three checkboxes for light signal states with mutually exclusive selections.

**Client Dashboard:**

Displays real-time values for speed, temperature, battery level, and light signal statuses using the Qt framework.

Features color-coded indicators for temperature and battery levels to visually represent their status:

**Temperature:** White (<5°C), Blue (5-39°C), Red (>39°C).
            
**Battery Level:** Red (<25%), Yellow (25-49%), Green (>49%).
        
Notifies users of active signals with blinking icons and sound alerts.

## Communication Protocols:

**Abstract Communication Interface:**

Supports TCP/IP, Serial, and CAN2.0B protocols.
        
Utilizes an abstract base class to manage communication between the server and client seamlessly.

## Error Handling:

**Automatic Error Notifications:**

Server-side logs communication errors to the terminal.

Client-side displays error messages and attempts to reconnect automatically if communication issues arise.

## Technologies:

**Programming Language:** C++
  
**Development Tools:** Visual Studio, PlatformIO, CMake

**Communication Protocols:** TCP/IP, Serial, CAN2.0B

**User Interface:** Qt

## Setup Instructions:

**Dependencies:**

Ensure you have the following installed:

CMake (minimum version 3.22)

Qt 6 (with required components: Widgets, Core, SerialPort, Multimedia)

PlatformIO (for uploading firmware to ESP32 microcontrollers)

**CMake Configuration:**

Navigate to your project directory in the terminal.

Run the following command to generate the build files:

    cmake -S . -B build

**Choosing Communication Protocol:**

You can toggle between CAN and TCP communication protocols using CMake options:
        
To use CAN protocol, run:

    cmake -DUSE_CAN=ON -S . -B build

To use TCP protocol, run:

    cmake -DUSE_CAN=OFF -S . -B build

**Building the Project:**

After configuring, build the project by running:

    cmake --build build

The executable files for both the client and server will be created in the build directory based on the selected communication protocol.

**Custom Targets for PlatformIO (Only if CAN is set to on and UART is being used):**

The CMake file includes custom targets for building and uploading firmware to the ESP32 microcontrollers:

To build the Client microcontroller:

    cmake --build build --target pio_client_build

To upload the Client firmware to the microcontroller:

	cmake --build build --target pio_client_upload

To build the Server microcontroller:

	cmake --build build --target pio_server_build

To upload the Server firmware to the microcontroller:

    cmake --build build --target pio_server_upload

**Font Installation:**

The project requires the Material Icons font. The CMake script includes functionality to locate and install this font. If not found in standard font directories, the script attempts to copy it from the provided resource directory within the project.

**Run the Applications:**

After building, the client and server applications can be executed from the terminal as follows:
		
	./build/server & ./build/client
