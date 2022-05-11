function image_labels = my_bwlabel( binary_image, connectivity )
    [height, width] = size(binary_image);
    [image_labels_first_scan, list_identical_labels] = bwlabel_first_scan(binary_image, connectivity);    
    
    
    list_identical_labels_from_file = csvread('pairs.csv');
    
    num_of_labels = length(list_identical_labels);
    for i=1:num_of_labels
        label_matlab = list_identical_labels(i, :);
        label_from_file = list_identical_labels_from_file(i, :);
        if (label_matlab(1) ~= label_from_file(1) || label_matlab(2) ~= label_from_file(2))
            i
            label_matlab
            label_from_file
            fprintf('wrong labels!\n');
        end
    end

    diff_pairs = list_identical_labels - list_identical_labels_from_file;
    sum_abs_diff_pairs = sum(abs(diff_pairs(:)));
    if (sum_abs_diff_pairs > 0)
        error('pairs are wrong');
    end
    
    
    image_labels_first_scan_from_file = csvread('labelsImageFirstScan.csv');
    image_labels_first_scan_from_file = reshape(image_labels_first_scan_from_file, [height, width]);

    diff_image_labels_first_scan = calc_sum_diff_images(image_labels_first_scan, image_labels_first_scan_from_file)
    
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
    
    
    
    unique_vals = unique(image_labels_from_file(:));
    unique_vals(unique_vals==0)=[];
    num_of_unique_vals = length(unique_vals);
    for i=1:num_of_unique_vals
        current_unique_val = unique_vals(i);
        image_labels_from_file(image_labels_from_file==current_unique_val)=i;
    end
    
    diff_image = calc_sum_diff_images(image_labels, image_labels_from_file)
    
    figure;
    subplot(1,2,1);
    imagesc(image_labels);
    title('image labels');
    subplot(1,2,2);
    imagesc(image_labels_from_file);
    title('image labels from file');
end