Boundary Tracing using the Moore Neighbourhood
==============================================
 
Adam H. Aitkenhead
adam.aitkenhead@physics.cr.man.ac.uk 
The Christie NHS Foundation Trust 
17th May 2010
 
2D boundary tracing using the Moore neighbourhood.
 
 
USAGE
=====
 
>>  [listCONTOUR,listNORMALS] = TRACE_MooreNeighbourhood(data2D,pixelFIRST)
takes the logical array <data2D> and traces the boundary of the region of 1s.  The coordinates of the starting pixel are optionally defined by <pixelFIRST>.
 
 
INPUT PARAMETERS
================
 
data2D                -  A 2D logical array of size (L,M)  (1 => Inside the region, 0 => Outside the region)
pixelFIRST (optional) -  A (2x1) array containing the [x;y] coordinates of any one pixel located on the edge of the region to be traced.

OUTPUT PARAMETERS
=================
 
listCONTOUR            - An Nx2 array containing the coordinates of the pixels located around the edge of the region to be traced. The pixels are listed in a clockwise direction around the contour.
listNORMALS (optional) - An Nx2 array containing the (approximate) normal vectors pointing out of the region of interest.  The vectors are listed in a clockwise direction around the contour, and correspond to each pixel listed in <listCONTOUR>.
 
 
EXAMPLE
=======
 
data2D = [0,0,0,0,0,0; 0,1,0,1,0,0; 0,1,1,1,1,0; 0,1,1,1,0,0; 0,1,1,1,1,0; 0,0,0,0,0,0];
[listCONTOUR,listNORMALS] = TRACE_MooreNeighbourhood(data2D);
imagesc(data2D)
colormap('gray')
hold on
  plot(listCONTOUR(:,2),listCONTOUR(:,1),'ro-','LineWidth',2)
  plot([listCONTOUR(:,2),listCONTOUR(:,2)+listNORMALS(:,2)]',[listCONTOUR(:,1),listCONTOUR(:,1)+listNORMALS(:,1)]','b','LineWidth',2)
hold off
 
 
 
NOTES
=====
 
The algorithm ends when the *second* pixel in the loop is revisited, entered from the same direction as it was entered on its first visit.  The second pixel is used rather than the first pixel for two reasons:  1. The first pixel may be encountered more than once while going around the contour;  2. It is possible that the first pixe will never be entered from the same direction as it was on its first visit.
 
The code does not identify any internal holes which exist in the region to be traced.
 
If more than one separate region exists in the image, the code only traces one of these regions.  The user can define which region by choosing an appropriate starting pixel <pixelFIRST>.  Alternatively, the first region encountered by the algorithm will be traced.
 
 
REFERENCES
==========
 
For a guide to contour tracing, including a description of Moore-Neighbour Tracing, see the tutorial by Abeer George Ghuneim at: http://www.imageprocessingplace.com/downloads_V3/root_downloads/tutorials/contour_tracing_Abeer_George_Ghuneim/index.html
 
 
