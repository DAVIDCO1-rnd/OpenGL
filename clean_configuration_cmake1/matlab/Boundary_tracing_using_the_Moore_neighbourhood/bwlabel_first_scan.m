function [image_labels, list_identical_labels] = bwlabel_first_scan(binary_image)
    %pixels are 0 (background) or 1 (foreground)
    [rows, cols] = size(binary_image);
    image_labels = zeros(rows, cols);
    

    list_identical_labels = [];
    label_counter = 0;
    for i=1:rows
        for j=1:cols
            if (j==735 && i==343)
                david = 6;
            end             
            pixel_val = binary_image(i,j);
            if (pixel_val == 0) %background
                continue;
            end
            
            if (i==1 && j==1)
                label_counter = label_counter + 1;
                image_labels(i,j) = label_counter;
            end
            
            if (i==1 && j>1)
                neighbor1 = image_labels(i,j-1);
                if (neighbor1 > 0)
                    image_labels(i,j) = neighbor1;
                else
                    label_counter = label_counter + 1;
                    image_labels(i,j) = label_counter;                    
                end
                if (j==735 && i==343)
                    david = 6;
                end                 
            end
            
            if (i>1 && j==1)
                neighbor1 = image_labels(i-1,j);
                if (neighbor1 > 0)
                    image_labels(i,j) = neighbor1;
                else
                    label_counter = label_counter + 1;
                    if (label_counter == 188)
                        david = 5;
                    end 
                    
                    if (j==735 && i==343)
                        david = 6;
                    end                    
                    image_labels(i,j) = label_counter;                     
                end
            end
            
            if (i>1 && j>1)
                neighbor1 = image_labels(i , j-1);
                neighbor2 = image_labels(i-1 , j);
                neighbor3 = image_labels(i-1 , j-1);
                
                if (j==735 && i==343)
                    david = 6;
                end
                
                neighbors = [neighbor1, neighbor2, neighbor3];
                [sorted_neighbours_vals, sorted_neighbors_indexes] = sort(neighbors);
                unique_neigbors = unique(neighbors);
                unique_sorted_neighbors = sort(unique_neigbors);
                if (unique_sorted_neighbors(1) == 0)
                    %remove 0 from neighbors
                    unique_sorted_neighbors(1) = [];
                end
                length_unique_sorted_neighbors = length(unique_sorted_neighbors);
                    
                %if (neighbor1 == 0 && neighbor2 == 0 && neighbor3 == 0)
                if (length_unique_sorted_neighbors == 0) %all neighbors are background
                    label_counter = label_counter + 1;
                    if (label_counter == 189)
                        david = 5;
                    end                    
                    image_labels(i,j) = label_counter;                    
                elseif (length_unique_sorted_neighbors==1)
                        image_labels(i,j) = unique_sorted_neighbors(1);
                else %there are two labels that are identical (to fix in next scan)
                    if (label_counter == 189)
                        david = 5;
                    end
                    image_labels(i,j) = unique_sorted_neighbors(1);
                    dest_val = unique_sorted_neighbors(1);
                    identical_labels = [unique_sorted_neighbors(2), dest_val];
                    is_in_list = check_if_pair_in_list(identical_labels, list_identical_labels);
                    if (is_in_list == false)
                        [is_single_value_in_list, dest_val] = check_if_single_value_in_first_column(unique_sorted_neighbors(1), list_identical_labels);
                        if (is_single_value_in_list == true)
                            
                            identical_labels = [unique_sorted_neighbors(2), dest_val];                        
                            is_in_list = check_if_pair_in_list(identical_labels, list_identical_labels);
                            if (is_in_list == false)
                                list_identical_labels = [list_identical_labels ; identical_labels];
                            end
                            
                            identical_labels = [unique_sorted_neighbors(1), dest_val];                        
                            is_in_list = check_if_pair_in_list(identical_labels, list_identical_labels);
                            if (is_in_list == false)
                                list_identical_labels = [list_identical_labels ; identical_labels];
                            end                            
                        else
                            [is_single_value_in_list, dest_val] = check_if_single_value_in_first_column(unique_sorted_neighbors(2), list_identical_labels);
                            if (is_single_value_in_list == true)
                                identical_labels = [unique_sorted_neighbors(1), dest_val];
                                list_identical_labels = [list_identical_labels ; identical_labels];
                            else
                                list_identical_labels = [list_identical_labels ; identical_labels];
                            end                            
                        end
                         
                    end
                    if (length_unique_sorted_neighbors == 3)
                        dest_val = unique_sorted_neighbors(2);
                        identical_labels = [unique_sorted_neighbors(3), dest_val];
                        is_in_list = check_if_pair_in_list(identical_labels, list_identical_labels);
                        if (is_in_list == false)
                            [is_single_value_in_list, dest_val] = check_if_single_value_in_first_column(unique_sorted_neighbors(2), list_identical_labels);
                            if (is_single_value_in_list == true)
                                identical_labels = [unique_sorted_neighbors(3), dest_val];                            
                                is_in_list = check_if_pair_in_list(identical_labels, list_identical_labels);
                                if (is_in_list == false)
                                    list_identical_labels = [list_identical_labels ; identical_labels];
                                end
                                
                                identical_labels = [unique_sorted_neighbors(2), dest_val];                            
                                is_in_list = check_if_pair_in_list(identical_labels, list_identical_labels);
                                if (is_in_list == false)
                                    list_identical_labels = [list_identical_labels ; identical_labels];
                                end                                
                            end
                        else
                            [is_single_value_in_list, dest_val] = check_if_single_value_in_first_column(unique_sorted_neighbors(3), list_identical_labels);
                            if (is_single_value_in_list == true)
                                identical_labels = [unique_sorted_neighbors(2), dest_val];
                                list_identical_labels = [list_identical_labels ; identical_labels];
                            else                            
                                list_identical_labels = [list_identical_labels ; identical_labels];
                            end
                        end
                    end
                end
            end            
        end
    end
end