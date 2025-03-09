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
SN_data$total <- SN_data$susceptible + SN_data$exposed + SN_data$infected + SN_data$recovered
#Normalizing data
SN_data$susceptible_percent <- SN_data$susceptible / SN_data$total
SN_data$exposed_percent <- SN_data$exposed / SN_data$total
SN_data$infected_percent <- SN_data$infected / SN_data$total
SN_data$recovered_percent <- SN_data$recovered / SN_data$total


g <- ggplot() +
  geom_line(data = SN_data, aes(x = time, y = susceptible_percent,  color="susceptible"))+
  geom_point(data = SN_data, aes(x = time, y = susceptible_percent,  color="susceptible"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = exposed_percent,  color="exposed"))+
  geom_point(data = SN_data, aes(x = time, y = exposed_percent,  color="exposed"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = infected_percent,  color="infected"))+
  geom_point(data = SN_data, aes(x = time, y = infected_percent,  color="infected"), size=2.5) +
  
  geom_line(data = SN_data, aes(x = time, y = recovered_percent,  color="recovered"))+
  geom_point(data = SN_data, aes(x = time, y = recovered_percent,  color="recovered"), size=2.5) +
  
  coord_cartesian(ylim = c(0, 1)) +
  scale_y_continuous(breaks = seq(0, 1, by = 0.1)) +
  scale_x_continuous(breaks = seq(0, 30, by = 1)) +
  scale_color_manual(values = c("susceptible" = "blue", "exposed" = "orange", "infected" = "red", "recovered" = "green")) +  # Custom colors
  labs(color = "States:", x = "Time in days", y = "Nodes normalized (%)") +
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

#ggsave(filename = "SEIR-curve.pdf", width = 20, height = 10)
#print("File saved")
#   print(g)

# g <- ggplot() +
#   geom_point(data = scalefree_in_nt, aes(x = degree, y = frequency , color = "In degree"), size = 3) +
#   geom_point(data = scalefree_out_nt, aes(x = degree, y = frequency, color = "Out degree"), size = 3) +
#   scale_x_log10()+
#   scale_y_log10() +
#   scale_color_manual(values = c("In degree" = "blue", "Out degree" = "red")) +  # Custom colors
#   labs(color = "Legend") +  # Legend title
