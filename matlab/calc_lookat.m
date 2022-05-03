function lookAtMat = calc_lookat(eye, at, up)

  zaxis = at - eye;    
  xaxis = cross(zaxis, up);
  yaxis = cross(xaxis, zaxis);
  
  unit_zaxis = zaxis / norm(zaxis);
  unit_xaxis = xaxis / norm(xaxis);
  unit_yaxis = yaxis / norm(yaxis);

  unit_zaxis = -unit_zaxis;
  
  lookAtMat = zeros(3,3);
  lookAtMat(1,:) = unit_xaxis;
  lookAtMat(2,:) = unit_yaxis;
  lookAtMat(3,:) = unit_zaxis;
end
