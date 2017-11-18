# boxed-in-solver

A* search implementation for the _Boxed In_ game

## Build Instructions - Linux

1. Install cmake: `apt install cmake`
1. Install boost:
```
wget <BOOST-URL>
tar xf <BOOST-ARCHIVE>
cd <BOOST-DIRECTORY>
./bootstrap.sh
```
1. Clone boxed-in-solver source: `git clone <BOXED-IN-SOLVER-URL>`
1. Build
```
cd boxed-in-solver
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=.. ..
make install
```
_Binaries will be in boxed-in-solver/bin directory_


## Running the solver

To run the solver on Boxed In 1, Level 1 data, execute the following from the **boxed-in-solver** directory:

```
bin/solve data/levels/1/01.txt
```

To run the solver on Boxed In 3, Level 7 data, execute the following from the **boxed-in-solver** directory:

```
bin/solve data/levels/3/07.txt
```

