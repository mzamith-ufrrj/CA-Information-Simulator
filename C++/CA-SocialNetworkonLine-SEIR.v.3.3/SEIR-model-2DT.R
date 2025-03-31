library(ggplot2)
library(latex2exp)
rm(list = ls())
# R --vanilla < SEIR-model.R 
# Rscript SEIR-model.R
print("SEIR MODEL")
file_name_dateset1  <- "statistic.Log-Normal-9.csv"
file_name_dateset2  <- "statistic.Log-Normal-8.csv"
#file_name_pdf <- "SEIR-Model.BA-CELL-1000000-P-1-C-6__Ep0_25__AAV-10_ASD-2_5__GAV-10-GSD-2_5.pdf"
my_title <-"$\\epsilon$ = 0.25 / N($\\mu_{\\alpha}$ = 10, $\\sigma_{\\alpha}$ = 2.5) / N($\\mu_{\\gamma}$ = 10, $\\sigma_{\\gamma}$ = 2.5)"

SN_data1 <-read.table(file_name_dateset1, header = T, quote = '"',
                             row.names = NULL, 
                             stringsAsFactors = FALSE, sep=";")
SN_data2 <-read.table(file_name_dateset2, header = T, quote = '"',
                             row.names = NULL, 
                             stringsAsFactors = FALSE, sep=";")

# #Total value calculated to obtain normalize graphics
# SN_data$total <- SN_data$susceptible + SN_data$exposed + SN_data$infected + SN_data$recovered
# #Normalizing data
# SN_data$susceptible_percent <- SN_data$susceptible / SN_data$total
# SN_data$exposed_percent <- SN_data$exposed / SN_data$total
# SN_data$infected_percent <- SN_data$infected / SN_data$total
# SN_data$recovered_percent <- SN_data$recovered / SN_data$total


#Normalizing data

g <- ggplot() +
  geom_line(data = SN_data1, aes(x = time, y = susceptible,  color="susceptible - ds1"))+
  geom_point(data = SN_data1, aes(x = time, y = susceptible,  color="susceptible - ds1"), size=2.5) +
  
  geom_line(data = SN_data1, aes(x = time, y = exposed,  color="exposed - ds1"))+
  geom_point(data = SN_data1, aes(x = time, y = exposed,  color="exposed - ds1"), size=2.5) +
  
  geom_line(data = SN_data1, aes(x = time, y = infected,  color="infected - ds1"))+
  geom_point(data = SN_data1, aes(x = time, y = infected,  color="infected - ds1"), size=2.5) +
  
  geom_line(data = SN_data1, aes(x = time, y = recovered,  color="recovered - ds1"))+
  geom_point(data = SN_data1, aes(x = time, y = recovered,  color="recovered - ds1"), size=2.5) +
#-----------
  geom_line(data = SN_data2, linetype = "dashed", aes(x = time, y = susceptible,  color="susceptible - ds2"))+
  geom_point(data = SN_data2, shape = 17,  aes(x = time, y = susceptible,  color="susceptible - ds2"), size=2.5) +
  
  geom_line(data = SN_data2, linetype = "dashed", aes(x = time, y = exposed,  color="exposed - ds2"))+
  geom_point(data = SN_data2, shape = 17,  aes(x = time, y = exposed,  color="exposed - ds2"), size=2.5) +
  
  geom_line(data = SN_data2, linetype = "dashed", aes(x = time, y = infected,  color="infected - ds2"))+
  geom_point(data = SN_data2, shape = 17,  aes(x = time, y = infected,  color="infected - ds2"), size=2.5) +
  
  geom_line(data = SN_data2, linetype = "dashed", aes(x = time, y = recovered,  color="recovered - ds2"))+
  geom_point(data = SN_data2, shape = 17,  aes(x = time, y = recovered,  color="recovered - ds2"), size=2.5) +

#-----------  
  coord_cartesian(ylim = c(0, 1000000)) +
  scale_y_continuous(breaks = seq(0, 1000000, by = 100000)) +
  scale_x_continuous(limits = c(1, 30), breaks = seq(0, 30, by = 1)) +
  #scale_color_manual(values = c("susceptible - ds1" = "blue", "exposed - ds1" = "orange", "infected - ds1" = "red", "recovered - ds1" = "green")) +  # Custom colors
  #scale_color_manual(values = c("susceptible - ds1" = "blue", "exposed - ds1" = "orange", "infected - ds1" = "red", "recovered - ds1" = "green", "susceptible - ds2" = "lightblue", "exposed - ds2" = "lightorange", "infected - ds2" = "lightred", "recovered - ds2" = "lightgreen")) +  # Custom colors
  scale_color_manual(values = c("susceptible - ds1" = "blue", "exposed - ds1" = "orange", "infected - ds1" = "red", "recovered - ds1" = "green", "susceptible - ds2" = "darkblue", "exposed - ds2" = "darkorange", "infected - ds2" = "darkred", "recovered - ds2" = "darkgreen")) +  # Custom colors
  #BA-CELL-10000-P-1-C-6__Ep0_1__AAV-10_ASD-2_5__GAV-10-GSD-2_5
  labs(title = TeX(my_title), color = "States:", x = "Time", y = "Cells (users)") +
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

#ggsave(filename = file_name_pdf, width = 20, height = 16)
#print("File saved")
#   print(g)

# g <- ggplot() +
#   geom_point(data = scalefree_in_nt, aes(x = degree, y = frequency , color = "In degree"), size = 3) +
#   geom_point(data = scalefree_out_nt, aes(x = degree, y = frequency, color = "Out degree"), size = 3) +
#   scale_x_log10()+
#   scale_y_log10() +
#   scale_color_manual(values = c("In degree" = "blue", "Out degree" = "red")) +  # Custom colors
#   labs(color = "Legend") +  # Legend title
