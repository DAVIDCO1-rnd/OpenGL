function image_labels = bwlabel_second_scan(image_labels_first_scan, list_identical_labels)
    [rows, cols] = size(image_labels_first_scan);
    num_of_pairs = size(list_identical_labels, 1);
    image_labels = image_labels_first_scan;
    if (num_of_pairs == 0)
        %no second scan is needed. image_labels_first_scan is what we need.
        return;
    end
    
%     unique_values_column_2 = unique(list_identical_labels(:,2));
%     sequence_indexes = 1:length(unique_values_column_2);
%     M = containers.Map(unique_values_column_2,sequence_indexes);
    
    
    for k=1:num_of_pairs
        fprintf('second scan:  pair %d out of %d\n',k,num_of_pairs);
        source_val = list_identical_labels(k, 1);
        dest_val = list_identical_labels(k, 2);
        for i=1:rows
            for j=1:cols
                current_val = image_labels_first_scan(i,j);
                if (current_val == source_val)
                    image_labels(i,j) = dest_val;
                end
%                 if (current_val == source_val || current_val == dest_val)
%                     dictionary_key = dest_val;
%                     dictionary_value = M(dictionary_key);
%                     image_labels(i,j) = dictionary_value;
%                 end
            end
        end
    end
    
    unique_values = unique(image_labels(:));
    sorted_unique_values = sort(unique_values);
    sorted_unique_values(1) = []; %removing zero (the backgoround)
    sequence_indexes = 1:length(sorted_unique_values)';
    
%     dict = [sorted_unique_values , sequence_indexes];
    num_of_unique_values = length(sorted_unique_values);
    
    for k=1:num_of_unique_values
        fprintf('third(?) scan:  pair %d out of %d\n',k,num_of_unique_values);
        source_val = sorted_unique_values(k);
        dest_val = sequence_indexes(k);
        for i=1:rows
            for j=1:cols
                current_val = image_labels(i,j);
                if (current_val == source_val)
                    image_labels(i,j) = dest_val;
                end
            end
        end
    end    
    
end