VMATH : vector-maths
================================================================================
This is a set of classes containing the needed vector-math for the ofl tools.
It is ment to be an backup data exchange format, if there is no GLM in your
project.
Note: Do not use this vector and matrix class. Use GLM or something else!
GLM is also header only, and does - imho - a great job. These classes are
only here, so you do not have to use GLM. There are two classes with a very
limited set of methods: vec4 and mat4 a vector of 4 floats and a 4x4-matrix.

Usage
--------------------------------------------------------------------------------
As mentioned above you should not use these classes for anything but
exchanging data with oflibs. To enshure, that GLM is used (if you use it in
your project) include `<glm/glm.hpp>` before you include this file.

