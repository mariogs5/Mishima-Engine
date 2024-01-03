# Mishima Engine

Mishima Engine is a humble engine created for the Game Engine subject.

### Repository Link:   [Mishima Engine](https://github.com/mariogs5/Mishima-Engine)

## Members:
- Mario Garcia Sutil **GitHub:**   [mariogs5](https://github.com/mariogs5)
- Víctor Gil Espallargues  **GitHub:**   [Ludo-pixel](https://github.com/Ludo-pixel)

## New v0.2 Features:

- Models and Textures drawn properly with Bounding Box for each mesh.
- GameObjects creation, transformation and hierarchy.
- Game Viewport and Scene ViewPort (With an independent camera for each one).
- Cameras with de folowing characteristics:
    - All cameras reworked as components.
    - Implementation of Frustrum Culling (Can be enabled or disabled in the Game camera inspector).
    - Mouse Picking capable of selecting a mesh through the Scene window and displaying it in the inspector menu.
    - Same controls for the Scene Camera as shown below except for a bug with the rotation orbiting an object that has been disabled due to errors with the rotation while looking at the object.

## v0.1 Features:

- ImGui window & docking implementation.
- Main Bar with the following characteristics: 
    - File -> Option to shut down the engine
    - Configuration window:
        - Check Pc resources in real time.
        - Modify the Engine window settings.
        - Check the hardware usage.
        - About section with all the Engine information.
        - Modify all the Open Gl settings.
    - Console Window to see all the Log information.
    - Models:
        - Primitives: you can create primitives.
        - Delete Models: Delete all the Models in the Scene. 
- Drag & Drop FBX Models.
- Load an initial texture without UV's.
- Unity Like camera.

## Camera Controls:

    -While Right clicking, “WASD” fps-like movement and free look enabled.
    -Mouse wheel zoom in and out.
    -Alt+Left click to orbit the object.
    -F to focus the camera around the geometry and reset position.
    -Holding SHIFT duplicates movement speed.

