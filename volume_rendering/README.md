# Volume Rendering Using Ray-casting

A basic volume renderer with ray sampler, tri-linear interpolator, Phong shading-based transfer function, from-to-back and back-to-front compositor.

## Dependencies

- Eigen
- stb_image
- [tinycolormap](https://github.com/yuki-koyama/tinycolormap)


## Results

- Trilinear interpolator, back-to-front composition, sample step=0.01

![Volume Rendering Result](results/tri_bw.png)

- Trilinear interpolator, back-to-front composition, sample step=0.1

![Volume Rendering Result](results/tri_bw_s.png)

- More results are placed in the *results/* directory.


## Related Resources

* [Volume Visualization and Volume Rendering Techniques](http://www.cs.unh.edu/~cs880/volvis/Meissner-VolRenderingEGTutorial.pdf)  

  A paper which introduces basic concept and common techniques of volume rendering.  

* [State of the Art in Transfer Functions for Direct Volume Rendering](https://www.researchgate.net/publication/304823671_State_of_the_Art_in_Transfer_Functions_for_Direct_Volume_Rendering)  

  A paper which describes the concept and common approaches of transfer functions.  
