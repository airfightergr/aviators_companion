## AVIATOR's COMPANION

A desktop app with some tools useful to flight sim aviators. Currently only for Linux.

### What can do (still WIP)
1. Unit conversions.
    * Temperature
    * Weights
    * Length
    * Fuel volume and weigh calculation
2. Airports weather information.
    * Metars
    * TAFs
3. Future additions not determined yet.

### Usage

The app is located in the `bin` folder. ~~The app now is only ready to be use in terminal.~~

Add UI based on [cimgui](https://github.com/cimgui/cimgui), a thin c-api wrapper for [Dear ImGui](https://github.com/ocornut/imgui).

Still WIP, though!

To run it from root folder, open it in terminal and run:

```
$ ./bin/aviators_companion`
```
or

```
$ cd bin && ./aviators_companion
```


To build the app, run:

```
$ cmake CmakeLists.txt 
$ make
```

**CAUTION: Use only for Flight Simulation! Do not use it in real aviation!**


