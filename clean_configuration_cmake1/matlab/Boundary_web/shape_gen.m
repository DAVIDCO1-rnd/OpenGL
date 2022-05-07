function [XA XM Xrh Xt Xloop] = shape_gen()
%make shapes
%random points chosen for shapes at about 0.4m distance

%fp = fopen('first_sample_01_FD40cm.txt','r');%samples points
fp = fopen('LIDAR_sample_01_adjusted.txt','r');%original points
X1=fscanf(fp,'%f %f %f', [3 Inf]);
P=X1';
fclose(fp);

[A, R, bbox] = geotiffread('sample_01.tif'); %original image
%change grid space

%FlightDist = 0.4;%for sampled points
FlightDist = 0.2;%for original points

Rd = R;
Rd(2,1) = FlightDist;
Rd(1,2) = -FlightDist;
 
y = (P(:,1)*Rd(1,2) - Rd(3,1)*Rd(1,2) + Rd(3,2)*Rd(1,1) - P(:,2)*Rd(1,1))/(Rd(2,1)*Rd(1,2)-Rd(2,2)*Rd(1,1));
x = (P(:,2) - Rd(3,2) - y*Rd(2,2))/Rd(1,2);

X = [x y];
[szx szy szz] = size(A);

%top-left and bottom-right points
TL = [1 1 1]*R;
BR = [szx szy 1]*R;

rows = double(ceil(abs(BR(1,2)-TL(1,2))/FlightDist));
cols = double(ceil(abs(BR(1,1)-TL(1,1))/FlightDist));

CP = [rows/2 cols/2];
Xc = [X(:,1)-CP(1,1) X(:,2)-CP(1,2)];

figure;
subplot(3,2,1); plot(Xc(:,1), Xc(:,2), '.k', 'markersize', 10);

%Noisy shape
Ao = [-20,-30; -30,-30; -30,40; -20,40; -10,4; 0,40; 10,40; 10,-30; 0,-30; 0,15; -10,4; -20,15];%outside points 
AoPol = [Ao; Ao(1,:)];
INo = inpolygon(Xc(:,1), Xc(:,2), AoPol(:,1), AoPol(:,2));
Xa = Xc(INo,:);

%Ai = [-20,15; -22.5,-3; -17.5,-3];
%AiPol = [Ai; Ai(1,:)];
%INi = inpolygon(Xa(:,1), Xa(:,2), AiPol(:,1), AiPol(:,2));
%XA = Xa(~INi,:);

subplot(3,2,2); plot(Xa(:,1), Xa(:,2), '.k', 'markersize', 10);

fp = fopen('Noisy-shape_ori.txt','w');
for i = 1:size(Xa,1)
    fprintf(fp,'%f\t%f\n',Xa(i,1), Xa(i,2));
end
fclose(fp);
Xloop = Xa;

%Rec-shape-with-hole
Ao = [20,-40; 55,-40; 55,40; 20,40; 20,1; 25,1; 25,20; 37.5,35; 50,20; 50,-20; 37.5,-35; 25,-20; 25,-1.5; 20,-1.5];%outside points
%Ao = [20,-40; 60,-40; 60,40; 20,40; 20,0.5; 25,20; 50,20; 50,-20; 25,-20; 20,-0.5];%outside points
AoPol = [Ao; Ao(1,:)];
INo = inpolygon(Xc(:,1), Xc(:,2), AoPol(:,1), AoPol(:,2));
Xa = Xc(INo,:);

%Ai = [-20,15; -22.5,-3; -17.5,-3];
%AiPol = [Ai; Ai(1,:)];
%INi = inpolygon(Xa(:,1), Xa(:,2), AiPol(:,1), AiPol(:,2));
%XA = Xa(~INi,:);

subplot(3,2,3); plot(Xa(:,1), Xa(:,2), '.k', 'markersize', 10);

fp = fopen('RectHole-shape_ori.txt','w');
for i = 1:size(Xa,1)
    fprintf(fp,'%f\t%f\n',Xa(i,1), Xa(i,2));
end
fclose(fp);
Xrh = Xa;

%M-shape
Ao = [-20,-30; -30,-30; -30,40; -27,40; -10,15; 7,40; 10,40; 10,-30; 0,-30; 0,15; -10,5; -20,15];%outside points 
AoPol = [Ao; Ao(1,:)];
INo = inpolygon(Xc(:,1), Xc(:,2), AoPol(:,1), AoPol(:,2));
Xa = Xc(INo,:);

%Ai = [-20,15; -22.5,-3; -17.5,-3];
%AiPol = [Ai; Ai(1,:)];
%INi = inpolygon(Xa(:,1), Xa(:,2), AiPol(:,1), AiPol(:,2));
%XA = Xa(~INi,:);

subplot(3,2,4); plot(Xa(:,1), Xa(:,2), '.k', 'markersize', 10);

fp = fopen('M-shape_ori.txt','w');
for i = 1:size(Xa,1)
    fprintf(fp,'%f\t%f\n',Xa(i,1), Xa(i,2));
end
fclose(fp);
XM = Xa;

%A-shape
Ao = [-21.5,-10; -18.5,-10; -15,-30; -10,-30; -19,30; -22,30;  -30,-30; -25,-30];%outside points 
AoPol = [Ao; Ao(1,:)];
INo = inpolygon(Xc(:,1), Xc(:,2), AoPol(:,1), AoPol(:,2));
Xa = Xc(INo,:);

Ai = [-20,15; -22.5,-3; -17.5,-3];
AiPol = [Ai; Ai(1,:)];
INi = inpolygon(Xa(:,1), Xa(:,2), AiPol(:,1), AiPol(:,2));
XA = Xa(~INi,:);

subplot(3,2,5); plot(XA(:,1), XA(:,2), '.k', 'markersize', 10);

fp = fopen('A-shape_ori.txt','w');
for i = 1:size(XA,1)
    fprintf(fp,'%f\t%f\n',XA(i,1), XA(i,2));
end
fclose(fp);

%MA-shape
Xn = [XA(:,1)+45 XA(:,2)];
Xt = [XM; Xn];