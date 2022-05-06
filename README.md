# Getting this code to work

Installation:

1. Install Xcode


2. Install llvm compiler


```bash
brew install llvm
```

3. Install OpenMP

```bash
brew install libomp
```

4. Clone the repo

```
    git clone https://github.com/larsgeb/msl-tutorial.git
```

5. Build the different files

## M1 people

```console
cd msl-tutorial

# Compile Tutorial 01
/opt/homebrew/opt/llvm/bin/clang++ -O2 -std=c++17 \
                                   -stdlib=libc++ \
                                   -g src/tutorial01.cpp \
                                   -o build/tutorial01.x

# Compile Tutorial 02
/opt/homebrew/opt/llvm/bin/clang++ -O2 -std=c++17 \
                                   -L/opt/homebrew/opt/libomp/lib -fopenmp \
                                   -stdlib=libc++ \
                                   -g src/tutorial02.cpp \
                                   -o build/tutorial02.x

# Compile Tutorial 03
xcrun -sdk macosx metal -c src/ops.metal -o ops.air
xcrun -sdk macosx metallib ops.air -o ops.metallib
/opt/homebrew/opt/llvm/bin/clang++ -O2 -std=c++17 \
                                   -L/opt/homebrew/opt/libomp/lib -fopenmp \
                                   -I./metal-cpp \
                                   -fno-objc-arc \
                                   -framework Metal -framework Foundation -framework MetalKit \
                                   -stdlib=libc++ \
                                   -g src/tutorial03.cpp src/MetalOperations.cpp \
                                   -o build/tutorial03.x 
```


## M1 people

```console
cd msl-tutorial

# Compile Tutorial 01
clang++  -O2 -std=c++17 \
         -g src/tutorial01.cpp \
         -o build/tutorial01.x

# Compile Tutorial 02, no openmp :(
clang++  -O2 -std=c++17 \
         -g src/tutorial02.cpp \
         -o build/tutorial02.x

# Compile Tutorial 03
# ... cry
```


6. Run the built applications

```
./build/tutorial01.x   

export OMP_NUM_THREADS=4
./build/tutorial02.x

export OMP_NUM_THREADS=4
./build/tutorial03.x
```

# Useful links

[Getting started with Metal CPP](https://developer.apple.com/metal/cpp/)

