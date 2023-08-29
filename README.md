# GameEngine_JL

GameEngine_JL is a student project that supports OpenGL and supports basic Game Artificial Intelligence (AI) and a networking TCP structure through Winsock. 
The overall OpenGL is built in Componenet Framework with physics and AI components added to the actors
while there contains another networking branch that contains networking structure without AI.

The AI branch contains a small animation structure that is used to simulate the direction the character is facing. 
(The sprite is original art asset.)

# Game AI
The Game AI contains of three parts: Steering, Pathfinding and Decision Making.

Steering contains multiple behaviours, such as Seek, Arrive, Align algorithm. 

Pathfinding implements a tile system to the scene which is used to perform the graph searching through A* algorithm.

Decision Making contains a state machine that is used to control and transit the character status. 
The actions and transitions are being loaded through XML file in the scene, 
and the animation of the sprite changes according to the state machine for visual representation.

# Networing
This is basic networking structure implemented through Winsock which supports client and server communication which updates the game constantly. 
While the server is sending information constantly, the client only sends information whenever the character moves in order to prevent the lagging or 
glitching happening.


