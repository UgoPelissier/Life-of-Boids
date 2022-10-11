# Project documentation

## Instructions to build the Project before running on Visual Studio 2019
```
cd software-engineering-project/
mkdir build && cd build/
. venv/Script/activate #path of the virtual env where you have installed conan
conan install ..
export PATH="/c/Program Files/CMake/bin":$PATH && cmake --version
cmake \     -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake \     -DCMAKE_GENERATOR_PLATFORM=x64 \     ..
cmake --build . --config Release
```

# Setting up project in Visual Studion 2019
- If there are any lines with errors in file `CMakeLists.txt`, then remove those lines.
- At the center top, You'll find a search box. There type, CMake Settings.
- Set the configuration to `Release` mode
- Change the CMake toolchain file to point to `build/conon_paths.cmake`
- Save the changes.
- Choose the .exe as the item of execution with green play button at the center top.
- Et Voila!


**Note: Make sure to update .gitignore file to avoid pushing large or unecessary files on the repository.