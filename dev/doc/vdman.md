VDMAN: VertexData Manufacturer
================================================================================
This class provides functionality to create VertexData, the same intuitive
way you might do it in OpenGL1.x. vertex per vertex in a given primitive
mode.  
It can also produce some simple geometric shapes:  

- Plane
- Box
- Cone
- UV-Sphere
- Cylinder
- Disk
- Coordinate system

Usage
--------------------------------------------------------------------------------

This tool creates VertexData, so the ofl_vd oflib is needed.

API
--------------------------------------------------------------------------------
It is very easy, with `begin(PRIMITIVE)` you start to constuct a new
VertexData object. With `color(...)`, `texCoord(...)`, and `normal(...)` you can
modify the data used for these attributes. With a call of `vertex(...)` the
vertex is finished and another is started. The state you set with `color(...)`
etc. is not changed with a call of `vertex(...)`.

Calling `finish()` returns the VertexData object and the Manufactuerer is
ready to construct a new one.



