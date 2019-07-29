# Grime

Repository for the grime language specification, compiler and standard libraries.

## Building

### Dependencies

#### Ubuntu

`$ sudo apt-get install cmake llvm git gcc`

#### Arch

`$ sudo pacman -S cmake llvm git gcc`

### Setup

```
$ git clone https://github.com/Edward-0/grime.git
$ cd grime
$ mkdir build
```

### Make

```
$ cd build
$ cmake ..
$ make
```

### Install

`$ sudo make install`

## Using the Compliler

### Grime to LLVM-IR
`$ cat path/to/your/file.grime | grimec > path/to/your/file.ll`

### LLVM-IR to AT&T Assembly

llc path/to/your/file.ll

### AT&T Assembly to ELF
gcc -o path/to/your/file path/to/your/file.s
