OGL_GEO: OpenGL Geometry
================================================================================
A class representing vertex Attribute data. You can generate a Geometry
object from a VertexData-Object, so they are obviously connected.

Usage
--------------------------------------------------------------------------------
You need to specify an OpenGL header in as `OFL_GL_HEADER`. for example:

```cpp
	#define OFL_GL_HEADER <glad/glad.h>  
	#define OFL_GL_HEADER <glew.h>
```

This module also uses GLM ... sorry I was too lazy to implement all
these functions, and its not feasible anyway.

API
--------------------------------------------------------------------------------
You can pass the data on construction or vie the method uploadData(...).
The	data is then uploaeded into OpenGL buffer objets and a corresponding
VAO is created.
Call the draw() method to provoke a glDrawElements() call.

