VRPV: Virtual Reality Projection & Viewmatrix
================================================================================
The toolkit consists of two main components: Screens and ScreenArrangements.
Screens are the data structures you are interested in. A Screen represents a
rectengular area in world space, on which you want to render. (This could be
one side of a cave, one display in a display array, a power wall, etc.).

A ScreenArrangement is a collection of screens. ScreenArangements can be
loaded from a file, which is handy if you want to change the screen
arragement without recompiling your program.

Usage
--------------------------------------------------------------------------------
If you use glm, make sure, that glm.hpp is included before you include vrpv.h
so that VRPV uses glm. If you don't use glm (which is fine) vrpv uses its own
glm compatible, rudimentary mat4 and vec4 implementation.


API
--------------------------------------------------------------------------------
You can create a ScreenArrangement and load screens by these two lines of
code:

```
	ScreenArrangement sa;
	sa.loadScreens("path/to/screens/file")
```

You can access a Screen using the `getScreen(...)` methods you can either
access it via its name, specified in the screen-file or via its id
(with id in [0,sa.countScreens()]):
```
	Screen* s = sa.getScreen(0); // by id
	Screen*  s = sa.getScreen("screen0"); // by name
```
Now you can calculate projection and  view-matrices for this screen using
the calculate_projection_and_view(...) method:

```
	mat4 projection,view;
	vec4 eye_position;
	float near;
	float far;
	s->calculate_projection_and_view(eye_position, near, far, projection, view);
```
Note: The eye_position must be in the same space as the screen corners in
the screen-file.

File-formats
--------------------------------------------------------------------------------
The screen files contain one line per screen. Each line constist of the
name of the screen, and the position of three corners of the screen:
bottom-left, bottom-right, top-left. You can use vec3 or vec4
(when using vec4 make shure the w-component is 1), however when you saver the
ScreenArrangement, vec4s will be used.
Example:
```
	# <- the '#' marks a line as comment.
	screen0 (0.0,0.0,0.0,1.0) (1.0,0.0,0.0,1.0) (0.0,1.0,0.0,1.0)
	screen1 (0.0,1.0,0.0) (1.0,1.0,0.0) (0.0,2.0,0.0)
```
