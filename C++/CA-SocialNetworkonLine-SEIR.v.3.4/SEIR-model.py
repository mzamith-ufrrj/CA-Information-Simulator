import numpy as np
from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt

# Define the SEIR model differential equations
def seir_model(t, y, beta, sigma, gamma, N):
    S, E, I, R = y
    dSdt = -beta * S * I / N
    dEdt = beta * S * I / N - sigma * E
    dIdt = sigma * E - gamma * I
    dRdt = gamma * I
    return [dSdt, dEdt, dIdt, dRdt]

# Parameters
beta = 0.3  # Transmission rate
sigma = 0.1  # Rate of becoming infectious (1/incubation period)
gamma = 0.05  # Recovery rate
N = 1000  # Total population

# Initial conditions: S0, E0, I0, R0
S0 = 990
E0 = 10
I0 = 0
R0 = 0
initial_conditions = [S0, E0, I0, R0]

# Time span (in days)
t_span = (0, 200)
t_eval = np.linspace(0, 200, 200)

# Solve the differential equations
solution = solve_ivp(seir_model, t_span, initial_conditions, args=(beta, sigma, gamma, N), t_eval=t_eval)

# Extract the results
S = solution.y[0]
E = solution.y[1]
I = solution.y[2]
R = solution.y[3]
t = solution.t

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(t, S, label='Susceptible')
plt.plot(t, E, label='Exposed')
plt.plot(t, I, label='Infectious')
plt.plot(t, R, label='Recovered')
plt.xlabel('Time (days)')
plt.ylabel('Number of individuals')
plt.title('SEIR Model')
plt.legend()
plt.grid()
plt.show()
