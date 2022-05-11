function files_full_paths_in_folder = get_folder_files_names(folder_full_path)
    folder_content = dir(folder_full_path);
    folder_content_without_current_and_parent_folder = folder_content;
    folder_content_without_current_and_parent_folder(1:2) = []; %removing '.' and '..'
    num_of_files = length(folder_content_without_current_and_parent_folder);
    
    files_full_paths_in_folder = cell(1,num_of_files);
    for file_index=1:num_of_files
        current_file_data = folder_content_without_current_and_parent_folder(file_index);
        current_file_with_extension = current_file_data.name;
        current_file_full_path = fullfile(folder_full_path, current_file_with_extension);
        files_full_paths_in_folder{file_index} = current_file_full_path;
    end
end