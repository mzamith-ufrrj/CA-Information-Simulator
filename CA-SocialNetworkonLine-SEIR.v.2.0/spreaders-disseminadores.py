import math

import matplotlib.pyplot as plt
import numpy as np
def derivada(prop, K_in, inte, t):
    #a = np.log(prop)
    #b = np.log(K_in)
    #c = np.log(inte)
    #d = np.log(t)
    #return np.exp(-(a + b + c + d))
    return np.exp(-((1-inte) * t ))

def euler(propagadores, H, times, prop, K_in, inte):
    y0 = propagadores
    y1 = 0
    T = []
    Y = []

    T.append(0)
    Y.append(y0)

    for t in range(1, times):
        T.append(t)
        d = derivada(prop, K_in, inte, t)
        y1 = y0 + H * d;
        Y.append(math.floor(y1 + 0.5))
        y0 = y1

    return T, Y
if __name__ == "__main__":


    # define data values
    spreaders = 1
    prop = 0.01
    K_out = 10 #10 pessoas eu sigo
    K_in  = 10 #10 pessoas me seguem
    K = K_in / K_out
    inte = 0.1
    H = K
    time_steps = 21
    T1, Y1 = euler(spreaders, H, time_steps, prop, K_in, inte)

    K_out = 10  # 10 pessoas eu sigo
    K_in = 30  # 10 pessoas me seguem
    K = K_in / K_out
    inte = 0.5
    H = K
    T2, Y2 = euler(spreaders, H, time_steps, prop, K_in, inte)


    plt.plot(T1, Y1)  # Plot the chart
    plt.plot(T2, Y2)  # Plot the chart

    plt.grid()
    plt.legend(["Y1", "Y2"])
    plt.show()  # display