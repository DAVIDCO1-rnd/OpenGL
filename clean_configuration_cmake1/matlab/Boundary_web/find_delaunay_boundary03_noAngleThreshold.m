function [bids E Ne] = find_delaunay_boundary03_noAngleThreshold(X,Fd)
%inputs:
%X = n by 3 array (Easting, Northing, Height) or (X,Y,Z)
%Fd = dmax (max point to point distance
%outputs
%bids: IDs to X that presents the sequence of points in the extracted
%boundary
%Please refer the paper: M. Awrangjeb, "Using point cloud data to identify,
%trace, and regularize the outlines of buildings" International Journal of Remote Sensing, Volume 37, Issue 3, February 2016, pages 551-579
%Open access at: http://www.tandfonline.com/doi/pdf/10.1080/01431161.2015.1131868

aThresh = 22.5/2; %standard 45 degree
dFd = 2*Fd;
msd = dFd*dFd;
msd1 = Fd*Fd;
TRI = delaunay(X(:,1),X(:,2));
TRI = sort(TRI,2);
%draw
%triplot(TRI,X(:,1),X(:,2));

nP = size(X,1);
E = zeros(nP,nP) == 1;
Ne = zeros(nP,nP);%number of times an edge is shared by triangles (max 2, min 0)

for i = 1:size(TRI,1)
    T = TRI(i,:);
    Ne(T(1,1),T(1,2)) = Ne(T(1,1),T(1,2)) + 1;
    Ne(T(1,2),T(1,3)) = Ne(T(1,2),T(1,3)) + 1;
    Ne(T(1,1),T(1,3)) = Ne(T(1,1),T(1,3)) + 1;
    
    E(T(1,1),T(1,2)) = 1;
    E(T(1,2),T(1,1)) = 1;
    E(T(1,2),T(1,3)) = 1;
    E(T(1,3),T(1,2)) = 1;
    E(T(1,1),T(1,3)) = 1;
    E(T(1,3),T(1,1)) = 1;
end

Q = [];
for i = 1:nP
    for j = 1:nP
        if (i < j && E(i,j) == 1 && Ne(i,j) == 1)% && ~V(i,j)) % check only the upper triangle; E(i,j) = 1 indicates this is an outside edge
        	sd = (X(i,1)-X(j,1))*(X(i,1)-X(j,1)) + (X(i,2)-X(j,2))*(X(i,2)-X(j,2));
            if sd > msd
                %out1 = [i j]
                if i == 2986 || j == 2986
                    here = 1;
                end
                %remove this 
                %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-r'); hold on;
                E(i,j) = 0;
                E(j,i) = 0;
                Ne(i,j) = 0;
                %find other vertex k of triangle ijk and add edge ik and jk
                %as suspects
                k = find(E(i,:) & E(j,:) == 1);
                if size(k,2)>1
                    here = 1;
                    P1 = X(i,:);
                    P2 = X(j,:);
                    m = (P2(1,2) - P1(1,2))/(P2(1,1) - P1(1,1));
                    c = P1(1,2) - m*P1(1,1);
                    Pks = X(k',:);
                    num = sqrt(1 + m*m);
                    den = m*Pks(:,1) - Pks(:,2) + c;
                    dks = abs(den/num);
                    [mn id] = min(dks);
                    k = k(1,id);
                end
                if size(k,2) == 1%found
                    %for edge (i,k)
                    if i < k
                        Ne(i,k) = Ne(i,k)-1;
                        Q = [Q; [i k]];
                    else
                        Ne(k,i) = Ne(k,i)-1;
                        Q = [Q; [k i]];
                    end
                    %for edge (j,k)
                    if j < k
                        Ne(j,k) = Ne(j,k)-1;
                        Q = [Q; [j k]];
                    else
                        Ne(k,j) = Ne(k,j)-1;
                        Q = [Q; [k j]];
                    end
                else%not found, error! check if happen
                end
            else
                
            end
        end
    end
end

while size(Q,1)>0
    i = Q(1,1); j = Q(1,2);
    if E(i,j) == 1
        %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-y'); hold on;
        sd = (X(i,1)-X(j,1))*(X(i,1)-X(j,1)) + (X(i,2)-X(j,2))*(X(i,2)-X(j,2));
        if sd > msd
            %out2 = [i j]
            if i == 2986 || j == 2986
                here = 1;
            end
            %remove this 
            %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-r'); hold on;
            E(i,j) = 0;
            E(j,i) = 0;
            Ne(i,j) = 0;
            %find other vertex k of triangle ijk and add edge ik and jk
            %as suspects
            k = find(E(i,:) & E(j,:) == 1);
            if size(k,2)>1
                here = 1;
                P1 = X(i,:);
                P2 = X(j,:);
                m = (P2(1,2) - P1(1,2))/(P2(1,1) - P1(1,1));
                c = P1(1,2) - m*P1(1,1);
                Pks = X(k',:);
                num = sqrt(1 + m*m);
                den = m*Pks(:,1) - Pks(:,2) + c;
                dks = abs(den/num);
                [mn id] = min(dks);
                k = k(1,id);
            end
            if size(k,2) == 1%found
                %for edge (i,k)
                if i < k
                    Ne(i,k) = Ne(i,k)-1;
                    Q = [Q; [i k]];
                else
                    Ne(k,i) = Ne(k,i)-1;
                    Q = [Q; [k i]];
                end
                %for edge (j,k)
                if j < k
                    Ne(j,k) = Ne(j,k)-1;
                    Q = [Q; [j k]];
                else
                    Ne(k,j) = Ne(k,j)-1;
                    Q = [Q; [k j]];
                end
            else%not found, error! check if happen
            end
        else

        end
    end
    %else%this edge alreday marked unused in an iteration of the above code
        if size(Q,1)>1
            Q = Q(2:end,:);
        else
            Q = [];
        end
    %end
end
%{
Se = [];
F = zeros(nP,nP) == 1;%F(i,j) = 0: not checked before or decided to remove, 1: checked before and decided not to remove
while(1)
    dE = 0;
    for i = 1:nP
        for j = 1:nP
            if i == 2935 && j == 2986
                here = 1;
            end
            if (i < j && E(i,j) == 1 && Ne(i,j) == 1 && F(i,j) == 0)
                %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-k'); hold on;
                sd = (X(i,1)-X(j,1))*(X(i,1)-X(j,1)) + (X(i,2)-X(j,2))*(X(i,2)-X(j,2));
                k = find(E(i,:) & E(j,:) == 1);
                if size(k,2)>1
                    here = 1;
                    P1 = X(i,:);
                    P2 = X(j,:);
                    m = (P2(1,2) - P1(1,2))/(P2(1,1) - P1(1,1));
                    c = P1(1,2) - m*P1(1,1);
                    Pks = X(k',:);
                    num = sqrt(1 + m*m);
                    den = m*Pks(:,1) - Pks(:,2) + c;
                    dks = abs(den/num);
                    [mn id] = min(dks);
                    k = k(1,id);
                end
                if size(k,2) == 1%found
                    %find angle at vertex k for triangle ijk
                    A = X(i,:);
                    B = X(j,:);
                    C = X(k,:);
                    asq = (B(1,1)-C(1,1))*(B(1,1)-C(1,1)) + (B(1,2)-C(1,2))*(B(1,2)-C(1,2));
                    a = sqrt(asq);
                    bsq = (A(1,1)-C(1,1))*(A(1,1)-C(1,1)) + (A(1,2)-C(1,2))*(A(1,2)-C(1,2));
                    b = sqrt(bsq);
                    csq = (B(1,1)-A(1,1))*(B(1,1)-A(1,1)) + (B(1,2)-A(1,2))*(B(1,2)-A(1,2));
                    %c = sqrt(csq);
                    %now find corners with the base RC
                    %alpha = 180*acos((bsq + csq - asq)/(2*b*c))/pi; %angle at A
                    %beta = 180*acos((csq + asq - bsq)/(2*c*a))/pi; %angle at B
                    gamma = 180*acos((asq + bsq - csq)/(2*a*b))/pi; %angle at C
                    if (gamma > 90 && sd > msd1) || abs(180-gamma) <= aThresh%abtuse angle
                        dE = dE + 1;
                        %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-r'); hold on;
                        E(i,j) = 0;
                        E(j,i) = 0;
                        Ne(i,j) = 0;
                        %for edge (i,k)
                        if i < k
                            Ne(i,k) = Ne(i,k)-1;
                            %Q = [Q; [i k]];
                        else
                            Ne(k,i) = Ne(k,i)-1;
                            %Q = [Q; [k i]];
                        end
                        %for edge (j,k)
                        if j < k
                            Ne(j,k) = Ne(j,k)-1;
                            %Q = [Q; [j k]];
                        else
                            Ne(k,j) = Ne(k,j)-1;
                            %Q = [Q; [k j]];
                        end
                    else
                        F(i,j) = 1;
                    end
                else%not found, error! check if happen
                    %mark this edge as a straight edge without forming a triangle
                    Se = [Se; [i j]];
                end
            end
        end
    end
    if dE == 0
        break
    end
end%while
%}
%track boundary
%find the initial boundary edge
%global El;%cols 1&2: edges which are already visited, col 3: edge's boundary cell number col 4: edge number/row number in that cell
%El = [];
%global S;
chkV = E;
fv = 0;PVs = [];
for i = 1:nP
    for j = 1:nP
        if i < j && E(i,j) == 1 && Ne(i,j) == 1
            s = struct('f0', [], 'f1', [i j], 'f2', [i;j], 'f3', []);%f0: parent struct, f1: edges, f2: vertices list, f3: children structs
            fv = i;%first vertex
            cv = j;%current vertex
            pv = i;%previous vertex
            %PVs = pv;
            %show
            %plot(X(fv,1), X(fv,2), 'og'); hold on;
            %plot(X(pv,1), X(pv,2), 'sy'); hold on;
            %plot(X(cv,1), X(cv,2), 'db'); hold on;
            %plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-c', 'linewidth', 2); hold on;
            break;
        end
        chkV(i,j) = 0;
        chkV(j,i) = 0;
    end
    if fv > 0
        break;
    end
end
if fv>0
    S{1} = s;
    El = [i j 1 1];
    chkV(i,j) = 0;
    chkV(j,i) = 0;
    count = 1;
    Q = [count];%queue for structs to explore
    is = i;
    js = j;
    Qactive = 1;
else
    Q = [];
    Qactive = [];
end
%B = []; B1 = [];
ccount = 1;PVall ={};
InsertLoop = [];
while size(Q,1)>0
    found = 0;
    prevSQ = size(Q,1);
    for qc = prevSQ:-1:1
        q = Q(qc,1);%struct number
        if Qactive(qc,1) == 1%if this sturct is not marked inactive
            found = 1;
            break;
        end
    end
    if found == 0
        Q = [];
        Qactive = [];
        break;
    end
    e = S{q}.f1;%starting edge
    pv = e(1,1);%previous and current vertices
    cv = e(1,2);
    %plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-g', 'linewidth', 2); hold on;
    if size(PVs,1) == 0
        PVs = [[pv q];[cv q]];
        stq = q;%starting q of this connected commponent of the graph
    else
        PVs = [PVs; [cv q]];%previous vertics, which are left column of S{q}.f1
    end
    %explore the current struct
    while(1)
        vs = find(E(cv,:) == 1); %all vertices that are connected to cv
        %find all vertices, other than pv, which are along boundary (remove non
        %boundary vertices
        bv = [];
        if cv == 1628 || cv == 156
            here = 1;
        end
        for m = 1:size(vs,2)
            k = vs(1,m);
            if q == 13 && k == 42 && cv == 1
                here = 1;
            end
            chk = (El(:,1) == cv & El(:,2) == k) | (El(:,1) == k & El(:,2) == cv); %check if edge (cv,k) or (k,cv) is already visited
            if sum(chk) > 0 && k ~= pv%the current edge is already visited, so a close boundary is found and mark it
                el = El(chk,:);%&& sum(PVs == k)==0 %
                if el(1,3) == stq && cv == el(1,1) && el(1,4) == 1%first vertex of first edge of stq is cv
                    %make q as a parent of stq
                    S{stq}.f0 = q;
                    InsertLoop(ccount,:) = [stq q];
                end
                if el(1,3) < q
                    %this edge was added in the Q and S to be explored
                    %latter, but it has now come to be an edge of contour
                    %found from q, so make this Q and S entry inactive so
                    %that it is not used latter
                    chkq = Q == el(1,3);
                    if sum(chkq)>0
                    Qactive(chkq,1) = 0;
                    
                    %and add this to be a nextly visited edge
                    bv = [bv; k];
                    %plot(X(bv,1), X(bv,2), 'dk'); hold on;
                    %plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-y', 'linewidth', 2); hold on;
                    else
                        %B1 = [B1; [q size(S{q}.f1,1) el(1,3:4)]];%record the edge, it may be on a potential loop in future  
                        %S{q}.f3 = [S{q}.f3; el(1,3)];%update child list (a previuos paranet becomes a child, so a loop)
                        %show
                        %plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-w', 'linewidth', 2); hold on;%ignore already visited edge  
                    end
                else%it is within the current or a previously explored 
                    %B = [B; [q size(S{q}.f1,1) el(1,3:4)]];%record the boundary
                    %S{q}.f3 = [S{q}.f3; el(1,3)];%update child list (a previuos paranet becomes a child, so a loop)
                    %show
                    %plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-k', 'linewidth', 2); hold on;
                    continue;
                end
            end
            if k ~= pv           && sum(chk) == 0 && (Ne(cv,k) == 1 || Ne(k,cv) == 1)%k is not pv, all of whose possible neigbours are not already in Vc and E(j,k) is used only once
            %if sum(PVs == k)==0 && sum(chk) == 0 && (Ne(cv,k) == 1 || Ne(k,cv) == 1)
                bv = [bv; k];
                %plot(X(bv,1), X(bv,2), 'dk'); hold on;
                 %plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-y', 'linewidth', 2); hold on;
            end
        end

        if size(bv,1) == 0
            %{
            ccount = ccount+1;
            PVall{ccount} = PVs;            
            %dead end
            %noway to progress
            plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-r', 'linewidth', 2); hold on;
            if size(Q,1) == 1%only one element which is now being explored
            %look for another boundary edge, if any
            fv = 0;
            bm = (E & chkV) & (Ne == 1);
            hasB = sum(sum(bm));
            if hasB > 0
                for i = is:nP
                    for j = js:nP
                        if i < j && bm(i,j)
                            s = struct('f0', [], 'f1', [i j], 'f2', [i;j], 'f3', []);%f0: parent struct, f1: edges, f2: vertices list, f3: children structs
                            fv = i;%first vertex
                            cv = j;%current vertex
                            pv = i;%previous vertex

                            %show
                            %plot(X(fv,1), X(fv,2), 'og'); hold on;
                            %plot(X(pv,1), X(pv,2), 'sy'); hold on;
                            %plot(X(cv,1), X(cv,2), 'db'); hold on;
                            plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-c', 'linewidth', 2); hold on;
                            break;
                        end
                        chkV(i,j) = 0;
                        chkV(j,i) = 0;
                    end
                    if fv > 0
                        break;
                    end
                end
                if fv > 0% a new edge is found
                    count = count+1;
                    S{count} = s;
                    El = [El;[i j count 1]];
                    chkV(i,j) = 0;
                    chkV(j,i) = 0;
                    is = i;
                    js = j;
                    Q = [Q;count];%queue for structs to explore
                    Qactive = [Qactive; 1];
                    PVs = [];
                end
            end
            end
            %}
            break;
        elseif size(bv,1) == 1
            %only 1 option, so go forward tracking boundary
            k = bv(1,1);
            
            chkV(cv,k) = 0;
            chkV(k,cv) = 0;
            
            S{q}.f1 = [S{q}.f1; [cv k]];%update edge list for S{q}
            S{q}.f2 = [S{q}.f2; k];%update vertices list for S{q}
            El = [El; [cv k q size(S{q}.f1,1)]];
            pv = cv;
            cv = k;
            %plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-m', 'linewidth', 2); hold on;
            
            if cv == 2935
                here = 1;
            end
            
            PVs = [PVs;[cv q]];            
            %if sum(PVs == cv)==1 %current vertext has already been visited in an earlier iteration, possibly a loop, so break
                %self-loop! Update B
            %    loopid = find(PVs == cv);
            %    B = [B; [q size(S{q}.f1,1) q loopid]];%record the boundary
            %    S{q}.f3 = [S{q}.f3; q];%update child list (a previuos paranet becomes a child, so a loop)
                %break;
            %end
            %{
            Vi(end,1) = k;
            Vc = [Vc; pv];
            if k == fv % aboundary is found
                Vi = [];
                Vc = [Vc; fv];
                count = count + 1;
                Vb{count,1} = Vc;
                Vc = [];
            end
            %}            
        else%more than one option, generate new structs to explore latter
            clockwiseids = clockwise_choice(X([cv;pv;bv],:)); 
            bvclockwise = bv(clockwiseids,1);
            for l = size(bvclockwise,1):-1:1
                k = bvclockwise(l,1);
                
                chkV(cv,k) = 0;
                chkV(k,cv) = 0;
                
                count = count+1;
                %create a new struct and set q as a parent of new, and new
                %as a child of q
                S{count} = struct('f0', q, 'f1', [cv k], 'f2', [cv;k], 'f3', []);
                El = [El; [cv k count 1]];
                S{q}.f3 = [S{q}.f3; count];
                Q = [Q; count]; 
                Qactive = [Qactive; 1];
            end
            break;
        end
    end
    
    %if prevSQ > qc
        
        Q1 = Q(1:qc-1,:);
        Qa1 = Qactive(1:qc-1,:);
        
        Q2 = Q(prevSQ+1:end,:);
        Qa2 = Qactive(prevSQ+1:end,:);
        
        Q = [Q1;Q2];
        Qactive = [Qa1;Qa2];        
    %end
    
    if sum(Qactive,1) == 0 || size(Q,1) == 0
        Q = [];
        Qactive = [];        
        PVall{ccount} = PVs; 
        if size(InsertLoop,1)<ccount
            InsertLoop(ccount,:) = [0 0];
        end
            
        ccount = ccount+1;
    %end
        
    %if size(Q,1) <= qc || size(Q,1) == 0
    %    Q = [];
    %    Qactive = [];
        
        %look for another boundary edge, if any
            fv = 0;
            bm = (E & chkV) & (Ne == 1);
            hasB = sum(sum(bm));
            if hasB > 0
                for i = is:nP
                    for j = js:nP
                        if i < j && bm(i,j)
                            s = struct('f0', [], 'f1', [i j], 'f2', [i;j], 'f3', []);%f0: parent struct, f1: edges, f2: vertices list, f3: children structs
                            fv = i;%first vertex
                            cv = j;%current vertex
                            pv = i;%previous vertex

                            %show
                            %plot(X(fv,1), X(fv,2), 'og'); hold on;
                            %plot(X(pv,1), X(pv,2), 'sy'); hold on;
                            %plot(X(cv,1), X(cv,2), 'db'); hold on;
                            %plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-c', 'linewidth', 2); hold on;
                            break;
                        end
                        chkV(i,j) = 0;
                        chkV(j,i) = 0;
                    end
                    if fv > 0
                        break;
                    end
                end
                if fv > 0% a new edge is found
                    count = count+1;
                    S{count} = s;
                    El = [El;[i j count 1]];
                    chkV(i,j) = 0;
                    chkV(j,i) = 0;
                    is = i;
                    js = j;
                    Q = [Q;count];%queue for structs to explore
                    Qactive = [Qactive; 1];
                    PVs = [];
                end
            end
        if size(Q,1) == 0    
            break;
        end
    %else
    %    Q = Q(qc+1:end,:);
    %    Qactive = Qactive(qc+1:end,:);
    end
end

for pv=1:size(PVall,2)
    %find loops
    PV1 = PVall{pv};
    loops{pv} = [];
    for nid = 1:size(PV1,1)-2
        node = PV1(nid,1);
        chk = PV1(nid+1:end,1) == node;
        if sum(chk) == 1
           %a loop found
           loops{pv} = [loops{pv}; [nid find(chk==1)+nid]];
        end
    end
end

%for each loop try to find two segment loops
bids = {};
bcount = 0;
for pv=1:size(PVall,2)
    %find loops
    lcount = 0; loopids = {};
    lentries = loops{pv};
    insloop = InsertLoop(pv,:);
    PVs = PVall{pv};
    for i = 1:size(lentries,1)
        %try to find a loop starting from one end to another end. so, maximum loops will be 2 for this entry
        lp = lentries(i,:);
        stq = PVs(lp(1,1),2);
        enq = PVs(lp(1,2),2);
        if (insloop(1,1) == stq && insloop(1,2) == enq) || (insloop(1,1) == enq && insloop(1,2) == stq)
            %inserted loop, call only once
            [loopids1 flg1] = checkloop01(PVs, S, lp(1,1), lp(1,2));
            
            if flg1 == 1
                if lcount > 0
                    [idr flgr] = checkSubset(loopids, loopids1);
                %flgr: 0 new, so add; -1 loopids1 is a superset of an existing, so replace by loopids1;
                %1 loopids1 is a subset of an existing, so no replace 
                if flgr == 0%new
                    lcount = lcount+1;
                    loopids{lcount} = loopids1;
                    %plot(X(loopids1,1),X(loopids1,2),'-m', 'linewidth', 2); hold on;
                elseif flgr == -1%replace
                    loopids{idr} = loopids1;
                    %plot(X(loopids1,1),X(loopids1,2),'-m', 'linewidth', 2); hold on;
                end
                else
                    lcount = lcount+1;
                    loopids{lcount} = loopids1;
                    %plot(X(loopids1,1),X(loopids1,2),'-g', 'linewidth', 2); hold on;
                end
            end
        else%call twice
            [loopids1 flg1] = checkloop01(PVs, S, lp(1,1), lp(1,2));
            
            if flg1 == 1
                if lcount > 0
                    [idr flgr] = checkSubset(loopids, loopids1);
                %flgr: 0 new, so add; -1 loopids1 is a superset of an existing, so replace by loopids1;
                %1 loopids1 is a subset of an existing, so no replace 
                if flgr == 0%new
                    lcount = lcount+1;
                    loopids{lcount} = loopids1;
                    %plot(X(loopids1,1),X(loopids1,2),'-m', 'linewidth', 2); hold on;
                elseif flgr == -1%replace
                    loopids{idr} = loopids1;
                    %plot(X(loopids1,1),X(loopids1,2),'-m', 'linewidth', 2); hold on;
                end
                else
                    lcount = lcount+1;
                    loopids{lcount} = loopids1;
                    %plot(X(loopids1,1),X(loopids1,2),'-g', 'linewidth', 2); hold on;
                end
            end
            [loopids2 flg2] = checkloop01(PVs, S, lp(1,2), lp(1,1));
            
            if flg2 == 1
                if lcount > 0
                    [idr flgr] = checkSubset(loopids, loopids2);
                %flgr: 0 new, so add; -1 loopids1 is a superset of an existing, so replace by loopids1;
                %1 loopids1 is a subset of an existing, so no replace 
                if flgr == 0%new
                    lcount = lcount+1;
                    loopids{lcount} = loopids2;
                    %plot(X(loopids2,1),X(loopids2,2),'-m', 'linewidth', 2); hold on;
                elseif flgr == -1%replace
                    loopids{idr} = loopids2;
                    %plot(X(loopids2,1),X(loopids2,2),'-m', 'linewidth', 2); hold on;
                end
                else
                    lcount = lcount+1;
                    loopids{lcount} = loopids2;
                    %plot(X(loopids2,1),X(loopids2,2),'-g', 'linewidth', 2); hold on;
                end
            end
        end
    end
    
    for j = 1:size(loopids,2)
        bcount = bcount + 1;
        bids{bcount} = loopids{j};
    end
end

%show boundary
figure;
plot(X(:,1),X(:,2),'.b'); hold on;
plot(X(:,1),X(:,2),'ok'); hold on;
for i=1:size(bids,2)
    bndry = X(bids{i},:);
    plot(bndry(:,1),bndry(:,2),'-r'); hold on;
end
hold off;
%{
%consider each pair in B1 if there is a loop
nb1 = size(B1,1);
U = zeros(nb1,1);
for b1=1:nb1
    s1 = B1(b1,1);
    e1 = B1(b1,2);
    s2 = B1(b1,3);
    e2 = B1(b1,4);
    for b2=2:nb1
        s3 = B1(b2,1);
        e3 = B1(b2,2);
        s4 = B1(b2,3);
        e4 = B1(b2,4);
        if s1 == s3 && s2 == s4
           loop = findloop(S{s1}, e1, e3, S{s2}, e2, e4);
        end
    end
end
%}
%{
bids = {};
sids = {};
bcount = 0;
for bn = 1:size(B,1)
    b = B(bn,:);%[q size(S{q}.f1,1) el(1,3:4)]
    %last sturct
    last_num = b(1,1);
    last_num_edges = b(1,2);
    %first sturct
    first_num = b(1,3);
    first_num_edges = b(1,4);
    
    bids1 = [S{last_num}.f1(1:last_num_edges,:) S{last_num}.f2(1:last_num_edges,:)];
    sids1 = [last_num];
    cur_num = last_num;
    joints = []; found = 1;
    while(cur_num ~= first_num)%if they are different
        %then backtrach the edges and nodes
        cur_num = S{last_num}.f0; %take parents
        if size(cur_num,1) == 0 || sum(sids1 == cur_num) == 1
            %the first sturcture was visited in previous iteration and it does not have a parent (cur_num is empty),
            % or cur_num is alreday a visited strcuture, so break the loop
            found = 0;
            break;
        end
        if cur_num == first_num%loop found, so break the loop
            joints = [joints; size(bids1,1)];
            bids1 = [[S{first_num}.f1(first_num_edges:end,:) S{first_num}.f2(first_num_edges:end-1,:)]; bids1];
            sids1 = [sids1;first_num];
        else
            joints = [joints; size(bids1,1)];
            bids1 = [[S{cur_num}.f1 S{cur_num}.f2(1:end-1,:)]; bids1];
            sids1 = [sids1;cur_num];
            last_num = cur_num;
        end
    end
    
    %check joints if they confirm a loop
    if found == 1
        fJoint = 1;
        for j = 1:size(joints,1)
            jid = joints(j,1);
            if bids1(jid,2) ~= bids1(jid+1,1)
                fJoint = 0;
                break;
            end
        end
        if cur_num == first_num && fJoint == 1 && bids1(1,1) == bids1(end,2)
            bcount = bcount + 1;
            bids{bcount} = bids1;
            sids{bcount} = sids1;
            
            plot(X(bids1(:,1),1),X(bids1(:,1),2),'-c', 'linewidth', 2); hold on;
        end
    end
end
%}
%{
bids = {};
nB = size(B,1);
fB = ones(nB,1);
bcount = 0;
for bn = 1:size(B,1)
    if fB(bn,1) == 1
        q = B(bn,1);
        chk = B(:,1) == q;
        fB(chk,1) = 0;
        b = B(chk,:);%[q size(S{q}.f1,1) el(1,3:4)]
        if size(b,1)>1
            chk2 = b(:,3) == q;
            if sum(chk2) == 1
            b = b(chk2,:);
            else
                here = 1;
            end
        else%size(b,1) == 1
            bcount = bcount + 1;
            %last sturct
            last_num = b(1,1);
            last_num_edges = b(1,2);
            %first sturct
            first_num = b(1,3);
            first_num_edges = b(1,4);

            bids{bcount} = [S{last_num}.f1(1:last_num_edges,:) S{last_num}.f2(1:last_num_edges,:)];
            cur_num = last_num;
            while(cur_num ~= first_num)%if they are different
                %then backtrach the edges and nodes
                cur_num = S{last_num}.f0; %take parents
                if cur_num == first_num
                    bids{bcount} = [bids{bcount}; [S{first_num}.f1(first_num_edges:end,:) S{cur_num}.f2(first_num_edges:end-1,:)]];
                else
                    bids{bcount} = [bids{bcount}; [S{cur_num}.f1 S{cur_num}.f2(1:end-1,:)]];
                    last_num = cur_num;
                end
            end
        end
    end
end
%}
%bids = find_loops(E, Ne);
end%function

%{
%%%%%%%%%%%
function find_loops(E, Ne)
%find next vertext iteratively
Vb = {}; count = 0;
Vc = [];%vertices whose all possible neighbours are alreday in the boundary
Vi = [i;j];%vertices whose all possible neighbours are NOT in the boundary
while (0)%size(Vi,1)>0
    vs = find(E(cv,:) == 1); %all vertices now connected to cv
    %find all vertices, other thatn pv, which are along boundary
    bv = [];
    for m = 1:size(vs,2)
        k = vs(1,m);
        chk = sum(Vc == k); 
        if k ~= pv && chk == 0 && (Ne(cv,k) == 1 || Ne(k,cv) == 1)%k is not pv, all of whose possible neigbours are not already in Vc and E(j,k) is used only once
            bv = [bv; k];
            plot(X(bv,1), X(bv,2), 'dk'); hold on;
        end
    end
    
    if size(bv,1) == 0
        %error
    elseif size(bv,1) == 1
        k = bv(1,1);
        Vi(end,1) = k;
        Vc = [Vc; pv];
        if k == fv % aboundary is found
            Vi = [];
            Vc = [Vc; fv];
            count = count + 1;
            Vb{count,1} = Vc;
            Vc = [];
        end
    end
    
end
end%function
%%%%%%%%%%%
%}