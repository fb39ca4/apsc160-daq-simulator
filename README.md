# DAQ Simulator

## What is this?
This a portable simulator for the DAQ hardware module used in APSC 160 at 
University of British Columbia. It runs on Linux (compiled with GCC), OSX (using
 XCode), Windows (Using Visual Studio 2013/15), and potentially other platforms.

## How do I use this?
If you haven't already, head over to
[releases](https://github.com/fb39ca4/apsc160-daq-simulator/releases) and 
download a copy. Add all the .c and .h files to the same directory as your 
source code and setup your makefile/IDE to compile mongoose.c and daqsim.c. 
(Usually done with an "Add files to project..." option in IDEs). Add 
`#include "daqsim.c"` to your program and the DAQ functions can be used in the 
same way as the simulator provided in class. When running the program, the user 
interface should open up in a web browser.

See main.c for an example of how to use this simulator library.

Note for Windows users: You may see a prompt asking to allow your compiled 
program through your firewall, but you do not need to allow it through, as all 
communication takes place on the local computer.

## How does it work?
Calling setupDAQ starts a web server from your program, and opens up a webpage 
in a browser. This page is the GUI, and communicates with the server to recieve 
outputs from the main program, and send it the inputs.
