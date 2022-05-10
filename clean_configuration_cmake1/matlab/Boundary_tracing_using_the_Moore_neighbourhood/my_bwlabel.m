function image_labels = my_bwlabel( binary_image, connectivity )
    [height, width] = size(binary_image);
    [image_labels_first_scan, list_identical_labels] = bwlabel_first_scan(binary_image, connectivity);    
    
    
    list_identical_labels_from_file = csvread('pairs.csv');
    
    
    image_labels_first_scan_from_file = csvread('labelsImageFirstScan.csv');
    image_labels_first_scan_from_file = reshape(image_labels_first_scan_from_file, [height, width]);

    
    figure;
    subplot(1,2,1);
    imagesc(image_labels_first_scan);
    title('image labels first scan');
    subplot(1,2,2);
    imagesc(image_labels_first_scan_from_file);
    title('image labels first scan from file');
    
    
    image_labels = bwlabel_second_scan(image_labels_first_scan, list_identical_labels); 
    
    image_labels_from_file = csvread('labelsImage.csv');
    image_labels_from_file = reshape(image_labels_from_file, [height, width]);
    
    figure;
    subplot(1,2,1);
    imagesc(image_labels);
    title('image labels');
    subplot(1,2,2);
    imagesc(image_labels_from_file);
    title('image labels from file');
end