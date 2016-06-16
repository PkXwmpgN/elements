# Elements
OpenGL ES 2.0 and C++ experiments.

###Contents
* [Experiments](#experiments)
 * [Liquid Simulation](#liquid-simulation)
 * [Light Scattered](#light-scattered)
 * [Strange Attractors](#strange-attractors)
 * [Wind Field](#wind-field)
 * [Flame Simualtion](#flame-simulation)
* [Build](#build)

-----------------------

##Experiments

###Liquid Simulation 
![Liquid Simulation](https://raw.githubusercontent.com/PkXwmpgN/elements/master/screenshots/liquid.jpeg)

The simulation is based upon the article [Particle-based Viscoelastic Fluid Simulation](http://www.ligum.umontreal.ca/Clavet-2005-PVFS/pvfs.pdf).
- Sources experiments/liquid
- Preview into [the YouTube](https://www.youtube.com/watch?v=Z-z4TH40bM4)
- Demo into [the Google Play](https://play.google.com/store/apps/details?id=com.yegorov.alexey.elements.liquid)

###Light Scattered
![Light Scattered](https://raw.githubusercontent.com/PkXwmpgN/elements/master/screenshots/light.jpeg)

The adaptation of a light scattered effect. Based upon [Volumetric Light Scattering as a Post-Process](http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html).
- Sources experiments/light
- Preview into [the YouTube](https://www.youtube.com/watch?v=CSaBSs1KVkA)

###Strange Attractors

![Strange Attractors](https://raw.githubusercontent.com/PkXwmpgN/elements/master/screenshots/strange.jpeg)

The implementation of a dynamic system based on the particles (2^20), called [Strange Attractors](https://en.wikipedia.org/wiki/Attractor). 
- Sources experiments/strange
- Preview into [the YouTube](https://www.youtube.com/watch?v=lx3xy8CakE0)

###Wind Field

![Wind Field preview](https://raw.githubusercontent.com/PkXwmpgN/elements/master/screenshots/air.jpeg)

The implementation of a dynamic system based on the particles (2^20) that handles [Navier-Stokes](http://www.intpowertechcorp.com/GDC03.pdf) and wind Forward-Euler integration.
- Sources experiments/air
- Preview into [the YouTube](https://www.youtube.com/watch?v=n3TOHyJzNDs)

###Flame Simulation

![Fire Simulation preview](https://raw.githubusercontent.com/PkXwmpgN/elements/master/screenshots/fire.jpeg)

The adaptation of a [raymarching](http://iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm) flame effect.
- Sources experiments/fire
- Preview into [the YouTube](https://www.youtube.com/watch?v=eYFEQRwEKm4)
- Demo into [the Google Play](https://play.google.com/store/apps/details?id=com.yegorov.alexey.elements.fire)

##Build

* Install [Android Studio](https://developer.android.com/studio/index.html);
* Open the project elements/platform/android;
* Download the required components;
 * Android SDK Platform;
 * Android Platform-Tools;
 * Android Tools;
 * Android Support Repository.
* Open Project Structure (Menu->File->Project Structure...) and setup Android NDK;
* Run.

###Author
Alexey Yegorov.
If you have an idea, feel free to get in touch with me yegorov.alex@gmail.com
