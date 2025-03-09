
# Define a sequence of x-values
x <-  # From -4 to 4 standard deviations

# Calculate the corresponding y-values using dnorm()
#y <- dnorm(x, mean = mean, sd = sd)

# Plot the normal curve
 plot( dpois( x=0:10, lambda=4.1 ))

# Add a grid for better visualization
grid()
