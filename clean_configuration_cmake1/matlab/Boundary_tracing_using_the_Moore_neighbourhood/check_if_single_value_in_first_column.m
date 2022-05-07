function [is_single_value_in_list, dest_val] = check_if_single_value_in_first_column(source_value, list_pairs)
    dest_val = -1;
    is_single_value_in_list = false;
    num_of_pairs = size(list_pairs, 1);
    
    for i=1:num_of_pairs
        current_val = list_pairs(i,1);
        diff_vals = source_value - current_val;
        abs_diff_vals = abs(diff_vals);
        if (abs_diff_vals == 0)
            is_single_value_in_list = true;
            dest_val = list_pairs(i,2);
            return;
        end
    end
end