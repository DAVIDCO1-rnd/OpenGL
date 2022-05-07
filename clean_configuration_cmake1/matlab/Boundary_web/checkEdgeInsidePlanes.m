function chk2 = checkEdgeInsidePlanes(X,i,j,Plns)

Xi = X(i,:);
Xj = X(j,:);
midP = [Xi + Xj]/2;
Ps = [Xi;Xj;midP];
%iP = 0;
%jP = 0;
chk2 = 0;%edge (i,j) is outside all planes
for p = 1:size(Plns,2)
   pbpoly = [Plns{p};Plns{p}(1,:)];
   IN = inpolygon(Ps(:,1), Ps(:,2), pbpoly(:,1), pbpoly(:,2));
   if sum(IN) == 3 || IN(3,1) == 1
       %edge (i,j) is inside plane p
       chk2 = p;
       break;
   end
       %{
   if IN(1,1) == 1
       iP = p;%point i is on plane p
   end
   
   if IN(2,1) == 1
       jP = p;%point i is on plane p
   end 
   
   if iP > 0 && jP > 0 && iP ~= jP
       chk2(1,1) = 1;%they are on different planes
       break;
   elseif iP > 0 && jP > 0 && iP == jP
       chk2(1,1) = 0;%they are on same plane       
       %find if they are consecutive points in p
       d = abs(i-j);
       if d == 1 || d == size(pbpoly,1)-2
           chk2(2,1) = 1;
       else
           chk2(2,1) = 0;
       end
       break;
   end    
   %}
end