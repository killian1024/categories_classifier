### classifier ###

classifier is a simple C++ program whose propose is classify folders using json files for
specify the categories and then create symbolic links to point the entries in the categories 
directories. For more information use `./classifier --help` command.

### Build ###

Use the folowing commands to buil the CMake project.

    cmake .
    cmake --build .

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Also the following libraries are necessary:
- speed (https://github.com/killian1024/speed)
- boost 1.58
