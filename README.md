Robot Artist
================

The goal of this project is to make something unexpected beautiful with robots. :)  The goal of this project is to build a painting program that allows users to create artwork in the virtual world, then control a connect robot arm to bring that artwork into the physical world.  Users can add simple shapes such as lines, rectangles, and fill regions or import images, via file or webcam, to bring content into the program.  A user can then individually select commands to have a robot arm paint each component.  Additionally a webcam is used to monitor the robot arm's progress and make adjustments to make the physical world painting as needed.  This project is ongoing.

## Required Libraries ##
- Qt version 5.6 in the default install directory (`c:\Qt`)  
-  Cyton Epsilon viewer libraries in the default install directory (`C:\Program Files (x86)\Robai\Cyton Epsilon 1500 Viewer_4.0.0-20150629`)  
- Boost version 1.60.0 at `c:\libraries\boost_1_60_0`  
- OpenCV version 3.1 at `c:\libraries\opencv`  

### Qt Requirements ###
Install both `MSVC2015` and `MSVC2015_64` 
### Cyton Epsilon Viewer ###
The Cyton libraries are currently built in -32 bit as provided by Robai. This section will be updated if/when Robai sends us -64 bit compiled libraries.  
### Building Boost ###
Boost is built in the following manner (for building 32-bit along-side 64-bit):  
1. Build the standard 64-bit configuration    
	1. Navigate to `C:\libraries\boost_1_60_0` in Command Prompt (sorry Git Bash users :[ )   
	2. Enter the following two commands:  
		1. `> bootstrap`  
		2. `>b2 --build-type=complete address-model=64 --with-system`
3. Rename the `lib` folder in `C:\libraries\boost_1_60_0\stage` to `lib_64`  
4. Build the 32-bit configuration  
	1. `>b2 --build-type=complete address-model=32 --with-system`  
### Building OpenCV ###
TODO: Describe using CMAKE for this process. 