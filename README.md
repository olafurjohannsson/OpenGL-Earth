# C++ OpenGL 2D Map Renderer

author: Ólafur Aron Jóhannsson<br>
email: olafurjohannss@gmail.com<br>
website: https://olafuraron.is<br>

C++ application that uses OpenGL to render a map of the world.

Qt6 for the window and OpenGL context. And QML for the UI.

glm for matrix calculations and GeoJSON for map data.

Earcut from mapbox is used for triangulation of the map data.



Vertex shader implements the map projections, supported projections are 

- Mercator
- Orthographic
- Stereographic



![Orthographic](Ortho.png)

![Stereo](Stereo.png)

![Mercator](Mercator.png)




Contact 

olafurjohannss@gmail.com
