clear;
close all;
clc;

height = 720;
width = 1280;
binary_image_from_file = csvread('binaryImage1.csv');

height = 3;
width = 4;
binary_image_from_file = [1:width*height]';

binary_image_from_file1 = manipulate_vector_to_get_new_vector(binary_image_from_file, height, width)
binary_image_from_file2 = reverse_manipulate_vector_to_get_new_vector(binary_image_from_file1, height, width)

image = reshape(binary_image_from_file, [height, width]);



figure;
imshow(image);
title('image');


% 
% 
% 
% 
% % binary_image_from_file1 = csvread('binaryImage1.csv');
% 
% % diff_images = sum(abs(binary_image_from_file - binary_image_from_file1))
% 
% 
% 
% 
% binary_image_from_file_matrix_transposed = reshape(binary_image_from_file, [width, height]);
% binary_image_from_file_matrix1 = reshape(binary_image_from_file1, [height, width]);
% 
% binary_image_from_file_matrix = binary_image_from_file_matrix_transposed';
% % binary_image_from_file_matrix1 = binary_image_from_file_matrix_transposed1';
% 
% figure;
% subplot(1,2,1);
% imshow(binary_image_from_file_matrix);
% title('binaryImage.csv');
% subplot(1,2,2);
% imshow(binary_image_from_file_matrix1);
% title('binaryImage1.csv');
% 
% binary_image_from_file1 = binary_image_from_file_matrix_transposed';
% binary_image_from_file2 = binary_image_from_file1(end:-1:1 , :);
% binary_image_from_file1_col = binary_image_from_file1(:);
% binary_image_from_file2_col = binary_image_from_file2(:);
% 
% 
% 
% 
% 
% 
% 
% diff1 = sum(abs(binary_image_from_file_order_changed - binary_image_from_file2_col))
% 
