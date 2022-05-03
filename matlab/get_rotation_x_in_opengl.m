function rotation = get_rotation_x_in_opengl(theta)
    rotation = [1, 0, 0 ;
                0 , cosd(theta) , -sind(theta) ;
                0 , sind(theta) , cosd(theta)];
end