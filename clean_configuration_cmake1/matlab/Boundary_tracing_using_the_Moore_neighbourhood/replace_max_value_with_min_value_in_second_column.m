function [list_identical_labels, new_min_value] = replace_max_value_with_min_value_in_second_column(list_identical_labels, min_value, max_value)
    num_of_pairs = size(list_identical_labels, 1);
    
    new_min_value = min_value;
    for i=1:num_of_pairs        
        current_val_column1 = list_identical_labels(i,1);
        if (current_val_column1 == min_value)
            new_min_value = list_identical_labels(i,2);
        end
    end    
    
    for i=1:num_of_pairs        
        current_val_column2 = list_identical_labels(i,2);
        if (current_val_column2 == max_value)
            current_val_column1 = list_identical_labels(i,1);
            new_pair = [current_val_column1, new_min_value];
            list_identical_labels(i,:) = new_pair;
        end
    end    
end