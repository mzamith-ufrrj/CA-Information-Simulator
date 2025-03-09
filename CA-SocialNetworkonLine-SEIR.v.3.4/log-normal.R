# Define the mean" and standard deviation
library(latex2exp)
rm(list = ls())
mi <- .15
sigma <- 0.75
title <- sprintf("Log-normal distribuition curve ($\\mu = %.2f / $\\sigma = %.2f)", mi, sigma)
filename <- sprintf("pdf-log-normal-%.2f-%.2f.pdf", mi, sigma)

# Define a sequence of x-values
x <- seq(0, 100, length.out = 1000)  # From -4 to 4 standard deviations

# Calculate the corresponding y-values using dnorm()
y <- dlnorm(x, meanlog = mi , sdlog = sigma)

# Plot the normal curve
plot(x, y, type = "l", lwd = 2, col = "blue", 
     xlab = "x", ylab = "Density", 
     main = TeX(title))

# Add a grid for better visualization
grid()
#dev.print(pdf, filename)
#dev.off()
