OGL_STATE: OpenGL State
================================================================================
This is a state tracking class, which also provides some stock shaders and
functions.

###State tracking:
There is a matrix stack for modelview- and projection-matrix, and a state
foreight light sources. Just like in OpenGL 1.x

###Stock Shaders:
There are shaders for texturing, phong shading and distance field alpha
handling.

Usage
--------------------------------------------------------------------------------

You need to specify an OpenGL header in as OFL_GL_HEADER. for example:  

```
	#define OFL_GL_HEADER <glad/glad.h>  
	#define OFL_GL_HEADER <glew.h>
```

This module also uses GLM ... sorry I was too lazy to implement all
these functions, and its not feasible anyway.


API
--------------------------------------------------------------------------------

###Matrix-Stacks:
There are two stacks. `PROJECITON` and `MODEL_VIEW`. You can change which is
modified using the `matrixMode(...)` method.
with `pushMatrix()` and `popMatrix()` you can either push a copy of the top-most
matrix onto the stack or pop the top-most matrix from the stack.
All other matrix modifying methods effect the top-most(current)
matrix of the selected stack:  

```
void loadIdentity();//   sets the matrix as identity  
void translate(..);//    translates the current matrix  
void rotate(...);//      rotates the current matrix  
void scale(...);//       scales the current matrix  
void lookAt(...);//      multiplies the current matrix with a lookAt-matix  
void ortho(...);//       multiplies the current matrix with an ortho-matix  
void frustum(...);//     multiplies the current matrix with a frustum-matix  
void perspective(...);// multiplies the current matrix with a persp.-matix  
```
###Lights:
You can set Light parameters using the `setLight*(...)` methods.

###Stock-shaders:
You can select different stock shaders by calling `enable(...)` or `disable(...)`
fordifferent features.
The features are:

* Texturing: Basic color mapping
* Lighting: Basic per fragment lighting
* DFAlpha: Iterpreting the Alpha channel as distance field.
* NonStockShader: Use the set non stock shader. If this feature is
enabled all uniforms and attributes will still be set, but your shader
will be used.

