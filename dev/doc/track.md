TRACK : Tracking
================================================================================
This is basically an API to the ART network data stream. The data send by
the ARTDTrack2 is received, and provided in one easy to use class.

API
--------------------------------------------------------------------------------
Just create an TrackingData object. From here on you can ask the objekt of
the current state of all tracked targets.
After calling the method `start()` the object spawns a thread to handle the
communication with the DTrack2 server. All public methods are save to call
any time. You can use the `setTransformation(...)` method to set a
transformation that  will be  applied to all positions and orientations.

There are different methods to access different types of targets:

* `get3ds()`returns all 3DoF-targets found in the last frame.
* `get6d(id)`returns the selected 6DoF-Target*.
* `get6di(id)`returns the selected Dtrack2-Internal-6DoF-Target*.
* `getFlystick(id)` retunrs the selected Flystick*.

*If there is no information a Target object will be returned,
located in (0,0,0). If there are no new informations about a Target the last
knonw information is returned.

