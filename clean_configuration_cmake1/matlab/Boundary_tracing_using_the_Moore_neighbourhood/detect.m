%% bw labeling
%------------
% if you have rgb
% myimg=imread('testhough.jpg');
% myimg  =rgb2gray(myimg);
%  level = graythresh(myimg);
% bw=im2bw(myimg,level);
% bw=~bw;
% figure(1),imshow(bw,'Initialmagnification','fit');title('bw ')
%---------------
% else
% compare
%    BW = logical([1 1 1 0 0 0 0 0
%                       1 1 1 0 1 1 0 0
%                       1 1 1 0 1 1 0 0
%                       1 1 1 0 0 0 1 0
%                       1 1 1 0 0 0 1 0
%                       1 1 1 0 0 0 1 0
%                       1 1 1 0 0 1 1 0
%                       1 1 1 0 0 0 0 0]);
%         L = bwlabel(BW,4)
%          [Lmic]  = detect(bw);

function [L]  = detect(bw);


[m,n]=size(bw);
bwnnew = [ zeros(1,n) ;  bw; zeros(1,n)];
bwnnew  = [ zeros(m+2,1) , bwnnew , zeros(m+2,1)];



L=zeros(size(bwnnew));

startLabel=1;
for(ir=2:m+1)
    for(ic=2:n+1)
        
        curdata=bwnnew(ir,ic);
        lc           = L(ir,ic);
        
        if((curdata==1)&&(lc==0))
            L(ir,ic)=startLabel;
             L = findConnectedLabels(L,startLabel,bwnnew,ir,ic,m,n);
             startLabel  =startLabel+1;
        end
        
    end
end


L = L(2:m+1,2:n+1);


%%     -------------  findConnectedLabels
    function [L]  = findConnectedLabels(L,startLabel,bwcur,ir,ic,m,n)
        %startLabel
        %L
        %pause(0.1)
        
        a = bwcur(ir+1, ic);  % next row
        b = bwcur(ir-1, ic);    % previous row
        c = bwcur(ir, ic+1);  % next col
        d = bwcur(ir, ic-1);   % prev column
        
        aa = L(ir+1, ic);  % next row
        bb = L(ir-1, ic);    % previous row
        cc = L(ir, ic+1);  % next col
        dd = L(ir, ic-1);   % prev column
        
     %   Lout = L;
        
        if((a==1)&&(aa==0))
            L(ir+1, ic)=startLabel;
            [L]  = findConnectedLabels(L,startLabel,bwcur,ir+1,ic,m,n);
        end
        
        if((b==1)&&(bb==0))
            L(ir-1, ic)=startLabel;
            [L]  = findConnectedLabels(L,startLabel,bwcur,ir-1,ic,m,n);
        end
        
        if((c==1)&&(cc==0))
            L(ir, ic+1)=startLabel;
            [L]  = findConnectedLabels(L,startLabel,bwcur,ir,ic+1,m,n);
        end
        
        if((d==1)&&(dd==0))
            L(ir, ic-1)=startLabel;
            [L]  = findConnectedLabels(L,startLabel,bwcur,ir,ic-1,m,n);
        end
        
        
    end
%---------------------

end


