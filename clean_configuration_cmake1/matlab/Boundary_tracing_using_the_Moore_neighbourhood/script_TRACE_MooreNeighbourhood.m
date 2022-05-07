clear;
close all;
clc;

image_folder = '../../saved_images';
image_name = 'screenShot.bmp';
image_full_path = fullfile(image_folder, image_name);

circle_color = [255, 255, 255];
models_color = [255, 0, 0];

image = imread(image_full_path);
[height, width, ~] = size(image);

imageR = image(:,:,1);
imageG = image(:,:,2);
imageB = image(:,:,3);

binary_image = imageR == circle_color(1) & imageG == circle_color(2) & imageB == circle_color(3);

% width = 100;
% height = 100;
% binary_image = zeros(width, height);
% binary_image(2:4,2:4) = 1;
% binary_image(5:6, 5:6) = 1;
% binary_image(8:13, 5:6) = 1;

% imshow(binary_image);

image_labels = my_bwlabel( binary_image, 8);
unique_labels = unique(image_labels(:));
figure;
imagesc(image_labels);
CC = bwconncomp1 (binary_image, 8);
% [labeledImage, numberOfObject] = bwlabel(binary_image);
labeledImage = bwlabel1(binary_image);


unique_labeledImage = unique(labeledImage(:));
numberOfObject = length(unique_labeledImage) - 1;
% labeledImage1  = detect(binary_image)
figure;
imagesc(labeledImage);

boundary1 = csvread('david.csv');
boundary2 = boundary1;
yvals = boundary1(:,2);
min_y = min(yvals)
max_y = max(yvals)
% boundary2(:,2) = height - boundary1(:,2);

imshow(binary_image);
hold on;
for i=1:numberOfObject
    [tempR,tempC] = find(labeledImage==i);
    pixelFIRST = [tempR(1);tempC(1)];
    [boundary,varargout] = TRACE_MooreNeighbourhood(binary_image, pixelFIRST);
    plot(boundary(:,2), boundary(:,1), 'w', 'LineWidth', 3, 'Color', [0, 1, 0]);
end

david = 5;