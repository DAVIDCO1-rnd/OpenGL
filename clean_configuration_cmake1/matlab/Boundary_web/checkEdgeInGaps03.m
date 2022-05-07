function chk2 = checkEdgeInGaps03(X,i,j,Plns)

Xi = X(i,:);
Xj = X(j,:);
Ps = [Xi;Xj];
iP = 0;
jP = 0;
chk2 = [-1; -1];%not defined
for p = 1:size(Plns,2)
   pbpoly = [Plns{p};Plns{p}(1,:)];
   IN = inpolygon(Ps(:,1), Ps(:,2), pbpoly(:,1), pbpoly(:,2));
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
       comp = iP;
       iid = find((Xi(:,1)-Plns{comp}(:,1)) == 0 & (Xi(:,2)-Plns{comp}(:,2)) == 0);
       jid = find((Xj(:,1)-Plns{comp}(:,1)) == 0 & (Xj(:,2)-Plns{comp}(:,2)) == 0);
       d = abs(iid-jid);
       if d == 1 || d == size(Plns{comp},1)-1
           chk2(2,1) = 1;
       else
           chk2(2,1) = 0;
       end
       break;
   end       
end