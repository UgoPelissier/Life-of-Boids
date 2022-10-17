# Simulation project of a flock of birds

## Description

Boids, “bird-oid objects”, is the combination of simple rules that simulates flock behavior. Boids was originally introduced by computer graphics expert, Craig Reynolds, who also worked on scenes for the original Tron movie from 1982 and Batman Returns in 1992. It consists of three fundamental rules: Cohesion, Alignment, and Separation. Just like many other emergent behaviors, each bird can only register and apply these rules to its immediate neighbors within the limited ranges.

The following explains how each rule affects each bird:
- Separation: steer to avoid crowding local flockmates
- Alignment: steer towards the average heading of local flockmates
- Cohesion: steer to move towards the average position (center of mass) of local flockmates

## Instructions to build the project before running

Download the repository from the main branch and place it into a directory of your choice.

### Visual Studio 2019
```
cd software-engineering-project-main/

mkdir build && cd build/

. venv/Script/activate #Path of the virtual env where you have installed conan

conan install ..

export PATH="/c/Program Files/CMake/bin":$PATH && cmake --version

cmake \
    -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake \
    -DCMAKE_GENERATOR_PLATFORM=x64 \
    ..
    
cmake --build . --config Release
```

### MacOS
```
cd software-engineering-project-main/

mkdir build && cd build/

conan install ..

cmake \
    -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake \
    -DCMAKE_BUILD_TYPE="${MODE}" \
    ..
    
cmake --build .
```

## Setting up the project

### Visual Studion 2019
- If there are any lines with errors in file `CMakeLists.txt`, then remove those lines.
- At the center top, You'll find a search box. There type, CMake Settings.
- Set the configuration to `Release` mode
- Change the CMake toolchain file to point to `build/conon_paths.cmake`
- Save the changes.
- Choose the .exe as the item of execution with green play button at the center top.
- Et Voila!

### CLion (MacOS)
- Open the project in CLion
- Let the default parameters of the Open Project Wizard window and click Ok
- Chose "Life-of-boids" as the target for Run at the center right.
- Et voila!


**Note: Make sure to update .gitignore file to avoid pushing large or unecessary files on the repository.

## Parameters
The parameters of the project are defined in ```config/config.cpp``` and can be changed by the user.

The 3 main parameters that the user can change are:
- The number of birds const ```DEFAULT_NUM_AGENTS```
- The number of predators ```DEFAULT_NUM_PREDATORS```
- The number of obstacles ```DEFAULT_NUM_OBSTACLES```