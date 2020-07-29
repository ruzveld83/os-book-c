A repo for some of the programming exercises and projects 
of [Operating System Concepts](http://os-book.org) book (2018 edition).

The repo includes only C-language assignments. Some of them can be built and run 
on any POSIX-compliant system. Also there are some Linux kernel modules written 
here which are compiled only on Linux.

### How to run
#### 1. From command line
By now each program resides in its own directory. So simply navigate to one, 
compile directory contents and run.

#### 2. Using CLion
CLion uses CMake build tool which is configured by `CMakeLists.txt` file. 
Each executable unit has a separate `add_executable()` statement in it which 
automatically provide distinct Run Configurations in CLion.

OS-specific exercises (Linux kernel modules) are not meant to be compiled and run 
from inside the repo. However they also have their Run Configuration. It is made so 
to enable editing with convenient IDE features like syntax/errors highlighting 
and jumps to sources.

In order to conveniently edit Linux kernel code its sources should be copied to the 
local machine and added by `include_directories()` statements (example is already there).
