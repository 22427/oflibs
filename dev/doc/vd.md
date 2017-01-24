VD: Vertex Data
================================================================================
This module constist of two structures the VertexData and VertexDataTools.
The first is a simple, straight forward datastructure to store geometry in
a renderable fashion.
VertexDataTools contain methods to load and store vertex data and some 
simple functions to calculate normals and tangents.

Usage
--------------------------------------------------------------------------------
If you use glm, make sure, that glm.hpp is included before you include vd.h
so that vd uses glm. If you don't use glm (which is fine) vd will use its own
glm compatible, rudimentary mat4 and vec4 implementation.


File-formats
--------------------------------------------------------------------------------
The .vd file format is a simple memory dump of a VertexData Object.
The header is organized in "lines" each of 5 unsigned integers.
The first line contains information about the other lines and the object.
The other lines contain information about the attributes.
The last one contains information about the size of the actual data.

| 0          | 1            | 2       | 3              | 4               |
|------------|--------------|---------|----------------|-----------------|
| VDFF       | #lines-1     | version | #attributes    | primitive type  |
| ATTRIBUTE0 | #elements    | type    | normalized?    | stride in bytes |
| ATTRIBUTE1 | ...          | ...     | ...            | ...             |
| #vertices  | sizeof(data) |#indices | sizeof(indeces)| index type      |

Then comes the data as BLOB
