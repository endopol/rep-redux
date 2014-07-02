function [x, y] = plotTree(tree, doplot)

n = length(tree);

[tree, i_tree] = sort(tree);
[~, ii_tree] = sort(i_tree);

h = gcf;
% Compute points
if(nargin<5)
    u = zeros(1, n);
    v = zeros(1, n);
    
    depth = 0;
    col = 0;
    widths = [];
    
    % Extract depth and in-level order
    end_level = 0;
    for i=2:n
        
        if tree(i)>=end_level
            depth = depth + 1;
            end_level = i;
            widths = [widths, col];
            col = 0;
        end
        
        u(i) = col+1;
        v(i) = depth;
        
        col = col + 1;
        
    end
    
    xo = 0*u;
    yo = -v;
    
    
    x = xo;
    y = yo;
    
    % Iteratively compute point coordinates
    K = 500;
    R = .5;
    max_change = 10;
    
    
    niter = K*sqrt(n);
    dt = 50/(K*sqrt(n));
    for i = 1:niter
        F = soft_trunc(tree_force(x, y, tree) - R*(x - xo), max_change);
        x = x + x - xo + 2*dt*F;       
        xo = x;
        x = x-x(1);
        
%         figure(2)
%         plot(x,y,'x')
%         hold on
%         for j=2:n
%             plot([x(j) x(tree(j))], [y(j) y(tree(j))], 'r-')
%         end
%         ylim([-length(widths),0])
%         xlim([-10, 10])     
%         hold off
%         pause(0.01)
    end   
end

% dx = 0.1;
% x = round(x/dx)*dx;

figure(h)
set(gcf, 'Color', 'white')
% Plot points
Ry = max(abs(y))+.0001;
Rx = max(abs(x))+.0001;
x = x*Ry/Rx;
plot(x, y, '.b')
xlim([-Ry, Ry*1.1])
hold on

% Plot vectors
%     quiver(x, y, F1, 0*F1, 'b');
%     quiver(x, y, F2, 0*F2, 'r');

if nargin<2
    doplot = 0;
end

if doplot
    % Plot edges
    for i=2:n
        plot([x(i) x(tree(i))], [y(i) y(tree(i))], 'r-')
    end
    
    ylim([-(length(widths)+1), 0])
    plot(x, y, '.b')
    
    %title('Decision Tree')
    %axis off
end
axis off
hold off

x = x(ii_tree);
y = y(ii_tree);

end

