function image_labels = my_bwlabel( binary_image, connectivity )
    [image_labels_first_scan, list_identical_labels] = bwlabel_first_scan(binary_image, connectivity);    
    
    image_labels_first_scan_from_file = csvread('labelsImageFirstScan.csv');
%     image_labels_first_scan_from_file_upside_down = image_labels_first_scan_from_file(end:-1:1,:);
%     unique_vals = unique(list_identical_labels(:,2))

%     diff_images = image_labels_first_scan_from_file - image_labels_first_scan;
%     abs_diff_images = abs(diff_images(:));
%     sum_abs_diff_images = sum(abs_diff_images)
    
%     figure;
%     subplot(1,2,1);
%     imagesc(image_labels_first_scan);
%     subplot(1,2,2);
%     imagesc(image_labels_first_scan_from_file_upside_down);
    
    
    image_labels = bwlabel_second_scan(image_labels_first_scan, list_identical_labels); 
    
%     image_labels_from_file = csvread('labelsImage.csv');
%     image_labels_from_file_upside_down = image_labels_from_file(end:-1:1, :);
%     figure;
%     subplot(1,2,1);
%     imagesc(image_labels);
%     subplot(1,2,2);
%     imagesc(image_labels_from_file_upside_down);
end