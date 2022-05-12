function [image_labels, list_identical_labels] = bwlabel_first_scan(binary_image, connectivity)
    %pixels are 0 (background) or 1 (foreground)
    [rows, cols] = size(binary_image);
    image_labels = zeros(rows, cols);
    

    list_identical_labels = [];
    label_counter = 0;
    
    temp = zeros(rows*cols, 1200);
    counter = 0;
    for j=1:cols
        fprintf('col %d out of %d\n',j,cols);
        for i=1:rows
            pixel_val = binary_image(i,j);
            if (pixel_val == 0) %background
                [temp, counter] = update_temp(temp, list_identical_labels, i, j, counter);
                continue;                
            end
            
            if (i==1 && j==1)
                label_counter = label_counter + 1;
                image_labels(i,j) = label_counter;
                [temp, counter] = update_temp(temp, list_identical_labels, i, j, counter);
                continue;
            end
            
            if (i==1 && j>1)
                neighbor1 = image_labels(i,j-1);
                if (neighbor1 > 0)
                    image_labels(i,j) = neighbor1;
                else
                    label_counter = label_counter + 1;
                    image_labels(i,j) = label_counter;                    
                end
                [temp, counter] = update_temp(temp, list_identical_labels, i, j, counter);
                continue;
            end
            
            if (i>1 && j==1)
                neighbor1 = image_labels(i-1,j);
                if (neighbor1 > 0)
                    image_labels(i,j) = neighbor1;
                else
                    label_counter = label_counter + 1;                                       
                    image_labels(i,j) = label_counter;                     
                end
                [temp, counter] = update_temp(temp, list_identical_labels, i, j, counter);
                continue;
            end
            
            if (i>1 && j>1)
                neighbor1 = image_labels(i , j-1);
                neighbor2 = image_labels(i-1 , j);
                if (connectivity == 8)
                    neighbor3 = image_labels(i-1 , j-1);
                end
                
                if (connectivity == 8)
                    neighbors = [neighbor1, neighbor2, neighbor3];
                else
                    neighbors = [neighbor1, neighbor2];
                end
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
                    image_labels(i,j) = label_counter;                    
                elseif (length_unique_sorted_neighbors==1)
                        image_labels(i,j) = unique_sorted_neighbors(1);
                else %there are two labels that are identical (to fix in next scan)
                    image_labels(i,j) = unique_sorted_neighbors(1);
                    identical_labels = [unique_sorted_neighbors(2), unique_sorted_neighbors(1)];                    
                    list_identical_labels = insert_to_list(list_identical_labels, identical_labels, 0);                  
                    if (length_unique_sorted_neighbors == 3 && connectivity == 8)
                        identical_labels = [unique_sorted_neighbors(3), unique_sorted_neighbors(2)];
                        list_identical_labels = insert_to_list(list_identical_labels, identical_labels, 0);
                    end
                end
            end
            
            [temp, counter] = update_temp(temp, list_identical_labels, i, j, counter);            
        end
    end
%     fprintf('reading temp.csv\n');
%     tic
%     temp_from_file = csvread('temp.csv');
%     toc
%     fprintf('finished reading temp.csv\n');
%     
%     num_of_pixels_from_file = size(temp_from_file, 1);
%     
%     for i=1:num_of_pixels_from_file
%         val_matlab = temp(i,:);
%         val_from_file = temp_from_file(i,:);
%         diff_vals = val_matlab - val_from_file;
%         sum_abs_diff_vals = sum(abs(diff_vals(:)));
%         if (sum_abs_diff_vals > 0)
%             fprintf('i = %d\n',i);
%             val_matlab
%             val_from_file
%             david = 5;
%         end
%     end
%     
%     %temp1 = temp(1:20, :);
%     diff_temps = temp_from_file - temp;
%     sum_abs_diff_temps = sum(abs(diff_temps(:)));
%     fprintf('sum_abs_diff_temps = %f\n',sum_abs_diff_temps);
end