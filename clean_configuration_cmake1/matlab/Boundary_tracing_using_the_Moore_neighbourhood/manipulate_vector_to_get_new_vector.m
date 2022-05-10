function new_vector = manipulate_vector_to_get_new_vector(original_vector, height, width)
    num_of_pixels = length(original_vector);
    if (num_of_pixels ~= width*height)
        error('wrong parameters');
    end
    new_vector = zeros(1,num_of_pixels)';
    counter = 0;
    for i=1:width
        for j=1:height
            counter = counter + 1;
            new_index = width * (height - j) + i;
            new_vector(counter) = original_vector(new_index);
        end
    end
end