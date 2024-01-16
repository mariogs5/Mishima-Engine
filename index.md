# Mishima Engine

Mishima Engine is a humble game engine created for the Game Engine subject from the Videogame Design and Development degree
(CITM UPC) thaught and supervised by [Miquel Suau](https://github.com/MayKoder).

<p align="center">
  <img src="https://cdn.discordapp.com/attachments/529313201444225037/1169034298364538880/41VApAg2qpL.png?ex=65b03808&is=659dc308&hm=949389bf1d26b6486ff474a20569871851dada216b9224da80e9f1b82322e373&" alt="MishimaEngineLogo" width="400" height="400"/>
</p>

# Team Members:

### _**Mario Garcia Sutil**_

RESUMEN PERSONAL
  
* **GitHub:** [mariogs5](https://github.com/mariogs5)
<p align="left">
  <img src="https://cdn.discordapp.com/attachments/951095100707049522/1196887263221665902/IMG-20230613-WA0016.jpg?ex=65b94320&is=65a6ce20&hm=9ab452ccad9be70b8c05bd96a3b2d261febd5509a8e7e7d20ebf64e66de74b2c&" alt="MishimaEngineLogo" width="300" height="400"/>
</p>

### _**Víctor Gil Espallargas**_

RESUMEN PERSONAL

* **GitHub:** [Ludo-pixel](https://github.com/Ludo-pixel)
<p align="left">
  <img src="https://cdn.discordapp.com/attachments/529313201444225037/1196889892383371385/IMG-20231118-WA0012.jpg?ex=65b94593&is=65a6d093&hm=6b277832796098078833de3c7782d42e83b82d845229d8a1e30f59fa5a0f1886&" alt="MishimaEngineLogo" width="300" height="400"/>
</p>

# Main Core Sub-Systems & Features

- Models and Textures drawn properly with Bounding Box for each mesh.
- GameObjects creation, transformation and hierarchy.
- Game Viewport and Scene ViewPort (With an independent camera for each one).
- Cameras with de folowing characteristics:
    - All cameras reworked as components.
    - Implementation of Frustrum Culling (Can be enabled or disabled in the Game camera inspector).
    - Mouse Picking capable of selecting a mesh through the Scene window and displaying it in the inspector menu.
    - Same controls for the Scene Camera as shown below except for a bug with the rotation orbiting an object that has been disabled due to errors with the rotation while looking at the object.

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

# High-Level System: Audio System

- Wwise Project Implementation.
- Module Audio to control listeners and audio sources.
- New GameObjects Components implemented:
  - Component Listener: is the component that manages which GameObject receives sound.
  - Component AudioSource: is the component that manages which GameObject emits sound.

**IMPORTANT NOTE:** *Due to several problems the implementation of Wwise in the project has given errors that we have not been able to solve. For this reason the executable of the v1.0 release is the same of the v0.2 release. But if you download the source code you can visualize the implementation of Wwise that we have done within our possibilities.*

# Repository and Releases

### Repository Link: [Mishima Engine](https://github.com/mariogs5/Mishima-Engine)

### Last Release Link: [v0.3 - Audio System]()

# Workflow Video

<iframe width="560" height="315" src="https://youtu.be/8z52u_QCgUU" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

# License

MIT License

Copyright (c) 2023 Mario Garcia Sutil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
