library(ggplot2)
library(latex2exp)
rm(list = ls())
# R --vanilla < SEIR-model.R 
# Rscript SEIR-model.R

print("SEIR MODEL")
file_name_dateset1 <- "statistic.sample-I.csv"
SN_data <-read.table(file_name_dateset1, header = T, quote = '"',
                             row.names = NULL, 
                             stringsAsFactors = FALSE, sep=";")


#Total value calculated to obtain normalize graphics
 SN_data$total <- SN_data$susceptible + SN_data$exposed + SN_data$infected + SN_data$recovered
#Normalizing data
SN_data$susceptible_percent <- SN_data$susceptible / SN_data$total
SN_data$exposed_percent <- SN_data$exposed / SN_data$total
SN_data$infected_percent <- SN_data$infected / SN_data$total
SN_data$recovered_percent <- SN_data$recovered / SN_data$total


#Normalizing data

g <- ggplot() +
  geom_line(data = SN_data, aes(x = time, y = susceptible_percent,  color="susceptible"))+
  geom_point(data = SN_data, aes(x = time, y = susceptible_percent,  color="susceptible"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = exposed_percent,  color="exposed"))+
  geom_point(data = SN_data, aes(x = time, y = exposed_percent,  color="exposed"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = infected_percent,  color="infected"))+
  geom_point(data = SN_data, aes(x = time, y = infected_percent,  color="infected"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = recovered_percent,  color="recovered"))+
  geom_point(data = SN_data, aes(x = time, y = recovered_percent,  color="recovered"), size=2.5) +
  
#  coord_cartesian(ylim = c(0, 1000000)) +
  coord_cartesian(ylim = c(0, 1)) +
  #scale_y_continuous(breaks = seq(0, 1000000, by = 100000)) +
  scale_y_continuous(breaks = seq(0, 1, by = 0.1)) +
  scale_x_continuous(breaks = seq(0, 35, by = 1)) +
  scale_color_manual(values = c("susceptible" = "green", "exposed" = "orange", "infected" = "red", "recovered" = "blue")) +  # Custom colors
  #BA-CELL-10000-P-1-C-6__Ep0_1__AAV-10_ASD-2_5__GAV-10-GSD-2_5
  #labs(title = TeX(my_title), color = "States:", x = "Time", y = "Cells (users)") +
  labs(color = "States:", x = "Time", y = "Cells (users)") +
  #statistic.BA-CELL-1000000-P-1-C-6__Ep0_1__AAV-10_ASD-2_5__GAV-10-GSD-2_5.csv
  theme_bw() + 
  theme(#legend.position = "none",
    legend.position = "top",
    axis.text.x = element_text(angle = 45, vjust = 1, hjust=0.8),
    text=element_text(size=16), #change font size of all text
    axis.text=element_text(size=14), #change font size of axis text
    axis.title=element_text(size=16), #change font size of axis titles
    plot.title=element_text(size=20), #change font size of plot title
    legend.text=element_text(size=14), #change font size of legend text
    legend.title=element_text(size=14))

print(g)    
#ggsave("plot.pdf", plot = p, width = 6.5, height = 4.5, units = "in", device = "pdf")
ggsave(filename = file_name_pdf, width = 6.5, height = 4.5, units = "in")
#ggsave(filename = file_name_pdf, width = 20, height = 16)
