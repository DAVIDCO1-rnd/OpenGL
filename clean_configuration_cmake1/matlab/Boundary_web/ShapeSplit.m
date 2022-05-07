function ShapeSplit(XA)
%XA generated from shape_gen() function

dmax = 1.3;
figure, plot(XA(:,1), XA(:,2), '.k', 'markersize', 10); hold on;
xlim([-30.5,-9.5]); ylim([-32,+32]);%for A-shape

rect1 = [-30, -30; -20, -30; -20,  5; -30, 5; -30, -30];
rect2 = [-20, -30; -10, -30; -10,  5; -20, 5; -20, -30];
rect3 = [-20,   5; -10,   5; -10, 30; -20, 30; -20,  5];
rect4 = [-30,   5; -20,   5; -20, 30; -30, 30; -30,  5];

%plot(rect1(:,1), rect1(:,2), '-r'); hold on;
%plot(rect2(:,1), rect2(:,2), '-c'); hold on;
%plot(rect3(:,1), rect3(:,2), '-m'); hold on;
%plot(rect4(:,1), rect4(:,2), '-g'); hold on;

IN1 = inpolygon(XA(:,1), XA(:,2), rect1(:,1), rect1(:,2));
XA1 = XA(IN1,:);

IN2 = inpolygon(XA(:,1), XA(:,2), rect2(:,1), rect2(:,2));
XA2 = XA(IN2,:);

IN3 = inpolygon(XA(:,1), XA(:,2), rect3(:,1), rect3(:,2));
XA3 = XA(IN3,:);

IN4 = inpolygon(XA(:,1), XA(:,2), rect4(:,1), rect4(:,2));
XA4 = XA(IN4,:);

plot(XA1(:,1), XA1(:,2), 'om'); hold on;
plot(XA2(:,1), XA2(:,2), 'og'); hold on;
plot(XA3(:,1), XA3(:,2), 'oc'); hold on;
plot(XA4(:,1), XA4(:,2), 'ob'); hold off;

[bids1 E Ne] = find_delaunay_boundary03_fig1(XA1,dmax);
[bids2 E Ne] = find_delaunay_boundary03_fig1(XA2,dmax);
[bids3 E Ne] = find_delaunay_boundary03_fig1(XA3,2.5);
[bids4 E Ne] = find_delaunay_boundary03_fig1(XA4,dmax);

if bids1{1}(1,1) == bids1{1}(end,1)
    bids1{1} = bids1{1}(1:end-1,:);
end
if bids2{1}(1,1) == bids2{1}(end,1)
    bids2{1} = bids2{1}(1:end-1,:);
end
if bids3{1}(1,1) == bids3{1}(end,1)
    bids3{1} = bids3{1}(1:end-1,:);
end
if bids4{1}(1,1) == bids4{1}(end,1)
    bids4{1} = bids4{1}(1:end-1,:);
end

figure, plot(XA(:,1), XA(:,2), '.k', 'markersize', 10); hold on;
xlim([-30.5,-9.5]); ylim([-32,+32]);%for A-shape

bs1 = XA1(bids1{1},:);
bs2 = XA2(bids2{1},:);
bs3 = XA3(bids3{1},:);
bs4 = XA4(bids4{1},:);

plot([bs1(:,1); bs1(1,1)], [bs1(:,2); bs1(1,2)], '-m'); hold on;
plot([bs2(:,1); bs2(1,1)], [bs2(:,2); bs2(1,2)], '-g'); hold on;
plot([bs3(:,1); bs3(1,1)], [bs3(:,2); bs3(1,2)], '-c'); hold on;
plot([bs4(:,1); bs4(1,1)], [bs4(:,2); bs4(1,2)], '-b'); hold off;

bsp = [bs1; bs2; bs3; bs4];
[bids E Ne] = find_delaunay_boundary03_fig1(bsp,dmax);
figure, plot(bsp(:,1), bsp(:,2), '.k', 'markersize', 10); hold on;
xlim([-30.5,-9.5]); ylim([-32,+32]);%for A-shape
bspb = bsp(bids{1},:);
plot([bspb(:,1); bspb(1,1)], [bspb(:,2); bspb(1,2)], '-m'); hold off;
figure;

Plns{1} = bs1;
Plns{2} = bs2;
Plns{3} = bs3;
Plns{4} = bs4;
figure,
[bndry bndryin] = delaunay_boundary02_fig(bsp,dmax,Plns);

bspb = bsp(bndry{1},:);
bspbi = bsp(bndryin{1}{1},:);

figure, plot(bsp(:,1), bsp(:,2), '.k', 'markersize', 10); hold on;
xlim([-30.5,-9.5]); ylim([-32,+32]);%for A-shape
plot([bspb(:,1); bspb(1,1)], [bspb(:,2); bspb(1,2)], '-m'); hold on;
plot([bspbi(:,1); bspbi(1,1)], [bspbi(:,2); bspbi(1,2)], '-m'); hold off;
end