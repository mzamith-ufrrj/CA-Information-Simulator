# Define the mean" and standard deviation
library(latex2exp)
rm(list = ls())
mi <- 1.0
sigma <- 0.1
title <- sprintf("Log-normal distribuition curve ($\\mu = %.2f / $\\sigma = %.2f)", mi, sigma)
filename <- sprintf("pdf-log-normal-%.2f-%.2f.pdf", mi, sigma)

# Define a sequence of x-values
x <- seq(0, 20, length.out = 1000)  # From -4 to 4 standard deviations

# Calculate the corresponding y-values using dnorm()
y <- dlnorm(x, meanlog = mi , sdlog = sigma)

# Plot the normal curve
plot(x, y, type = "l", lwd = 2, col = "blue", 
     xlab = "x", ylab = "Density")

# Add a grid for better visualization
grid()
dev.print(pdf, filename)
dev.off()
