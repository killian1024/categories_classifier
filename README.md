### classifier ###

classifier create a directory tree that represents entries in which symbolic links are used to 
point to other directories and thus classify them. JSON files are used to specify the entries in 
the source directories. For information about how to write them and use the program use
`classifier --help` command.

### Build ###

Use the folowing commands to buil the CMake project.

    cmake .
    cmake --build .

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Also the following tools and libraries are necessary with a version equal or 
highter the specified one:
- GCC 8.1.0
- CMake 3.9
- speed (https://github.com/killian1024/speed)
- boost 1.58
