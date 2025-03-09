# Define the mean and standard deviation
mean <-50
sd <- 15

# Define a sequence of x-values
x <- seq(0, 80, length.out = 1000)  # From -4 to 4 standard deviations

# Calculate the corresponding y-values using dnorm()
y <- dnorm(x, mean = mean, sd = sd)

# Plot the normal curve
plot(x, y, type = "l", lwd = 2, col = "blue", 
     xlab = "x", ylab = "Density", 
     main = "Normal Distribution Curve (mean = 0, sd = 1)")

# Add a grid for better visualization
grid()