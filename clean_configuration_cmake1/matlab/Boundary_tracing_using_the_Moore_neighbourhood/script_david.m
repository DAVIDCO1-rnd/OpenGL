clear;
close all;
clc;

height = 720;
width = 1280;

% height = 4;
% width = 3;

% num_of_pixels = width * height;
% image_col = 3*(1:num_of_pixels);
% image_col_uint8 = uint8(image_col);
% image = reshape(image_col_uint8, [height, width]);
% imwrite(image, 'small_image.bmp');
% image1 = imread('small_image.bmp');


binary_image_from_file = csvread('binaryImage.csv');
binary_image_from_file1 = csvread('binaryImage1.csv');

diff_images = sum(abs(binary_image_from_file - binary_image_from_file1))




binary_image_from_file_matrix_transposed = reshape(binary_image_from_file, [width, height]);
binary_image_from_file_matrix1 = reshape(binary_image_from_file1, [height, width]);

binary_image_from_file_matrix = binary_image_from_file_matrix_transposed';
% binary_image_from_file_matrix1 = binary_image_from_file_matrix_transposed1';

figure;
subplot(1,2,1);
imshow(binary_image_from_file_matrix);
title('binaryImage.csv');
subplot(1,2,2);
imshow(binary_image_from_file_matrix1);
title('binaryImage1.csv');

binary_image_from_file1 = binary_image_from_file_matrix_transposed';
binary_image_from_file2 = binary_image_from_file1(end:-1:1 , :);
binary_image_from_file1_col = binary_image_from_file1(:);
binary_image_from_file2_col = binary_image_from_file2(:);



num_of_pixels = length(binary_image_from_file);
binary_image_from_file_order_changed = zeros(1,num_of_pixels)';

counter = 0;
for i=1:height
    for j=1:width
        counter = counter + 1;
        new_index = height * (width - j) + i;
        binary_image_from_file_order_changed(counter) = binary_image_from_file(new_index);
    end
end

diff1 = sum(abs(binary_image_from_file_order_changed - binary_image_from_file2_col))

david = 5;