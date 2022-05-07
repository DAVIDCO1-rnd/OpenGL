function list_identical_labels = insert_to_list(list_identical_labels, pair_labels)
    if (pair_labels(1) == 19)
        david = 5;
    end
    
    value1 = pair_labels(1);
    value2 = pair_labels(2);
    
    is_pair_in_list = check_if_pair_in_list(pair_labels, list_identical_labels);
    if (is_pair_in_list == false)
        column_index = 1;
        [is_single_value_in_list, dest_val] = check_if_single_value_in_column(value1, list_identical_labels, column_index);    
        if (is_single_value_in_list == true)
            %for example we want to insert (19, 12).
            %the pair (19,12) is not in the list.
            %but the pair (19, 30) is in the list.
            %therefore, we don't insert (19, 12) and now need to check the pair (12, 30).
            new_pair_labels = [value2, dest_val];
            list_identical_labels = insert_to_list(list_identical_labels, new_pair_labels);
        end
        
        
        column_index = 2;
        [is_single_value_in_list, dest_val] = check_if_single_value_in_column(value1, list_identical_labels, column_index);    
        if (is_single_value_in_list == true)
            %for example we want to insert (19, 12).
            %the pair (19,12) is not in the list.
            %but the pair (25, 19zzx) is in the list.
            %therefore, we don't insert (19, 12) and now need to check the pair (12, 30).
            new_pair_labels = [value2, dest_val];
            list_identical_labels = insert_to_list(list_identical_labels, new_pair_labels);
        end        
    end
end