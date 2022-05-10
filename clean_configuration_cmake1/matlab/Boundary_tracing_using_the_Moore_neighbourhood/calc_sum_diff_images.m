function sum_abs_diff_images = calc_sum_diff_images(image1, image2)
    diff_images = image1(:) - image2(:);
    abs_diff_images = abs(diff_images);
    sum_abs_diff_images = sum(abs_diff_images);
    fprintf('sum_abs_diff_images = %f\n',sum_abs_diff_images);
end