clear;
close all;
clc;

image_folder = '../../saved_images';
image_name = 'screenShot.bmp';
image_full_path = fullfile(image_folder, image_name);

circle_color = [255, 255, 255];
models_color = [255, 0, 0];

image = imread(image_full_path);
% imshow(image);
[height, width, ~] = size(image);

imageR = image(:,:,1);
imageG = image(:,:,2);
imageB = image(:,:,3);

binary_image = imageR == circle_color(1) & imageG == circle_color(2) & imageB == circle_color(3);
binary_image_from_file = csvread('binaryImage1.csv');
binary_image_from_file_image = reshape(binary_image_from_file, [height, width]);

% figure;
% subplot(1,2,1);
% imshow(binary_image);
% title('image from matlab');
% subplot(1,2,2);
% imshow(binary_image_from_file_image);
% title('image from c++');



% riceImage = imread('rice.png');
% 
% threshold = 150;
% riceImage(riceImage>threshold)=255;
% riceImage(riceImage<=threshold)=0;
% % imshow(riceImage);
% 
% binary_image = riceImage;

% width = 10;
% height = 10;
% binary_image = zeros(width, height);
% binary_image(2:2,2:4) = 1;
% binary_image(2:2,7:9) = 1;
% binary_image(3:3, :) = 1;
% imshow(binary_image);

tic
connectivity = 4;
image_labels = my_bwlabel( binary_image, connectivity);
david_time = toc;


unique_labels = unique(image_labels(:));

% CC = bwconncomp1 (binary_image, 8);
% [labeledImage, numberOfObject] = bwlabel(binary_image);

% tic
% labeledImage = bwlabel1(binary_image);
% internet_time = toc;

fprintf('david_times = %f\n',david_time);
% fprintf('internet_time = %f\n',internet_time);


unique_labeledImage = unique(image_labels(:));
unique_labeledImage(1)=[]; %remove zero (background)
numberOfObject = length(unique_labeledImage);
% labeledImage1  = detect(binary_image)
figure('units','normalized','outerposition',[0 0 1 1]);
subplot(1,2,1);
imagesc(image_labels);
axis equal;

% boundary1 = csvread('david.csv');
% boundary2 = boundary1;
% yvals = boundary1(:,2);
% min_y = min(yvals)
% max_y = max(yvals)
% boundary2(:,2) = height - boundary1(:,2);

subplot(1,2,2);
imshow(binary_image);
hold on;
for i=1:numberOfObject
%     current_val = unique_labeledImage(i);
%     [tempR,tempC] = find(image_labels==current_val);
    [tempR,tempC] = find(image_labels==i);
    pixelFIRST = [tempR(1);tempC(1)];
    [boundary,varargout] = TRACE_MooreNeighbourhood(binary_image, pixelFIRST);
    plot(boundary(:,2), boundary(:,1), 'w', 'LineWidth', 3, 'Color', [0, 1, 0]);
end

