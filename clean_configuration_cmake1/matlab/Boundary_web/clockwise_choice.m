function clockwiseids = clockwise_choice(Ps)
%first point (cv) transferred to origin
%so fis other points wrt origin
%2nd point is pv, we find clockwise position with respect to line pv-cv

dsx = Ps(2:end,1) - Ps(1,1);
dsy = Ps(2:end,2) - Ps(1,2);

%[baseQ baseT] = find_quadrant(dsx(1,1),dsy(1,1));

for p = 1:size(dsx,1)
    QT(p,:) = find_quadrant(dsx(p,1),dsy(p,1));
end

[Ts ids] = sort(QT(:,2),'descend');

idp = find(ids == 1);%position of pv
clockwiseids = [ids(idp+1:end,1);ids(1:idp-1,1)] - 1;% -1 to restore to indices in bv
end


function out = find_quadrant(dx,dy)
%quadrangle & theta wrt positive x-axis
t = atand(dy/dx);
if dx > 0 && dy > 0
    Q = 1;
    theta = t;%do nothing for theta
elseif dx < 0 && dy > 0
    Q = 2;
    theta = 180+t; %t is -ve: 90>theta<180 
elseif dx < 0 && dy < 0
    Q = 3;
    theta = 180+t;%t is +ve: 180>theta<270 
elseif dx > 0 && dy < 0
    Q = 4;
    theta = 360+t;%t is -ve: 180>theta<270
elseif dx == 0 && dy > 0% between Q1 & Q2
    %merge with Q1 above?
    Q = 1.5; %on +ve y-axis
    theta = t;%90 degrees
elseif dx == 0 && dy < 0% between Q3 & Q4
    %merge with Q3 above?
    Q = 3.5; %on -ve y-axis
    theta = 180-t;%270 degrees
elseif dx > 0 && dy == 0% between Q4 & Q1
    %merge with Q4 above?
    Q = 0.5;%on +ve x-axis
    theta = t;%0 or 360 degrees
elseif dx < 0 && dy == 0% between Q2 & Q3
    %merge with Q2 above?
    Q = 2.5;%on -ve x-axis
    theta = 180+t;%180 degrees
end
out = [Q theta];
end