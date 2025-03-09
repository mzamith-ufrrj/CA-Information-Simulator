library(ggplot2)
rm(list = ls())
# R --vanilla < SEIR-model.R 
# Rscript SEIR-model.R
print("SEIR MODEL")
file_name  <- "statistic.config-sample.csv"


SN_data <-read.table(file_name, header = T, quote = '"', 
                     row.names = NULL, 
                     stringsAsFactors = FALSE, sep=";")

#Total value calculated to obtain normalize graphics



g <- ggplot() +
  geom_line(data = SN_data, aes(x = time, y = ave_alpha,  color="alpha"))+
  geom_point(data = SN_data, aes(x = time, y = ave_alpha,  color="alpha"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = ave_gamma,  color="gamma"))+
  geom_point(data = SN_data, aes(x = time, y = ave_gamma,  color="gamma"), size=2.5) +
  
  coord_cartesian(ylim = c(0, 1)) +
  #scale_y_continuous(breaks = seq(0, 1, by = 0.1)) +
  scale_x_continuous(breaks = seq(0, 30, by = 1)) +
  scale_color_manual(values = c("alpha" = "blue", "gamma" = "red")) +  # Custom colors
  labs(color = "States:", x = "Time in days", y = "Average value") +
  theme_bw() + 
  theme(#legend.position = "none",
    legend.position = "top",
    axis.text.x = element_text(angle = 90, vjust = 1, hjust=0.8),
    text=element_text(size=16), #change font size of all text
    axis.text=element_text(size=14), #change font size of axis text
    axis.title=element_text(size=16), #change font size of axis titles
    plot.title=element_text(size=16), #change font size of plot title
    legend.text=element_text(size=14), #change font size of legend text
    legend.title=element_text(size=14))

print(g)    

#ggsave(filename = "SEIR-alpha-gamma.pdf", width = 20, height = 10)
#print("File saved")
