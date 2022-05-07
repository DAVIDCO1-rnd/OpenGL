function image_labels = bwlabel_second_scan(image_labels_first_scan, list_identical_labels)
    [rows, cols] = size(image_labels_first_scan);
    num_of_pairs = size(list_identical_labels, 1);
    image_labels = image_labels_first_scan;
    
    for k=1:num_of_pairs
        source_val = list_identical_labels(k, 1);
        dest_val = list_identical_labels(k, 2);
        for i=1:rows
            for j=1:cols
                current_val = image_labels_first_scan(i,j);
                if (current_val == source_val)
                    image_labels(i,j) = dest_val;
                end
            end
        end
    end
end