function plot_arc(x, y, theta, label, arrow)

if nargin<5
    arrow=1;
end

EPS = .000001;
NUM_LINES = 30;
ARROW_T = pi/12;
ARROW_R = .05;
ARROW_OFF = .6;
LABEL_OFF = .01;

vf = [diff(x); diff(y)];
if vf(2)/vf(1)<0
    theta = -theta;
end
if vf(2)==0
    theta = 0;
end

S = norm(vf);
k_inv = S/(2*sin(theta)+EPS);

s = k_inv*tan(2*theta/NUM_LINES);

fix = 1;
if S==0
    fix = 0;
    s = .5/NUM_LINES;
    theta = pi;
end

if theta==0
    s = S/NUM_LINES;
end

cn = cos(-2*theta/(NUM_LINES-1));
sn = sin(-2*theta/(NUM_LINES-1));
Rn = [cn, -sn; sn, cn];     

c0 = cos(theta);
s0 = sin(theta);
R0 = [c0, -s0; s0, c0];
v0 = R0*(s*[diff(x); diff(y)]/S);

ai = round(NUM_LINES*ARROW_OFF);
X = zeros(1, NUM_LINES+1);
Y = zeros(1, NUM_LINES+1);
X(1) = x(1); Y(1) = y(1);
v = v0;
for i=1:NUM_LINES
    X(i+1) = X(i) + v(1);
    Y(i+1) = Y(i) + v(2);
    v = Rn*v;
    if i==ai
        vf = ARROW_R*v/norm(v); % arrow direction
    end
end
if fix
    Sf = sqrt((X(end)-X(1))^2 + (Y(end)-Y(1))^2);
    X = X(1) + S*(X-X(1))/Sf;
    Y = Y(1) + S*(Y-Y(1))/Sf;
end

hold on
plot(X,Y,'r-');
ca = cos(ARROW_T);
sa = sin(ARROW_T);
Ra = [ca, -sa; sa, ca];

ai = ai+2;
label_off = round(NUM_LINES*LABEL_OFF/s);
if nargin>=4        
    xt = X(ai-label_off);
    yt = Y(ai-label_off);
    text(xt, yt, label, 'BackgroundColor',[.9 .9 .9], 'VerticalAlignment', 'Middle', 'HorizontalAlignment', 'Center')
end

if arrow
f1 = [X(ai); Y(ai)]-Ra*vf;
f2 = [X(ai); Y(ai)]-Ra\vf;
fill([X(ai), f1(1), f2(1)], [Y(ai), f1(2), f2(2)], 'r')
end
hold off

end

