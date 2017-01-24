OGL_GEO: OpenGL Geometry
================================================================================
A class representing vertex Attribute data. You can generate a Geometry
object from a VertexData-Object, so they are obviously connected.

Usage
--------------------------------------------------------------------------------
You need to specify an OpenGL header in as `OFL_GL_HEADER`. 
For example:

```cpp
	#define OFL_GL_HEADER <glad/glad.h>  
	#define OFL_GL_HEADER <glew.h>
```

This module also uses GLM I once started to implement all the matrix vector
classes but it is useless work since GLM works so fine.

API
--------------------------------------------------------------------------------
You can pass the data on construction or via the method uploadData(...).
The	data is then uploaded into OpenGL buffer objects and a corresponding
VAO is created.
Call the draw() method to provoke a glDrawElements() call.


