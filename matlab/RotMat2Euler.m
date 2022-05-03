function Eul = RotMat2Euler(rotation_matrix)
    R13 = rotation_matrix(1,3);
    R12 = rotation_matrix(1,2);
    R11 = rotation_matrix(1,1);
    R23 = rotation_matrix(2,3);
    R33 = rotation_matrix(3,3);
    if R13 == 1 | R13 == -1
      %special case
      E3 = 0; %set arbitrarily
      dlta = atan2d(R12,R13);
      if R13 == -1
        E2 = pi/2;
        E1 = E3 + dlta;
      else
        E2 = -pi/2;
        E1 = -E3 + dlta;
      end
    else
      E2 = - asind(R13);
      E1 = atan2d(R23/cosd(E2), R33/cosd(E2));
      E3 = atan2d(R12/cosd(E2), R11/cosd(E2));
    end

    Eul = [E1 E2 E3];
end