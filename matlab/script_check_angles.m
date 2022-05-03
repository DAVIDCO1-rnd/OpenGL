clear;
close all;
clc;

theta_x = 119;
theta_y = -72;
theta_z = 15;

rotation_x = get_rotation_x_in_opengl(-theta_x)
rotation_y = get_rotation_y_in_opengl(-theta_y)
rotation_z = get_rotation_z_in_opengl(-theta_z);

rotation = rotation_x * rotation_y * rotation_z


eye = [0, 0, 0]; %camera position
orientation = [7, 1, -1];
at = eye + orientation; %where the camera is looking at
up = [1, 1, 8]; %the up direction of the camera
lookat1 = calc_lookat(eye, at, up)
lookat = calc_lookat_glm(eye, at, up)

Eul = RotMat2Euler(rotation);
lookat_angles = RotMat2Euler(lookat(1:3, 1:3))

lookat_x = lookat_angles(1)
lookat_y = lookat_angles(2)
lookat_z = lookat_angles(3)

lookat_rotation_x = get_rotation_x_in_opengl(-lookat_x)
lookat_rotation_y = get_rotation_y_in_opengl(-lookat_y)
lookat_rotation_z = get_rotation_z_in_opengl(-lookat_z);

lookat_rotation = lookat_rotation_x * lookat_rotation_y * lookat_rotation_z
lookat
% diff_lookat_mats = lookat_rotation - lookat
david = 5

