clear;
close all;
clc;

image_folder = '../saved_images';
image_name = 'screenShot.bmp';
image_full_path = fullfile(image_folder, image_name);

circle_color = [255, 255, 255];
models_color = [255, 0, 0];

image = imread(image_full_path);

imageR = image(:,:,1);
imageG = image(:,:,2);
imageB = image(:,:,3);

binary_image = imageR == circle_color(1) & imageG == circle_color(2) & imageB == circle_color(3);

[B,L] = bwboundaries(binary_image);
imshow(label2rgb(L, @jet, [.5 .5 .5]))
hold on
for k = 1:length(B)
   boundary = B{k};
   plot(boundary(:,2), boundary(:,1), 'w', 'LineWidth', 3, 'Color', [1, 0, 0])
end

% figure;
% subplot(1,2,1);
% imshow(binary_image);
% 
% subplot(1,2,2);
% imshow(image);

david = 5;