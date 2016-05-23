# StkDekrispatorSynthesizer
Port of Dekrispator Synthesizer to Windows, Linux, Mac and Raspberry Pi. Original for STM32F4 is here: https://github.com/MrBlueXav/Dekrispator Using STK for midi and sound playback.

## Build instructions using premake.

**Windows**

Click on build3/vs2010.bat and open build3/vs2010/0MySolution.sln

**Linux, Raspberry Pi and Mac OSX gnu make**

On Linux you may need to install ALSA, pthread:

	sudo apt-get install libasound2-dev
	sudo apt-get install libpthread-stubs0-dev

On Linux, you may try to run the following script terminal:

	./build_and_run.sh

Otherwise follow the steps:

	cd build3

Dependend on your system (Linux 32bit, 64bit, rPI or Mac OSX) use one of the following lines

	./premake4_linux     gmake
	./premake4_linux64   gmake
	./premake4_osx       gmake
	./premake4_rpi       gmake

Then

	cd gmake
	make

**Mac OSX Xcode**
	
Click on build3/xcode4.command or in a terminal window execute
	
	./premake_osx xcode4

## Usage
../../bin/App_Dekrispator_gmake

Dekrispator will start in demo mode, switching different sound waves and settings.
You can plug in a midi keyboard such as KORG nanoKONTROL2 to tune synth parameters 

If you use a midi keyboard you can start playing. It will stop the demo mode and uses the 
last active sound.
