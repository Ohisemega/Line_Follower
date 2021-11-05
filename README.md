# Line_Follower
As part of my robotics coursework, I developed a line follower software for a Poloulou 3Pi+ robot. The robot follows a black line placed on  a surface and recalibrates itself during the process.
Compile this using the Arduino IDE, and the Arduino leonardo board.
The algorithm follows a weighted measure from the IR sensors, integrated  with a PID controller to smoothen the motion.
The PID controller variables could be changed to obtain smoother motion if desired.
The goHome kinematics could definitely be improved upon.
