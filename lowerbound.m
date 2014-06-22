function max_ambig = lowerbound(fsm)
%LOWERBOUND  lower bound on required number of states

n = length(fsm);
mapping = [];

for i=1:n
    mapping = [mapping; fsm{i}];        
end

max_ambig = 0;
B = unique(mapping(:,1));
for i=1:length(B)
    curr = (mapping(:,1)==B(i));
   
    all_deci = mapping(curr, 2);
    ambig = length(unique(all_deci));
    max_ambig = max(max_ambig, ambig);
end


end

