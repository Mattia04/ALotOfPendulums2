# A Lot Of Pendulums 2

![Static Badge](https://img.shields.io/badge/License%20-%20MIT%20-%20green?style=flat-square)

In this project I will simulate the double pendulum to create the double pendulum fractal.

This repository is the continuation of [this repo](https://github.com/Mattia04/a-Lot-of-Pendulums), I have decided to create another repository because to implement GPU acceleration I had to write the code from zero again, so I choose to keep the two separate.

## Requirements

For the `C++` part of this repository you need to have installed on your machine: `Boost`, `OpenCL` and `hdf5`.

For the `python` part you need to have installed: `h4py`, `PIL` and `numpy`. You can find `/scripts/Requirements.txt` to install everything using `venv`.

## Run Locally

Build the program
```commandline
cmake cmake-build-debug/
```

Move to the build directory
```commandline
cd cmake-build-debug/
```

Execute the program, and wait for it to finish. 
```commandline
./ALotOFPendulums2
```

Generate the image.
```commandline
python3 ../scripts/generate_img.py
```

Alternatively if you are using `CLion` you can just import this repository as a project and run the program like you normally do.

### Settings

### Time estimation

There is no run-time estimation of the total computation time. 

But if you want to know how much time it will take:

- Run the program (there is no need to run the python script) using a low number of pixels like: 512 pixels (note that if you use a small number of pixels (total time < 2 seconds) you will get an overestimation of the time).
- Knowing that the program run time is proportional to the numer of pixels squared we can estimate the time $t$ of our wanted simulation using:
$$ t = \varepsilon \frac{N^2}{\overline{N}^2} $$
Where $\varepsilon$ is the computation time you got for the simulation, $N$ is the number of pixels of the test computation and $\overline{N}$ the number of pixels of the image you would like.
- Then running the simulation using $\overline{N}$ pixels it will take approximately the time $t$.

## Examples

## Authors

- [@Mattia04](https://www.github.com/Mattia04)

## Contributing

Contributions are always welcome!