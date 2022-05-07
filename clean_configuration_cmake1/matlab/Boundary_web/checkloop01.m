function [loopids1 found] = checkloop01(PVs, S, st, en)

%common node for loop
cnode = PVs(st,1);%same as PVs(en,1)
%strat and end entries in S
stq = PVs(st,2);
enq = PVs(en,2);
found = 0;
if stq == enq
    %self loop
    loopids1 = PVs(st:en,1);
    found = 1;
else
    %find position of cnode in enq edge list
    cid = find(S{enq}.f2 == cnode);
    if size(cid,1)>1 && S{enq}.f0 == stq && S{enq}.f2(1,1) == cnode && S{stq}.f2(end,1) == cnode
        cid = cid(2,1);
    end
    loopids1 = S{enq}.f2(1:cid,1);%nodes from enq
    %L = enq;%list of entries in S along the loop
    %curq = enq;
    %Stack = [];
    curq = S{enq}.f0;
    Vq = enq;
    while(1)
        %{
        children = S{curq}.f3;
        if size(children,1)>0
            %store 2nd and latter children in stack
            for c = 2:size(children,1)
                Stack = [Stack; [curq c]];
            end
            %work with first children and letter retrieve other children
            %from stack
            childq = children(1,1);
            if childq == enq%if it is the last q
                
            end
        end
        %}
        if curq == stq
            %find position of cnode in stq edge list
            cid1 = find(S{stq}.f2 == cnode);
            if size(cid1,1)>1 && S{stq}.f0 == enq && S{stq}.f2(1,1) == cnode && S{enq}.f2(end,1) == cnode
                cid1 = cid1(2,1);
            end    
            loopids2 = S{stq}.f2(cid1:end-1,1);%nodes from stq
            loopids1 = [loopids2; loopids1];
            found = 1;
            break;
        elseif size(curq,1) == 0 || sum(Vq == curq) == 1
            if size(loopids1,1)>2
                if loopids1(1,1) == loopids1(end,1)
                    found = 1;
                end
            end
            break;
        else
            loopids2 = S{curq}.f2(1:end-1,1);%nodes from stq
            loopids1 = [loopids2; loopids1];
            Vq = [Vq; curq];
            curq = S{curq}.f0;%parent of curq becomes new curq
        end
        
    end
    if found == 1
        if loopids1(1,1) == cnode
            loopids1 = loopids1(2:end,1);
        end
    end
end

end