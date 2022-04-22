clear;
close all;
clc;



circle_center = [0, 0];
z_val = 30;


num_of_angles = 3;
angles = linspace(0, 360, num_of_angles+1);
angles = angles(1:end-1);


radius = 90;
min_radius = 10;
num_of_radiuses = 2;
radiuses = linspace(min_radius, radius, num_of_radiuses);




circle_center_x = circle_center(1);
circle_center_y = circle_center(2);

triangles = [];

%plus 1 for the center itself
num_of_vertices = num_of_angles * num_of_radiuses + 1;

vertices = zeros(num_of_vertices, 2);
vertex_index = 0;

for radiusIndex = num_of_radiuses:-1:1
    for angleIndex = 1:num_of_angles
        currentRadius = radiuses(radiusIndex);
        currentRadius1 = min_radius + (radiusIndex - 1) * (radius - min_radius)/(num_of_radiuses - 1);
        
        currentAngle = angles(angleIndex);
        currentAngle1 = (angleIndex-1) * 360/num_of_angles;
        
        current_vertex_x = circle_center_x + currentRadius*cosd(currentAngle);
        current_vertex_y = circle_center_y + currentRadius*sind(currentAngle);
        
        current_vertex = [current_vertex_x, current_vertex_y];
        vertex_index = vertex_index + 1;
        vertices(vertex_index, :) = current_vertex;
    end
end

vertex_index = vertex_index + 1;
vertices(vertex_index, :) = circle_center;

facesIndexes = [];
index = 1;
for radiusIndex = 1:num_of_radiuses-1
    for angleIndex = 1:num_of_angles
        index2 = index+1;
        if index2 == (radiusIndex + 0) * num_of_angles + 1
            index2 = (radiusIndex - 1) * num_of_angles + 1;
        end
        singleFaceIndexes1 = [index, index2, index+num_of_angles];
        index3 = index+num_of_angles+1;
        if index3 == (radiusIndex + 1) * num_of_angles + 1
            index3 = (radiusIndex + 0) * num_of_angles + 1;
        end        
        singleFaceIndexes2 = [index2, index+num_of_angles, index3];
        currentTrianglesIndexes = [singleFaceIndexes1 ; singleFaceIndexes2];
        facesIndexes = [facesIndexes ; currentTrianglesIndexes];
%         fprintf('%d, %d, %d\n',singleFaceIndexes1(1), singleFaceIndexes1(2), singleFaceIndexes1(3));
%         fprintf('%d, %d, %d\n',singleFaceIndexes2(1), singleFaceIndexes2(2), singleFaceIndexes2(3));
%         fprintf('\n');
        index = index + 1;
    end
end

first_index = (num_of_radiuses - 1) * num_of_angles + 1;
index = first_index;
indexOfCenter = num_of_radiuses * num_of_angles + 1;
for angleIndex = 1:num_of_angles
    index2 = index+1;
    if (index2 == indexOfCenter)
        index2 = first_index;
    end    
    singleFaceIndexes = [index, index2, indexOfCenter];
    facesIndexes = [facesIndexes ; singleFaceIndexes];
    index = index + 1;
end



figure;
trisurf(facesIndexes, vertices(:,1), vertices(:,2), zeros(num_of_vertices, 1), 'LineStyle','-', 'EdgeColor' , [0, 0, 0], 'FaceColor', 'none');
hold on;
axis equal;

fileID = fopen('circle.obj','w');


for vertex_index = 1:num_of_vertices
    current_vertex = vertices(vertex_index, :);
    x_val = current_vertex(1);
    y_val = current_vertex(2);
    fprintf(fileID,'v %f %f %f\n', x_val, y_val, z_val);
end

num_of_faces = size(facesIndexes, 1);
num_of_faces1 = num_of_angles * 2 * (num_of_radiuses - 1) + num_of_angles;

fprintf(fileID,'\n\n\n');

for face_index = 1:num_of_faces
    current_face = facesIndexes(face_index, :);
    vertex_index1 = current_face(1) - 1;
    vertex_index2 = current_face(2) - 1;
    vertex_index3 = current_face(3) - 1;
    fprintf(fileID,'f %d %d %d\n', vertex_index1, vertex_index2, vertex_index3);
end
fclose(fileID);



