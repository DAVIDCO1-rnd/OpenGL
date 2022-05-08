function list_identical_labels = insert_to_list(list_identical_labels, pair_labels, counter)  
    value1 = pair_labels(1);
    value2 = pair_labels(2);
    
    if (value1 < value2)
        min_value = value1;
        max_value = value2;
    else
        min_value = value2;
        max_value = value1;        
    end
    
    %to keep a convention, the second value in each pair in the list is
    %always smaller than the first value
    pair_labels = [max_value, min_value]; 
    
    is_pair_in_list = check_if_pair_in_list(pair_labels, list_identical_labels);
    if (is_pair_in_list == true)
        return;
    end
    
    column_index = 2;
    [is_single_value_in_list1, dest_val1] = check_if_single_value_in_column(max_value, list_identical_labels, column_index);
    [is_single_value_in_list2, dest_val2] = check_if_single_value_in_column(min_value, list_identical_labels, column_index);
    
%     if (is_single_value_in_list1 == true && is_single_value_in_list2 == true)
%         %for example we want to insert (19, 12)
%         %the pair (19,12) is not in the list.
%         %but the pairs (30, 19) and also (25, 12) are both in the list
%         %since we now see that 19 and 12 are the same we should replace all
%         %the occurances of 19 in the second column to 12 (this can also be
%         %vise-versa but to make it consistent, we'll always choose the
%         %smaller number of the two).
%         list_identical_labels = replace_max_value_with_min_value_in_second_column(list_identical_labels, min_value, max_value);
%         new_pair_labels = [max_value, min_value]; %new_pair_labels = pair_labels
%         list_identical_labels = [list_identical_labels ; new_pair_labels];        
%         return;
%     end
    
    if (is_single_value_in_list1 == true)
        %for example we want to insert (19, 12).
        %the pair (19,12) is not in the list.
        %but the pair (25, 19) is in the list.
        %therefore, we need to replace all occurances of 19 in the second
        %column to 12.
        [list_identical_labels, min_value] = replace_max_value_with_min_value_in_second_column(list_identical_labels, min_value, max_value);
        new_pair_labels = [max_value, min_value]; %new_pair_labels = pair_labels
        list_identical_labels = [list_identical_labels ; new_pair_labels];         
        return;
    end
            
    if (is_single_value_in_list2 == true)
        %for example we want to insert (19, 12).
        %the pair (19,12) is not in the list.
        %but the pair (30, 12) is in the list.
        %therefore, we insert (19, 12)
        new_pair_labels = [max_value, min_value]; %new_pair_labels = pair_labels
        list_identical_labels = [list_identical_labels ; new_pair_labels];
        return;
    end    
    

    column_index = 1;
    [is_single_value_in_list3, dest_val3] = check_if_single_value_in_column(max_value, list_identical_labels, column_index);    
    if (is_single_value_in_list3 == true)
        %for example we want to insert (19, 12).
        %the pair (19,12) is not in the list.
        %if the pair (19, 8) is in the list.
        %we need to replace all the occurances of 19 or 12 in the second
        %column to 8.
        %and if the pair (19, 16) is in the list.
        %we need to replace all the occurances of 19 or 16 in the second
        %column to 12.
        %this is in order to make it consistent that the second column is
        %always smaller than the first column.
        if (dest_val3 < min_value)
            [list_identical_labels, dest_val3] = replace_max_value_with_min_value_in_second_column(list_identical_labels, dest_val3, min_value);
            [list_identical_labels, dest_val3] = replace_max_value_with_min_value_in_second_column(list_identical_labels, dest_val3, max_value);
            new_pair_labels1 = [min_value, dest_val3];
            is_pair_in_list1 = check_if_pair_in_list(new_pair_labels1, list_identical_labels);
            if (is_pair_in_list1 == false)               
                list_identical_labels = [list_identical_labels ; new_pair_labels1];
            end
            new_pair_labels2 = [max_value, dest_val3];
            is_pair_in_list2 = check_if_pair_in_list(new_pair_labels2, list_identical_labels);
            if (is_pair_in_list2 == false)                 
                list_identical_labels = [list_identical_labels ; new_pair_labels2];
            end                        
        else
            [list_identical_labels, min_value] = replace_max_value_with_min_value_in_second_column(list_identical_labels, min_value, dest_val3);
            [list_identical_labels, min_value] = replace_max_value_with_min_value_in_second_column(list_identical_labels, min_value, max_value);
            new_pair_labels1 = [max_value, min_value]; %new_pair_labels = pair_labels
            is_pair_in_list1 = check_if_pair_in_list(new_pair_labels1, list_identical_labels);
            if (is_pair_in_list1 == false)               
                list_identical_labels = [list_identical_labels ; new_pair_labels1];
            end            
            new_pair_labels2 = [dest_val3, min_value];
            is_pair_in_list2 = check_if_pair_in_list(new_pair_labels2, list_identical_labels);
            if (is_pair_in_list2 == false)                
                list_identical_labels = [list_identical_labels ; new_pair_labels2];
            end              
        end
        
        return;
    end

    [is_single_value_in_list4, dest_val4] = check_if_single_value_in_column(min_value, list_identical_labels, column_index);    
    if (is_single_value_in_list4 == true)
        %for example we want to insert (19, 12).
        %the pair (19,12) is not in the list.
        %but the pair (12, 8) is in the list.
        %therefore, we need to replace all occurances of 12 and 19 in the
        %second column to 8.
        %dest_val4 must be smaller than min_value
        [list_identical_labels, dest_val4] = replace_max_value_with_min_value_in_second_column(list_identical_labels, dest_val4, min_value);
        [list_identical_labels, dest_val4] = replace_max_value_with_min_value_in_second_column(list_identical_labels, dest_val4, max_value);
        new_pair_labels1 = [min_value, dest_val4];
        is_pair_in_list1 = check_if_pair_in_list(new_pair_labels1, list_identical_labels);
        if (is_pair_in_list1 == false)            
            list_identical_labels = [list_identical_labels ; new_pair_labels1];
        end
        new_pair_labels2 = [max_value, dest_val4];
        is_pair_in_list2 = check_if_pair_in_list(new_pair_labels2, list_identical_labels);
        if (is_pair_in_list2 == false)            
            list_identical_labels = [list_identical_labels ; new_pair_labels2];
        end  
        return;
    end  



    if (is_single_value_in_list1 == false && is_single_value_in_list2 == false && is_single_value_in_list3 == false && is_single_value_in_list4 == false)
        list_identical_labels = [list_identical_labels ; pair_labels];
    end

end