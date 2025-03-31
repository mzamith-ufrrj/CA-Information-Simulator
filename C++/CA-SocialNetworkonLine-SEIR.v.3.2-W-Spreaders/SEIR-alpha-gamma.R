library(ggplot2)
rm(list = ls())
# R --vanilla < SEIR-model.R 
# Rscript SEIR-model.R
print("SEIR MODEL")
file_name  <- "statistic.BA-CELL-1000000-P-1-C-6__Ep0_1__AAV-10_ASD-2_5__GAV-10-GSD-2_5.csv"
file_name_pdf <- "alpha_gamma.BA-CELL-1000000-P-1-C-6__Ep0_1__AAV-10_ASD-2_5__GAV-10-GSD-2_5.pdf"

SN_data <-read.table(file_name, header = T, quote = '"', 
                     row.names = NULL, 
                     stringsAsFactors = FALSE, sep=";")
SN_data$alpha_gamma =  SN_data$ave_alpha / SN_data$ave_gamma
#Total value calculated to obtain normalize graphics



g <- ggplot() +
  geom_line(data = SN_data, aes(x = time, y = ave_alpha,  color="alpha"))+
  geom_point(data = SN_data, aes(x = time, y = ave_alpha,  color="alpha"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = ave_gamma,  color="gamma"))+
  geom_point(data = SN_data, aes(x = time, y = ave_gamma,  color="gamma"), size=2.5) +
  

  
  #coord_cartesian(ylim = c(0, 1)) +
  #scale_y_continuous(breaks = seq(0, 1, by = 0.1)) +
  scale_x_continuous(breaks = seq(0, 50, by = 1)) +
  scale_color_manual(values = c("alpha" = "blue", "gamma" = "red")) +  # Custom colors
  labs(color = "States:", x = "Time", y = "Average value") +
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

ggsave(filename = file_name_pdf, width = 20, height = 10)
#print("File saved")
