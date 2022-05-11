function [temp, counter] = update_temp(temp, list_identical_labels, i, j, counter)
    empty_value = 0;
    num_of_pairs = size(list_identical_labels, 1);
    temp_size = 5 + 3*num_of_pairs;            
    temp_current_val = zeros(1,temp_size);
    temp_current_val(1) = i;
    temp_current_val(2) = j;
    temp_current_val(3) = empty_value;
    temp_current_val(4) = length(list_identical_labels);
    temp_current_val(5) = empty_value;         
    for g=1:num_of_pairs
        val1 = list_identical_labels(g, 1);
        val2 = list_identical_labels(g, 2);
        index = 5 + 3*(g-1);
        temp_current_val(index + 1) = val1;
        temp_current_val(index + 2) = val2;
        temp_current_val(index + 3) = empty_value;
    end

    counter = counter + 1;
    temp(counter, 1:temp_size) = temp_current_val;
end