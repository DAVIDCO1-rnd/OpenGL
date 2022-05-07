function [bndry bndryin] = delaunay_boundary02_fig(X,Fd,Plns)

[bndry E Ne] = find_delaunay_boundary03(X,Fd);
%Fd = Fd-0.2;

aThresh = 22.5/2; %standard 45 degree
dFd = 2*Fd;
msd = dFd*dFd;
msd1 = Fd*Fd;


nP = size(E,1);
nBnd = size(bndry,2);

V = E; %V(i,j) = 1: edge not visited; 0: no edge; 2: edge visited
Ner = Ne;
Er = E;
for i = 1:nP
    for j = i+1:nP
        if Ne(i,j) == 1
            Ner(i,j) = 3;%not to consider
        end
    end
end

for p = 1:size(Plns,2)
   pbpoly = [Plns{p};Plns{p}(1,:)];
   %plot(pbpoly(:,1), pbpoly(:,2), '-y','linewidth',2); hold on;  
end

%TRI = delaunay(X(:,1),X(:,2));
%TRI = sort(TRI,2);
%draw
figure;
%triplot(TRI,X(:,1),X(:,2),'-c'); hold on;
for i = 1:nP
    for j = 1:nP
        if E(i,j)== 1 && Ne(i,j)==2
            chk2 = checkEdgeInsidePlanes(X,i,j,Plns);
            if chk2 == 0
                plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-y'); hold on;
            end
        %else
        end
    end
end
%show boundary
%bspb = X(bndry{1},:);
%plot([bspb(:,1); bspb(1,1)], [bspb(:,2); bspb(1,2)], '-m'); hold on;
%show plane boundaries
p=1; pbpoly = [Plns{p};Plns{p}(1,:)];  plot(pbpoly(:,1), pbpoly(:,2), '-m','linewidth',1.5); hold on;
p=2; pbpoly = [Plns{p};Plns{p}(1,:)];  plot(pbpoly(:,1), pbpoly(:,2), '-g','linewidth',1.5); hold on;
p=3; pbpoly = [Plns{p};Plns{p}(1,:)];  plot(pbpoly(:,1), pbpoly(:,2), '-c','linewidth',1.5); hold on;
p=4; pbpoly = [Plns{p};Plns{p}(1,:)];  plot(pbpoly(:,1), pbpoly(:,2), '-b','linewidth',1.5); hold on;
plot(X(:,1), X(:,2), '.k', 'markersize', 10); hold on;

