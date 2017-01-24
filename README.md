What is this?
================================================================================
The oflibs are a set of useful classes and tools dealing with all kinds of 
problems.  
My personal focus is in 3D-visualisation and VR, so most of these deal with 
these topics. OFLIBS stands for "**o**ne **f**ile **lib**rarie**s**". The basic 
idea is to write libraries in a single file, which you just have to include into
your project, and you are ready to go.

I find these very handy for fast prototypes and "experimental" code, which is what 
this collection currently is, so don't expect a well tested, working toolset, but
maybe you find the one or two lines of code you were searching ;).
I'm constantly working with these tools and I am sure they will get better 
over time.

Structure
--------------------------------------------------------------------------------
In "dev/" you find the current parts of the oflibs in their disassembled form, 
which is used to develop the oflibs. There are no finished oflibs in this 
repository but you can create using the "generate_libs.py" script.
Here is a short list of the different modules you can generate libraries from:

- ofl_ogl_geo - A class reperesenting vertex-data on the GPU
- ofl_ogl_state - A state-wrapper imulating the "classic" OpenGL-fixed-function 
  states, including matrix stacks and lighting.
- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading
events. Currently implemented using glfw.
- ofl_ogl_stereo_compositor - A simple way to render in stereo image formats.
- ofl_socket: A class wrapping sockets.
- ofl_stru: Some string utilities. Used by many other oflibs.
- ofl_track - Tracking: An interface to the ART-DTrack2 tracking system.
- olf_vd - Vertex Data: A data structure representing geometry on the client 
  side. This is OpenGL independend. 
- ofl_vdman - Vertex Data MANufacturer: Allows you to create VertexData using
  the old begin(..) ... end() paradigma from fixed-function-GL, but is OpenGL
  independend.
- ofl_vrpv - Virtual Reality Projection and View - A module to genereate view-
  and projection-matrices for VR systems like PowerWalls or CAVEs
- ofl_processing_graph - A simple processing graph interface for parallel
processing of streaming data.

How do oflibs work?
--------------------------------------------------------------------------------
You use the oflib as a regular header, for example:
```c_cpp
	#include <ofl_all.h>
```
Then in one .cpp-file of your project you include the oflib defining
`OFL_IMPLEMENTATION` before:
```c_cpp
	#define OFL_IMPLEMENTATION
	#include <ofl_all.h>
	#undef OFL_IMPLEMENTATION
```
That's it :)

Internally the oflib-header is organized in two parts, the declaration-part 
(that's what you would write into an ordinary header) and the implementation-part 
(that's what you would write into *.cpp files)
Example:
```c_cpp
	#ifndef OFL_ALL_H
	#define OFL_ALL_H
		// ALL DECLARATIONS
	#endif
	#ifdef OFL_IMPLEMENTATION
		// ALL THE IMPLEMENTATIONS
	#endif
```

Where can I get more information?
--------------------------------------------------------------------------------
In dev/doc/ you find a small documatation file for each module. In the 
subdirectory tools/ there are also some more or less simple tools using 
the oflibs.




