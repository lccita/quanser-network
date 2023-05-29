# quanser-network

This project aims to allow people to share their Quanser data acquisition boards (DAQ) in the internet or local network through an agnostic communication protocol. The idea is simple:

- the board stays physically connected to some computer, named here as "server", which will have the Quanser HIL SDK or QuaRC driver installed and run the application named "server"
- other computers could build an application that connects to the server using TCP/IP protocol and control the DAQ without the need of installing any drivers
- the server will accept only one client at a time, multiple users is not supported


## Supported features

Only immediate I/O operations are supported. However, you will be able to control almost all features of the boards. You will also be able to write applications with:

- C/C++
- Simulink (available soon)
- Python 3 (available soon)


## How to get started

If you are using only the client in Simulink, then binaries are ready for you. Just download them by clicking the "tags" button in the github repository. The Python bindings are not available due the big number of Python versions.

To do this, start by downloading and installing the following software:

- [Microsoft Visual Studio Community Edition](https://visualstudio.microsoft.com/pt-br)
- [CMake](https://cmake.org/download/)

### The server application

Unfortunaltely you will be required to build the server in your own computer. In the server computer it is mandatory that the available drivers are installed and they must offer support to your board(s). You have basically two options:

- [HIL SDK](https://github.com/quanser/hil_sdk_win64 "HIL SDK") is a freeware driver, unfortunately only a few boards is supported
- [QuaRC](https://www.quanser.com/products/quarc-real-time-control-software/ "QuaRC") is a premium software provided by Quanser, the license is not free but it supports real time operations, Simulink and network natively

DO NOT change the default path during the drivers installation process. If you do, the compilation step will fail and you will be required to manually edit the CMakeLists file.

### The Simulink client

### The Python bindings

### Building

Follow the instructions to build:

1. Download a copy of this repository by clicking on "Code" button and selecting "Download zip" option
2. Extract the file and inside of the create folder (typically named "quanser-network-main"), create a new folder called "build"
3. Open cmake-gui, click on "Browse Source..." button and select the "src" folder inside the extracted folder (step 1)
4. Next, click on the "Browse Build..." button and select the "build" folder you just created (step 2)
5. Next, click on the "Configure" button. This step may take a while because it will download and configure all software dependencies, so, it is required you are connected to internet
6. Then, click on the "Generate" button
7. You may now close the CMake interface, navigate to the build folder using Windows Explorer and then open the file "quanserNetwork.sln" with Visual Studio
8. If you are building the server application or Simulink/Python bindings, then you certainly wants to switch the profile to Release, instead of Debug
9. Now, hit CTRL+SHIFT+B or click on Build Solution and be patient
10. The server application (server.exe) will be placed in build/Release folder. You should copy this file somewhere else


### Starting the server

The server application was designed to avoid hard coded stuff. If you have two or more boards and wish to keep them attached to a single computer, this is possible and easy. The server application requires to be executed from terminal with two options:

1. --board=<SOME_BOARD>
2. --port=<SOME_NUMBER>

One example is: 

```
server.exe --board=q2_usb --port=2000
```

The options to boards are:

- q2_usb
- q8_usb
- dummy (useful for testing the server and client communication)

If you are afraid of using the terminal, then inside the folder src/server there is a script, a bat file. Copy it to the the same folder which will contain the server application and then edit it (with notepad or notepad++) accordingly with your needs. Finally, just click twice on the bat file. To finish the server, just close the terminal window.



