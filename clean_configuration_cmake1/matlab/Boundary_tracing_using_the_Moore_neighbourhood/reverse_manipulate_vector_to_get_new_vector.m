function new_vector = reverse_manipulate_vector_to_get_new_vector(original_vector, height, width)
    num_of_pixels = length(original_vector);
    if (num_of_pixels ~= width*height)
        error('wrong parameters');
    end
    new_vector = zeros(1,num_of_pixels)';
    counter = 0;
    for i=1:height
        for j=1:width
            counter = counter + 1;
            new_index = height * j - i + 1;
            new_index
            new_vector(counter) = original_vector(new_index);
        end
    end
end