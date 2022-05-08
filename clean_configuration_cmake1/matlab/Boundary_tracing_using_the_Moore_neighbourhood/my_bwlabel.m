function image_labels = my_bwlabel( binary_image, connectivity )
    [image_labels_first_scan, list_identical_labels] = bwlabel_first_scan(binary_image);    
    image_labels = bwlabel_second_scan(image_labels_first_scan, list_identical_labels);  
end