Mesa Optimizer Demo
===================

A short FreeGLUT program to test the GLSL-compiler of Mesa. The purpose of this is only to be test data for the NIR mesa compiler.


Dependencies
============

GLEW and FreeGLUT.


Running
=======

In order to run the project with your home compiled mesa, run:

```
export LIBGL_DRIVERS_PATH=~/source/mesa/lib
LIBGL_DEBUG=verbose ./mesa-demo
```

If the output mentions that its trying your local mesa, you're all good.
