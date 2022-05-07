function [id flgr] = checkSubset(loopids, loopids1)

id = [];
flgr = 0;
sz1 = size(loopids1,1);
for  i = 1:size(loopids,2)
    loopids2 = loopids{i};
    sz2 = size(loopids2,1);
    int = intersect(loopids1, loopids2);
    szi = size(int,1);    
    if size(int,1)>0
        dset = setdiff(loopids2, int);%to consider repeated nodes
        if sz1>szi && (szi==sz2 || size(dset,1) == 0)
            %loopids2 is fully contained in loopids1, so loopids2 will be
            %replaced by loopids1
            id = i;
            flgr = -1;
            break;
        else
            dset = setdiff(loopids1, int);%to consider repeated nodes
            if sz2>=szi && (szi==sz1 || size(dset,1) == 0) 
            %loopids1 is fully contained in loopids2, so loopids1 will NOT be
            %replaced by loopids2
            id = i;
            flgr = 1;
            break;
            end
        end
    else
    end
end

end