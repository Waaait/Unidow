# Unidow
OpenGL projects of cube/lighting/Shader/Material. There are three branches that displays different simulations.
Users can adjust lamp's position using A/W/S/D/Q/E keys, and move camera using UP/DOWN/RIGHT/LEFT/COMMA/PERIOD keys.

# CUBE in master branch
What I work on is a simulation based on the Canadian cult film called CUBE. Rand() function will generate codes for each cube, and, if any of the number in
 the coordinates is prime power, then the room is tarpped & should be attached with a red picture to indicate it's dangerous. Light will also reflect differently
 when moved to different position.
 
 Note: Textures(pictures) attached are from Internet.
 
 Below is a screenshots when running codes in master branch.

![CUBE](/screenshots/CUBEGIF.gif?raw=true "CUBE")

# CUBE in branch materialCUBE
Slightly different from the master branch result, this branch produce a CUBE that is loaded with materials only (not texture, i.e. picture).

Below is a screenshots when running codes in materialCUBE branch.

![CUBEmaterial](/screenshots/cubeWithMaterials.JPG?raw=true "CUBEmaterial")

# Solar System Simulation specialized in Self-rotation Comparison in branch planet

Again, using cube, apply different textures(pictures) for each individual planet. The trick is that different planets has different self-rotation
- for example, Uranus and Venus rotate clockwise while others are counter-clockwise, Mercury and Venus have exceptionally longer planet rotation time
while Saturn and Neptune rotate quite fast(compare to Earth's rotation).

Note: for demonstration purposes, distance to sun & comparative sizes is not present in final implementation.

Below is a screenshots when running codes in planet branch.

![planetGIF](/screenshots/planetGIF.gif)


 
