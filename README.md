# Ray-Cast-Exercise
3D graphics like in the good old days

### What is Ray Cast?
It is one of the simplest way to render a 3D world.

Yes, it has to be a very simple world of vertical, square walls, but it can be drawn with very few lines of code and simple math.
It does not even need vectors or matrices (quite hard to believe, if you are used to more modern ways of doing 3D graphics).

![Sample screeshot 1](https://github.com/stefanos-86/Ray-Cast-Exercise/blob/master/Screenshot1.png "")
![Sample screeshot 2](https://github.com/stefanos-86/Ray-Cast-Exercise/blob/master/Screenshot2.png "")


The results look raw, but the algorithm worked nicely on the PCs available in the early 90s.

[And it changed the world.](https://archive.org/details/msdos_Wolfenstein_3D_1992)

### The Technicalities

In brief, the algorithm scans a world map to find the walls. For each column of pixels of the screen, it follows a (geometrical)
ray from the point of view of the player, through one of the pixel in the column, until it hits a wall. Then is a matter of
finding the distance to said wall, pick from a texture a strip of pixels corresponding from that bit of wall, scale it according to 
the distance and pasting it on the screen.

Refer [the tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/) for more details.
This project only implements the basic wall rendering. The tutorial explains many more options.

The project has 3 parts:
* The RayCast library, the implementation of the ray cast algorithms.
* The RayCast test project, to host the unit tests of RayCast.
* The RayCast UI, that gives an executable.

The code relies on the SDL library to listen to the input, handle the UI window, open images and display the pixels.

The code is as straightforward as the geometric calculations allowed me to make it.
The only "trick" is an application of the [dependency inversion principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle)
that let me test the rendering algorithm. The lower level package, RayCast, depends on a "Canvas" class that offers a method to draw a slice of pixels.
But the implementations are in the higher level packages: the real one that actually draws the pixels (via SDL calls) and a mock one to test.

### Quality and performance.
As usual, I made an effor to ensure that the code works right. I unit-tested the core library, kept the "untestable" UI 
part as simple as possible, verified the error codes...

However, the code is not written with "defensive programming" in mind, nor tested very deeply. It could be made more robust and safer.

I intentionally ignored every performance advice from the Ray Cast tutorial. As of now (2020) it is 24 years old. The "common wisdom" of the time
about using shift instead of division and about caching tables of supposedly expensive trigonometric functions is unlikely to be still valid.
CPUs and compilers went a long way since then.

A bit of profiling suggests that it was a good choice: the bottleneck I found is not at all where the tutorial would suggest. The code is much simpler for it.

### The Artistic side
I was surprised by how easy it was to create a wall texture with a nice retro look.
* With [Paint .NET](https://www.getpaint.net/), I started with an uniform grey square.
* Then traced by hand the cracks between the stones, making sure that strokes on the border ended at the same height (to ensure the texture is seamless).
* After copying on another layer, I applied a relief filter to highlight the craks.
* Added random noise on yet another layer.
* Used a black-to-white gradient and some hand painting to add "dirt" on the top layer.

At the end, I did what I would do with [materials](https://docs.blender.org/manual/en/2.80/editors/shader_editor/index.html), almost without noticing.


### How to compile
*I never tried to move the project on another computer.* But this should work:

* The project is a "solution" for Visual Studio 2019 (Community Edition).
* Get a copy of the [SDL library](https://www.libsdl.org/index.php)
* Download/clone the code.
* Open it in Visual Studio.
* Update the paths in the RayCastUI project to point to your copy of SDL (the project otherwise assumes it is at C:\libs\SDL2-2.0.12)
  * Properties >> C/C++ >> General >> Additional Include Directories
  * Properties >> VC++ Directories >> Library Directories
  

### References materials
I would like to thank the authors of the documents below for gifting their excellent guides to the internet.

* [Lazy Foo's tutorials on the SDL library](https://lazyfoo.net/tutorials/SDL/index.php)
* [Permadi's essay on ray casting](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)


### Where can you go from here?
Ray Casting is a very basic technique. But the principle is very close to what Ray Tracing does. This is a much more powerful techinque.
It's a long way to go, but you can start casting more rays (say, one per pixel, or a few per sample of the image function), calculate how the 
bounce on objects with more complicated mathematical functions to account for the physics of lights and materials, optimize using Monte Carlo algorithms, 
[until the results start to look better...](http://www.pbr-book.org/).
