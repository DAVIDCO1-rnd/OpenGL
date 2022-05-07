function is_in_list = check_if_pair_in_list(pair_value, list_pairs)
    is_in_list = false;
    num_of_pairs = size(list_pairs, 1);
    
    for i=1:num_of_pairs
        current_pair = list_pairs(i,:);
        diff_pairs = pair_value - current_pair;
        abs_diff_pairs = abs(diff_pairs);
        if (abs_diff_pairs == 0)
            is_in_list = true;
            return;
        end
    end
end