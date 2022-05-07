function [is_single_value_in_list, dest_val] = check_if_single_value_in_column(source_value, list_pairs, column_index)
    dest_val = -1;
    is_single_value_in_list = false;
    num_of_pairs = size(list_pairs, 1);
    
    if (column_index == 1)
        dest_index = 2;
    else
        dest_index = 1;
    end
    
    for i=1:num_of_pairs
        current_val = list_pairs(i,column_index);
        diff_vals = source_value - current_val;
        abs_diff_vals = abs(diff_vals);
        if (abs_diff_vals == 0)
            is_single_value_in_list = true;
            dest_val = list_pairs(i,dest_index);
            return;
        end
    end
end