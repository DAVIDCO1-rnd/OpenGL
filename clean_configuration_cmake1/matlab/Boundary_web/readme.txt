Please refer the paper: M. Awrangjeb, "Using point cloud data to identify,
trace, and regularize the outlines of buildings" International Journal of Remote Sensing, Volume 37, Issue 3, February 2016, pages 551-579
Open access at: http://www.tandfonline.com/doi/pdf/10.1080/01431161.2015.1131868

How to run the code:
====================
1. To generate sample shapes (for example, A-shape) from point cloud run shape_gen.m 

2. To find outwer boundary of an input point set, run find_delaunay_boundary03_fig1.m
For example, for the A-shape run as
[bids E Ne] = find_delaunay_boundary03_fig1(XA,1.3);

3. If the angle threshold is not to apply, run find_delaunay_boundary03_noAngleThreshold.m
For example, for the A-shape run as
[bids E Ne] = find_delaunay_boundary03_noAngleThreshold(XA,1.3);  

4. To find the hole in an input point set, run find_delaunay_boundary03_hole_fig.m
For example, for the A-shape run as
[bids E Ner] = find_delaunay_boundary03_hole_fig(XA,1.3);  

5. An example to check split shape and boundary extraction algorithms, run ShapeSplit.m; 
this code example works for A-shape only here, you should be able to adapt the code for any other shapes of your interest.

Note: the algorithms should work for any pointcloud data. The only input parameter is dmax, which is maximum point-to-point distance in the input 
point cloud. For the generated shapes (by shape_gen.m) this value is in between 1.0 to 1.5 pixels. So, the second parameter in the above examples, we use 1.3 pixels.

The algorithms have been extensively tested against nemerous LIDAR point cloud data. For example, for the sample data in LIDAR_sample_01_adjusted.txt
this second parameter is 0.2 (metre).

The algorithms are also capable of extracting multiple boundaries for more than one object in one single input data set. In that case, the distance between two objects
should be at least 2 times the maximum point-to-point distance in the input point cloud.

