function result = calc_lookat_glm(position, center, up)
    f = center - position;
    f = f / norm(f);
    u = up / norm(up);
    s = cross(f, u);
    s = s / norm(s);
    u = cross(s, f);

    result = eye(4,4);
    
    result(1,1) = s(1);
    result(2,1) = s(2);
    result(3,1) = s(3);
    result(1,2) = u(1);
    result(2,2) = u(2);
    result(3,2) = u(3);
    result(1,3) = -f(1);
    result(2,3) = -f(2);
    result(3,3) = -f(3);
    result(4,1) = -dot(s, position);
    result(4,2) = -dot(u, position);
    result(4,3) = dot(f, position);   
    
    
    
    
%     result[0][0] = s.x;
%     result[1][0] = s.y;
%     result[2][0] = s.z;
%     result[0][1] = u.x;
%     result[1][1] = u.y;
%     result[2][1] = u.z;
%     result[0][2] =-f.x;
%     result[1][2] =-f.y;
%     result[2][2] =-f.z;
%     result[3][0] =-dot(s, eye);
%     result[3][1] =-dot(u, eye);
%     result[3][2] = dot(f, eye);
end