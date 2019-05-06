
# Todo

## OpenGL stuff
- [ ] integrate model class
- [ ] make a BaseApp class (do we want that? see below)
- [ ] make a "CAD" camera

- [x] make glm a submodule
- [x] make a first 3d app

## Other
- [ ] get code from comp-fab exercises and merge


# Resources

https://www.ics.com/blog/fixed-function-modern-opengl-part-1-4

Glitter:
https://github.com/Polytonic/Glitter


# Thoughts 

## about BaseApp class
option 1: 
    make a BaseApp class, or rather a 2d and 3d version of it.
    virtual methods that can be overriden

option 2: 
    don't make a base app and provide all the functionality (rendering, scene interaction, HUD gui, ...) as callbacks or just functions

## example apps

> 2d apps:
- rb sim app 2d
- comp-fab-getting-started
- all other comp-fab exercises

> 3d apps:
- obj viewer:
    + load objs
    + click objs, translate and rotate meshes
    + group meshes to models
- robot model viewer, and motion plan playback
    + read rbs! maybe converter to new json format?
