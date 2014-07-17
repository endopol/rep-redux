
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def plot_arc(x, y, theta, label, arrow):

    # Local Variables: f1, f2, cn, vf, ai, ca, EPS, Ra, Rn, xt, LABEL_OFF, fix, s0, label, theta, R0, v0, S, k_inv, Y, X, c0, ARROW_OFF, Sf, ARROW_T, NUM_LINES, ARROW_R, i, label_off, s, sn, arrow, v, y, x, sa, yt
    # Function calls: plot, cos, plot_arc, text, sqrt, round, nargin, zeros, diff, tan, pi, sin, norm, fill
    if nargin<5.:
        arrow = 1.
    
    
    EPS = .000001
    NUM_LINES = 30.
    ARROW_T = np.pi/12.
    ARROW_R = .05
    ARROW_OFF = .6
    LABEL_OFF = .01
    vf = np.array(np.vstack((np.hstack((np.diff(x))), np.hstack((np.diff(y))))))
    if matdiv(vf[1], vf[0])<0.:
        theta = -theta
    
    
    if vf[1] == 0.:
        theta = 0.
    
    
    S = linalg.norm(vf)
    k_inv = matdiv(S, 2.*np.sin(theta)+EPS)
    s = np.dot(k_inv, np.tan(matdiv(2.*theta, NUM_LINES)))
    fix = 1.
    if S == 0.:
        fix = 0.
        s = matdiv(.5, NUM_LINES)
        theta = np.pi
    
    
    if theta == 0.:
        s = matdiv(S, NUM_LINES)
    
    
    cn = np.cos(matdiv(np.dot(-2., theta), NUM_LINES-1.))
    sn = np.sin(matdiv(np.dot(-2., theta), NUM_LINES-1.))
    Rn = np.array(np.vstack((np.hstack((cn, -sn)), np.hstack((sn, cn)))))
    c0 = np.cos(theta)
    s0 = np.sin(theta)
    R0 = np.array(np.vstack((np.hstack((c0, -s0)), np.hstack((s0, c0)))))
    v0 = np.dot(R0, matdiv(np.dot(s, np.array(np.vstack((np.hstack((np.diff(x))), np.hstack((np.diff(y))))))), S))
    ai = np.round(np.dot(NUM_LINES, ARROW_OFF))
    X = np.zeros(1., (NUM_LINES+1.))
    Y = np.zeros(1., (NUM_LINES+1.))
    X[0] = x[0]
    Y[0] = y[0]
    v = v0
    for i in np.arange(1., (NUM_LINES)+1):
        X[int((i+1.))-1] = X[int(i)-1]+v[0]
        Y[int((i+1.))-1] = Y[int(i)-1]+v[1]
        v = np.dot(Rn, v)
        if i == ai:
            vf = matdiv(np.dot(ARROW_R, v), linalg.norm(v))
            #% arrow direction
        
        
        
    if fix:
        Sf = np.sqrt(((X[int(0)-1]-X[0])**2.+(Y[int(0)-1]-Y[0])**2.))
        X = X[0]+matdiv(np.dot(S, X-X[0]), Sf)
        Y = Y[0]+matdiv(np.dot(S, Y-Y[0]), Sf)
    
    
    plt.hold(on)
    plt.plot(X, Y, 'r-')
    ca = np.cos(ARROW_T)
    sa = np.sin(ARROW_T)
    Ra = np.array(np.vstack((np.hstack((ca, -sa)), np.hstack((sa, ca)))))
    ai = ai+2.
    label_off = np.round(matdiv(np.dot(NUM_LINES, LABEL_OFF), s))
    if nargin >= 4.:
        xt = X[int((ai-label_off))-1]
        yt = Y[int((ai-label_off))-1]
        plt.text(xt, yt, label, 'BackgroundColor', np.array(np.hstack((.9, .9, .9))), 'VerticalAlignment', 'Middle', 'HorizontalAlignment', 'Center')
    
    
    if arrow:
        f1 = np.array(np.vstack((np.hstack((X[int(ai)-1])), np.hstack((Y[int(ai)-1])))))-np.dot(Ra, vf)
        f2 = np.array(np.vstack((np.hstack((X[int(ai)-1])), np.hstack((Y[int(ai)-1])))))-linalg.solve(Ra, vf)
        plt.fill(np.array(np.hstack((X[int(ai)-1], f1[0], f2[0]))), np.array(np.hstack((Y[int(ai)-1], f1[1], f2[1]))), 'r')
    
    
    plt.hold(off)
    return 