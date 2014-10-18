function [x_list, C] = vert_hist(varargin)

n_args = length(varargin);
disp(n_args)
n_colors = floor(n_args/3);

min_y = inf; max_y = 0;
x_list = [];
for i=1:n_colors
    X = varargin{(i-1)*3+1};
    x_list = unique([x_list(:); X(:)]);
    
    Y = varargin{(i-1)*3+2};
    min_y = min(min_y, min(Y));
    max_y = max(max_y, max(Y));
end

x_num = length(x_list);
bar_width = min(diff(x_list));

if mod(n_args, 3)==1
    nbins = varargin{n_args};
    C = linspace(min_y, max_y, nbins);
else
    C = (min_y:max_y);
end

y_num = length(C);
bar_height = min(diff(C))*.8;
maxbin = 0;
rects = zeros(x_num*y_num, n_colors, 3);
for k=1:n_colors
    X = varargin{(k-1)*3+1};
    Y = varargin{(k-1)*3+2};
   
    rectnum = 0; maxbin = 0;
    for i=1:x_num
        
        x = x_list(i);
        curr = (x==X);
        
        for j = 1:y_num
            
            y = C(j);
            curr_y = sum(curr & ((Y-y)<bar_height) & ((y-Y)<=bar_height));
            
            if curr_y==0
                continue
            end
            
            rectnum = rectnum + 1;
            rects(rectnum, k, :) = [x,y, curr_y];
            maxbin = max(maxbin, curr_y);
        end        
    end
end
    
for k=1:n_colors
    color = varargin{(k-1)*3+3};
        
    for i=1:(x_num*y_num)
        x = rects(i,k,1); y = rects(i,k,2);
        curr_width = 0.8*bar_width*(rects(i,k,3)/maxbin);
        
        if curr_width==0
            continue
        end
        
        h = rectangle('Position', [x,y+((k-1)/n_colors - 1/2)*bar_height, curr_width, bar_height/(n_colors)]);
        hold on
        set(h, 'FaceColor', color)
        set(h, 'EdgeColor', color)
    end
end


end