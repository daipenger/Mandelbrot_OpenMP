# Mandelbrot_OpenMP

A program for producing actually quite beatiful images of the Mandelbrot set. Double precision implementation, parallelized using OpenMP. I implemented it somewhere around 2018.

## Command line parameters
`xmin, xmax, ymin, ymax, #Iterations, width, height`

or

`xmiddle, ymiddle, radius, #Iterations, width, height (radius is scaled up on the axis with the larger portion of the aspect ratio)`

Example of an interesting region: `./mandelbrotAVX2 -0.64 -0.5 0.57 0.71 1000 2000 2000`

## Possible improvements

- Could make use of the x-axis symmetry
- Implement Mariani-Silver algorithm
- Implement with CUDA kernel
