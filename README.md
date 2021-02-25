# Laplace Equation Simulation

The simulation is done using successive averages. This is the result for a very low resolution grid.

<div align='center'>
   <img src="laplace.gif" align='center'/>
</div>

## Usage
The code can be accelerated using OpenMP. Resolution of grid can be changed as macros. Compile using

```bash
g++ laplace_omp.cpp -fopenmp -O3 -I /usr/include/python2.7 -lpython2.7
```

