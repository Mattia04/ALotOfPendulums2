# A Lot Of Pendulums 2

![Static Badge](https://img.shields.io/badge/License%20-%20MIT%20-%20green?style=flat-square)

In this project I will simulate the double pendulum to create the double pendulum fractal.

This repository is the continuation of [this repo](https://github.com/Mattia04/a-Lot-of-Pendulums), I have decided to create another repository because to implement GPU acceleration I had to write the code from zero again, so I choose to keep the two separate.

After playing around with some simulations i estimated that this repository is $\approx 130$ faster than the previous one. 😱

## Requirements

For the `C++` part of this repository you need to have installed on your machine: `Boost`, `OpenCL` and `hdf5`.

For the `python` part you need to have installed: `h5py`, `PIL` and `numpy`. You can find `/scripts/Requirements.txt` to install everything using `venv`.

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

#### High resolution (8192x8192) image:

<img src="Images/img8192.png">

#### Huge resolution image (12566x12566) 

I literally don't have enough memory on my machine to create a bigger image than this (this "problem" is due to the fact that I did not optimize memory usage).

`Computation time: 893.1 seconds`

<img src="Images/img12566.png">

### Zooms
#### Leaf 1
Stability region located in: $\theta_1 \in (-1.816, -1.718)$, $\theta_2 \in (-2.049, -1.963)$ radians.

<img src="Images/imgLeaf4096.png">

#### Leaf 2 

Stability region located in $\theta_1 \in (-1.982, -1.702)$, $\theta_2 \in (-2.142, -1.862)$ radians.

<img src="Images/imgLeaf2.png">

#### Border
Stability region located in $\theta_1 \in (-\pi, -2.902)$, $\theta_2 \in (-0.342, -0.102)$ radians.

This one is peculiar because it is located near the border, so it has a lot of potential energy but it still does not flip after 100 seconds.
<img src="Images/imgUpper4096.png">

## Authors

- [@Mattia04](https://www.github.com/Mattia04)

## Contributing

Contributions are always welcome!