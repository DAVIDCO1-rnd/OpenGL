function image_labels = my_bwlabel( binary_image, connectivity )
    [image_labels_first_scan, list_identical_labels] = bwlabel_first_scan(binary_image);
    list_identical_labels
    unique_labels = unique(list_identical_labels(:,2))
    
    figure;
    imagesc(image_labels_first_scan);
    
    image_labels = bwlabel_second_scan(image_labels_first_scan, list_identical_labels);
    unique_vals = unique(image_labels(:)); 
    
    figure;
    imagesc(image_labels);    
end