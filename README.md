A repo for some of the programming exercises and projects of [Operating System Concepts](http://os-book.org) book (2018 edition).

The repo includes C programs that can be run on mac OS.

### How to run
#### 1. From command line
By now each program resides in its own directory. So simply navigate to one, compile directory contents and run.

#### 2. Using CLion
CLion uses CMake build tool which is configured by `CMakeLists.txt` file. It is not possible to compile the whole repo as it contains multiple `main()` implementations. So you must decide which one you want to run. Put a path to a C source file with a `main()` method to `add_executable` and you're good to go. An example is already there.