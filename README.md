# Brief intro to libcmpltrtok

[libcmpltrtok](https://github.com/cmpltrtok20/libcmpltrtok) is a collection of the common C routines used in [Cmpltrtok](https://github.com/cmpltrtok20)'s projects, such as:

 - [libtvtsc](https://github.com/cmpltrtok20/libtvtsc): a C translation of the python code for scalar recording of [TVTS (Trackable and Visible Training System)](https://github.com/cmpltrtok20/tvts) in order to let the user uses TVTS in C
 - The author's other C projects that are under development.

Currently, **only Linux platforms** are considered. Maybe the user could run it on other Unix-like platforms, but it is not tested.

# Prerequisites

 - As the Makefile said, with macro -D\_GNU_SOURCE.

 - [libffcall-dev](https://www.gnu.org/software/libffcall/) is needed just for testing code test\_path\_join_many.c, which is optional for normal usage of this library.

# Clone the code

```bash
$ mkdir libmcpltrtok
$ git clone https://github.com/cmpltrtok20/libcmpltrtok.git libmcpltrtok
```


# Modify the Makefile for customization

```bash
$ cd libcmpltrtok
$ nano Makefile
```

At the beginning lines of the Makefile of this project, the user can edit it to modify:

- Will the build be for Debug (DEBUG=1) or Release (DEBUG=0)?
- The installation destinations for the header and the shared lib.

As below:

```makefile
DEBUG=1
DIR_PATH_OF_LIB_H=/usr/local/include
DIR_PATH_OF_LIB_SO=/usr/local/lib
```

The user could run **make check** to check these values after saving the modification to Makefile, as below:

```bash
$ cd libcmpltrtok
$ make check
Is for Debug or Release: Debug
Path to install the header: /usr/local/include/cmpltrtok.h
Path to install the lib: /usr/local/lib/libcmpltrtok.so
```

# Build and install

```bash
$ cd libcmpltrtok
$ make
$ sudo make install
```

Then the shared library **libcmpltrtok.so** will be built and installed with header file **cmpltrtok.h**.

Or the user could just simply copy the cmpltrtok.h and the libcmpltrtok.so to where she wishes to install them and give them the appropriate ownership and file mode after executing "make".

# Uninstall

The user could uninstall them like below:

```bash
$ cd libcmpltrtok
$ sudo make uninstall
```

Or the user could just simply delete the cmpltrtok.h and the libcmpltrtok.so at where she installed them.

# Invoke and compile with libcmpltrtok

The user could use these routines declared in cmpltrtok.h in her C code such as below:

```c
#include "cmpltrtok.h"

unsigned int *perm = cmp_rand_perm(range, seed);

char *base = cmp_path_split(str, dir_buf, MAXLINE);

//... ...
```

Then the user could compile her code like the below:

```bash
$ gcc -c user_code.c -o user_code.o -lcmpltrtok
```

If that the compiler cannot find the cmpltrtok.h and/or that the linker cannot find the libcmpltrtok.so, please specify them like below:

```bash
$ gcc -I/dir/path/of/cmpltrtok_h -c user_code.c -o user_code.o -L/dir/path/of/libcmpltrtok_so -lcmpltrtok
```

# Run with libcmpltrtok

If the user installed the libcmpltrtok.so into a system library location, she probably could directly run her program that depends on the libcmpltrtok.so. If the system complains it cannot find the libcmpltrtok.so, please add the path of the directory of the libcmpltrtok.so to the environment variable LD\_LIBRARY\_PATH like below:

```bash
$ export LD_LIBRARY_PATH="/dir/path/of/libcmpltrtok_so${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
$ ./the_exec
```

# About the author

Hi, my name is [**Yunpeng Pei**](https://github.com/cmpltrtok20), an AI engineer working in the City of Beijing, China. I developed [these projects](https://github.com/cmpltrtok20?tab=repositories) in my spare time just for interest. I will be very happy if you find them to be very little help to the community. I must say sorry that they are far away from perfect and their document is not finished yet, because of my limited ability and limited resources on these projects; although I have tried my best. You could contact me freely by email to my personal email **belarc@163.com**.

