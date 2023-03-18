# Simulation project of a flock of birds

## Description

Boids, “bird-oid objects”, is the combination of simple rules that simulates flock behavior. Boids was originally introduced by computer graphics expert, Craig Reynolds, who also worked on scenes for the original Tron movie from 1982 and Batman Returns in 1992. It consists of three fundamental rules: Cohesion, Alignment, and Separation. Just like many other emergent behaviors, each bird can only register and apply these rules to its immediate neighbors within the limited ranges.

![](extras/life-of-boids.gif)
 
## Building the executable

Download the repository from the main branch. Assuming you are now inside the project's root directory on your computer, you can do the following commands to build the project executable depending on your OS.

### Windows
Create /build directory, build the executable and pass the tests:
```
cd scripts/Windows
bash build.sh
bash test.sh
```

Go to the executable location and launch it:
```
cd ../../build/src
bash life-of-boids
./life-of-boids
```

### MacOS
Create /build directory, build the executable and pass the tests:
```
cd scripts/MacOS
chmod +x build.sh
./build.sh
chmod +x test.sh
./test.sh
```

Go to the executable location and launch it:
```
cd ../../build/src
chmod +x life-of-boids
./life-of-boids
```

## Set up

### Visual Studion 2019
- At the center top, you'll find a search box. There type, CMake Settings.
- Set the configuration to `Release` mode
- Change the CMake toolchain file to point to `build/conon_paths.cmake`
- Save the changes.
- Choose the .exe as the item of execution with green play button at the center top.
- Et Voila!

### CLion (MacOS)
- Open the project in CLion
- Let the default parameters of the Open Project Wizard window and click ```Ok```
- Chose "Life-of-boids" as the target for Run at the center right.
- Et voila!


**Note**: Make sure to update .gitignore file to avoid pushing large or unecessary files on the repository.

## About running
1. The parameters of the project are defined in ```config/config.cpp``` and can be changed by the user.

The 3 main parameters that the user can change are:
- The number of birds const ```DEFAULT_NUM_BIRDS```
- The number of predators ```DEFAULT_NUM_PREDATORS```
- The number of obstacles ```DEFAULT_NUM_OBSTACLES```

Most of the constants defined are declared as `Real` which is a `float`.

2. Compile & Build the Project using CMake and execute `build/src/life-of-boids.exe` 