for bn = 1:nBnd
    bids = {};
    %boundary bn
    Xids = bndry{bn}(:,1)';
    %nBP = size(Xids,2);    
    Bps = X(Xids,:);
    bpol = [Bps; Bps(1,:)];
    %plot(bpol(:,1), bpol(:,2), '-m','linewidth',2); hold on;
    
    %points within boundary bn
    IN = inpolygon(X(:,1), X(:,2), bpol(:,1), bpol(:,2));
    Idin = find(IN == 1);
    Psin = X(Idin,:);
    %plot(Psin(:,1), Psin(:,2), 'oc'); hold on;
    numIn = size(Psin,1);
    Q = []; bigWholeFound = 0; 
    if numIn>6
        for n = 1:numIn
            i = Idin(n,1);
            js = find(Er(i,:)==1)';
            %plot(X(i,1), X(i,2), 'sr'); hold on;
            %plot(X(js,1), X(js,2), 'dk'); hold on;
            for numj = 1:size(js,1)
                j = js(numj,1);
                chk = Idin == j; %check if j is also within boundary bn
                chk2 = checkEdgeInGaps02(X,i,j,Plns); %check if the edge (i,j) is within a gap in between two planes
                if sum(chk) == 1 && V(i,j) == 1 && (Ne(i,j) == 2 || Ne(j,i) == 2) && chk2(1,1) == 1 
                    sd = (X(i,1)-X(j,1))*(X(i,1)-X(j,1)) + (X(i,2)-X(j,2))*(X(i,2)-X(j,2));
                    %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-k'); hold on;
                    if sd > msd
                        %plot(X(i,1), X(i,2), 'sr'); hold on;
                        %plot(X(js,1), X(js,2), 'dk'); hold on;
                        out1 = [i j]
                        if i == 168 || j == 200
                            here = 1;
                        end
                        bigWholeFound = 1; 
                        %remove this 
                        plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-r'); hold on;
                        Er(i,j) = 0;
                        Er(j,i) = 0;
                        Ner(i,j) = 0;
                        Ner(j,i) = 0;
                        V(i,j) = 2;%added
                        vs = find(E(i,:) & E(j,:) == 1);%get all other vertics with edge (i,j)
                        for v = 1:size(vs,2)
                            here = 1;
                            k = vs(1,v);                          
                                                        
                            %for edge (i,k)
                            %check if (i,k) is already in Q
                            chk2 = checkEdgeInGaps03(X,i,k,Plns); %check if the edge (i,k) is within a gap in between two planes
                            %if chk2 == 1
                            fik = 0;
                            if size(Q,1)>0
                                if sum(Q(:,1) == i & Q(:,2) == k) == 1 || sum(Q(:,1) == k & Q(:,2) == i) == 1
                                    %it is alreday inlcuded into a previous
                                    %iteration, so do not add it anymore
                                    %Er(i,k) = 0;
                                    %Er(k,i) = 0;
                                    %Ner(i,k) = 0;
                                    %Ner(k,i) = 0;
                                    %plot([X(i,1) X(k,1)], [X(i,2) X(k,2)],'-m'); hold on;
                                    fik = 1;
                                end
                            end
                            if fik == 0 && (Ner(i,k) > 0 || Ner(k,i) > 0) && (chk2(1,1) == 1 ||...
                                    (chk2(1,1) == 0 && chk2(2,1) == 1 && (Ner(i,k) > 1 || Ner(k,i) > 1)) ||...
                                    (chk2(1,1) == 0 && chk2(2,1) == 0 && (Ner(i,k) > 0 || Ner(k,i) > 0)))%if (i,k) is from the same plane i,k should not be the consecutive points along plane boundary
                                %if edge (i,k) is from different planes, it can
                                %be nulled, but if it is from the same
                                %plane, then it cannot be
                                if i < k
                                    Ner(i,k) = Ner(i,k)-1;
                                    Q = [Q; [i k chk2(2,1)]];                                
                                else
                                    Ner(k,i) = Ner(k,i)-1;
                                    Q = [Q; [k i chk2(2,1)]];
                                end
                                %plot([X(i,1) X(k,1)], [X(i,2) X(k,2)],'-b'); hold on;
                                if Ner(i,k) == 0 && Ner(k,i) == 0
                                    here = 1;
                                end
                            end
                            %end
                            %for edge (j,k)
                            %check if (j,k) is already in Q
                            chk2 = checkEdgeInGaps03(X,j,k,Plns); %check if the edge (k,j) is within a gap in between two planes
                            %if chk2 == 1
                            fjk = 0;
                            if size(Q,1)>0
                                if sum(Q(:,1) == j & Q(:,2) == k) == 1 || sum(Q(:,1) == k & Q(:,2) == j) == 1
                                    %it is alreday inlcuded into a previous
                                    %iteration, so do not add it anymore
                                    %Er(j,k) = 0;
                                    %Er(k,j) = 0;
                                    %Ner(j,k) = 0;
                                    %Ner(k,j) = 0;
                                    %plot([X(j,1) X(k,1)], [X(j,2) X(k,2)],'-m'); hold on;
                                    fjk = 1;
                                end
                            end                            
                            if fjk == 0 && (Ner(j,k) > 0 || Ner(k,j) > 0) && (chk2(1,1) == 1 ||...
                                    (chk2(1,1) == 0 && chk2(2,1) == 1 && (Ner(j,k) > 1 || Ner(k,j) > 1)) ||...
                                    (chk2(1,1) == 0 && chk2(2,1) == 0 && (Ner(j,k) > 0 || Ner(k,j) > 0)))%if (j,k) is from the same plane j,k should not be the consecutive points along plane boundary
                                if j < k
                                    Ner(j,k) = Ner(j,k)-1;
                                    Q = [Q; [j k chk2(2,1)]];                                
                                else
                                    Ner(k,j) = Ner(k,j)-1;
                                    Q = [Q; [k j chk2(2,1)]];
                                end
                                %plot([X(j,1) X(k,1)], [X(j,2) X(k,2)],'-b'); hold on; 
                                if Ner(j,k) == 0 && Ner(k,j) == 0
                                    here = 1;
                                end
                            end                                                       
                            %end
                        end
                    end
                end
            end
        end
        msd = 5;
        if bigWholeFound == 1 
        while size(Q,1)>0
            i = Q(1,1); j = Q(1,2);
            if Er(i,j) == 1 && Q(1,3) ~= 1%Q(1,3) = 1 means edge (i,j) is from same plane and i,j are consecutive points on plane boundary
                %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-m'); hold on;
                sd = (X(i,1)-X(j,1))*(X(i,1)-X(j,1)) + (X(i,2)-X(j,2))*(X(i,2)-X(j,2));
                if sd > msd
                    out2 = [i j]                    
                    if i == 2986 || j == 2986
                        here = 1;
                    end
                    %remove this 
                    plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-r'); hold on;
                    Er(i,j) = 0;
                    Er(j,i) = 0;
                    Ner(i,j) = 0;
                    Ner(j,i) = 0;

                    vs = find(E(i,:) & E(j,:) == 1);%get all other vertics with edge (i,j)
                    for v = 1:size(vs,2)
                        here = 1;
                        k = vs(1,v);                          

                        %for edge (i,k)
                        %check if (i,k) is already in Q
                        chk2 = checkEdgeInGaps03(X,i,k,Plns); %check if the edge (i,k) is within a gap in between two planes
                        %if chk2 == 1
                        fik = 0;
                        if size(Q,1)>0
                            if sum(Q(:,1) == i & Q(:,2) == k) == 1 || sum(Q(:,1) == k & Q(:,2) == i) == 1
                                %it is alreday inlcuded into a previous
                                %iteration, so do not add it anymore
                                %Er(i,k) = 0;
                                %Er(k,i) = 0;
                                %Ner(i,k) = 0;
                                %Ner(k,i) = 0;
                                %plot([X(i,1) X(k,1)], [X(i,2) X(k,2)],'-m'); hold on;
                                fik = 1;
                            end
                        end
                        if fik == 0 && (Ner(i,k) == 2 || Ner(k,i) == 2) %&& (chk2(1,1) == 1 ||...
                                    %(chk2(1,1) == 0 && chk2(2,1) == 1 && (Ner(i,k) > 1 || Ner(k,i) > 1)) ||...
                                    %(chk2(1,1) == 0 && chk2(2,1) == 0 && (Ner(i,k) > 0 || Ner(k,i) > 0)))%if (i,k) is from the same plane i,k should not be the consecutive points along plane boundary
                            if i < k
                                Ner(i,k) = Ner(i,k)-1;
                                Q = [Q; [i k chk2(2,1)]];                                
                            else
                                Ner(k,i) = Ner(k,i)-1;
                                Q = [Q; [k i chk2(2,1)]];
                            end
                            %plot([X(i,1) X(k,1)], [X(i,2) X(k,2)],'-b'); hold on;
                            if Ner(i,k) == 0 && Ner(k,i) == 0
                               here = 1;
                            end
                        end
                        %end
                        %for edge (j,k)
                        %check if (j,k) is already in Q
                        chk2 = checkEdgeInGaps03(X,j,k,Plns); %check if the edge (k,j) is within a gap in between two planes
                        %if chk2 == 1
                        fjk = 0;
                        if size(Q,1)>0
                            if sum(Q(:,1) == j & Q(:,2) == k) == 1 || sum(Q(:,1) == k & Q(:,2) == j) == 1
                                %it is alreday inlcuded into a previous
                                %iteration, so do not add it anymore
                                %Er(j,k) = 0;
                                %Er(k,j) = 0;
                                %Ner(j,k) = 0;
                                %Ner(k,j) = 0;
                                %plot([X(j,1) X(k,1)], [X(j,2) X(k,2)],'-m'); hold on;
                                fjk = 1;
                            end
                        end                            
                        if fjk == 0 && (Ner(j,k) == 2 || Ner(k,j) == 2) %&& (chk2(1,1) == 1 ||...
                                    %(chk2(1,1) == 0 && chk2(2,1) == 1 && (Ner(j,k) > 1 || Ner(k,j) > 1)) ||...
                                    %(chk2(1,1) == 0 && chk2(2,1) == 0 && (Ner(j,k) > 0 || Ner(k,j) > 0)))%if (j,k) is from the same plane j,k should not be the consecutive points along plane boundary
                            if j < k
                                Ner(j,k) = Ner(j,k)-1;
                                Q = [Q; [j k chk2(2,1)]];                                
                            else
                                Ner(k,j) = Ner(k,j)-1;
                                Q = [Q; [k j chk2(2,1)]];
                            end
                            %plot([X(j,1) X(k,1)], [X(j,2) X(k,2)],'-b'); hold on; 
                            if Ner(j,k) == 0 && Ner(k,j) == 0
                               here = 1;
                            end
                        end         
                        %end
                    end
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

        Se = [];
        F = zeros(nP,nP) == 1;%F(i,j) = 0: not checked before or decided to remove, 1: checked before and decided not to remove
        while(1)
            dE = 0;
            for i = 1:nP
                for j = 1:nP
                    outij = [i j]
                    if i == 2935 && j == 2986
                        here = 1;
                    end                    
                    if (i < j && Er(i,j) == 1 && Ner(i,j) == 1 && F(i,j) == 0) 
                        chk2 = checkEdgeInGaps03(X,i,j,Plns);
                        if (chk2(1,1) == 1 || (chk2(1,1) == 0 && chk2(2,1) == 0))%if (j,i) is from the same plane j,i should not be the consecutive points along plane boundary
                            %plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-c'); hold on;
                            sd = (X(i,1)-X(j,1))*(X(i,1)-X(j,1)) + (X(i,2)-X(j,2))*(X(i,2)-X(j,2));
                            k = find(Er(i,:) & Er(j,:) == 1);
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
                                    plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-r'); hold on;
                                    Er(i,j) = 0;
                                    Er(j,i) = 0;
                                    Ner(i,j) = 0;
                                    %for edge (i,k)
                                    %chk2 = checkEdgeInGaps03(X,i,k,Plns); %check if the edge (i,k) is within a gap in between two planes
                                    %if chk2 == 1
                                    if i < k 
                                        Ner(i,k) = Ner(i,k)-1;
                                        %Q = [Q; [i k]];
                                    else
                                        Ner(k,i) = Ner(k,i)-1;
                                        %Q = [Q; [k i]];
                                    end
                                    %plot([X(i,1) X(k,1)], [X(i,2) X(k,2)],'-c'); hold on;
                                    %end
                                    %for edge (j,k)
                                    %chk2 = checkEdgeInGaps03(X,j,k,Plns); %check if the edge (k,j) is within a gap in between two planes
                                    %if chk2 == 1
                                    if j < k
                                        Ner(j,k) = Ner(j,k)-1;
                                        %Q = [Q; [j k]];
                                    else
                                        Ner(k,j) = Ner(k,j)-1;
                                        %Q = [Q; [k j]];
                                    end
                                    %plot([X(j,1) X(k,1)], [X(j,2) X(k,2)],'-c'); hold on;
                                    %end


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
            end
            if dE == 0
                break
            end
        end%while
        
        %for including any building boundary for which Ner(i,j) = 2, but
        %Ne(i,j) = 1, make Ner(i,j) == 1 too
        for i = 1:nP
            for j = i+1:nP
                if Ne(i,j) == 1 && Ner(i,j) == 2
                    Ner(i,j) = 1;%not to consider
                    plot([X(i,1) X(j,1)], [X(i,2) X(j,2)],'-c', 'linewidth', 2); hold on;
                end
            end
        end
        %track boundary
        %find the initial boundary edge
        %global El;%cols 1&2: edges which are already visited, col 3: edge's boundary cell number col 4: edge number/row number in that cell
        %El = [];
        %global S;
        chkV = Er;
        fv = 0;
        for i = 1:nP
            for j = 1:nP
                if i < j && Er(i,j) == 1 && Ner(i,j) == 1
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
        B = [];
        while size(Q,1)>0
            found = 0;
            for qc = 1:size(Q,1)
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
            plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-g', 'linewidth', 2); hold on;
            PVs = pv;%previous vertics, which are left column of S{q}.f1
            %explore the current struct
            while(1)
                vs = find(Er(cv,:) == 1); %all vertices that are connected to cv
                %find all vertices, other than pv, which are along boundary (remove non
                %boundary vertices
                bv = [];
                if cv == 1628 || cv == 156
                    here = 1;
                end
                for m = 1:size(vs,2)
                    k = vs(1,m);
                    chk = (El(:,1) == cv & El(:,2) == k) | (El(:,1) == k & El(:,2) == cv); %check if edge (cv,k) or (k,cv) is already visited
                    if sum(chk) > 0 && k ~= pv%the current edge is already visited, so a close boundary is found and mark it
                        el = El(chk,:);%&& sum(PVs == k)==0 %
                        if el(1,3) > q
                            %this edge was added in the Q and S to be explored
                            %latter, but it has now come to be an edge of contour
                            %found from q, so make this Q and S entry inactive so
                            %that it is not used latter
                            chkq = Q == el(1,3);
                            Qactive(chkq,1) = 0;

                            %and add this to be a nextly visited edge
                            bv = [bv; k];
                            %plot(X(bv,1), X(bv,2), 'dk'); hold on;
                            plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-y', 'linewidth', 2); hold on;
                        else%it is within the current or a previously explored 
                            B = [B; [q size(S{q}.f1,1) el(1,3:4)]];%record the boundary
                            S{q}.f3 = [S{q}.f3; el(1,3)];%update child list (a previuos paranet becomes a child, so a loop)
                            %show
                            plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-k', 'linewidth', 2); hold on;
                            continue;
                        end
                    end
                    if k ~= pv           && sum(chk) == 0 && (Ner(cv,k) == 1 || Ner(k,cv) == 1)%k is not pv, all of whose possible neigbours are not already in Vc and Er(j,k) is used only once
                    %if sum(PVs == k)==0 && sum(chk) == 0 && (Ner(cv,k) == 1 || Ner(k,cv) == 1)
                        bv = [bv; k];
                        %plot(X(bv,1), X(bv,2), 'dk'); hold on;
                         plot([X(k,1) X(cv,1)], [X(k,2) X(cv,2)],'-y', 'linewidth', 2); hold on;
                    end
                end

                if size(bv,1) == 0
                    %dead end
                    %noway to progress
                    plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-r', 'linewidth', 2); hold on;

                    %look for another boundary edge, if any
                    fv = 0;
                    bm = (Er & chkV) & (Ner == 1);
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
                        end
                    end
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
                    plot([X(pv,1) X(cv,1)], [X(pv,2) X(cv,2)],'-m', 'linewidth', 2); hold on;

                    if cv == 2935
                        here = 1;
                    end

                    PVs = [PVs;pv];            
                    if sum(PVs == cv)==1 %current vertext has already been visited in an earlier iteration, possibly a loop, so break
                        %self-loop! Update B
                        loopid = find(PVs == cv);
                        B = [B; [q size(S{q}.f1,1) q loopid]];%record the boundary
                        S{q}.f3 = [S{q}.f3; q];%update child list (a previuos paranet becomes a child, so a loop)
                        break;
                    end
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
                    for l = 1:size(bv,1)
                        k = bv(l,1);

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

            if size(Q,1) > qc
                Q = Q(qc+1:end,:);
                Qactive = Qactive(qc+1:end,:);
                if sum(Qactive,1) == 0
                    Q = [];
                    Qactive = [];
                end
            end

            if size(Q,1) <= qc || size(Q,1) == 0
                Q = [];
                Qactive = [];

                %look for another boundary edge, if any
                    fv = 0;
                    bm = (Er & chkV) & (Ner == 1);
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
        
        sids = {};
        bcount = 0;
        for bnn = 1:size(B,1)
            b = B(bnn,:);%[q size(S{q}.f1,1) el(1,3:4)]
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
        end%if bigWholeFound == 1 
    end
    bndryin{bn} = bids;
end

end