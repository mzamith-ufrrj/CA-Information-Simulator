library(ggplot2)
library(latex2exp)
rm(list = ls())
normalize_dataset <- function(dataset){
  #Normalizing data
  dataset$total <- dataset$susceptible + dataset$exposed + dataset$infected + dataset$recovered
  dataset$susceptible_percent <- dataset$susceptible / dataset$total
  dataset$exposed_percent <- dataset$exposed / dataset$total
  dataset$infected_percent <- dataset$infected / dataset$total
  dataset$recovered_percent <- dataset$recovered / dataset$total
  
  return (dataset);
}
# R --vanilla < SEIR-model.R 
# Rscript SEIR-model.R
print("SEIR MODEL - Análise do Epsilon no tempo - curva do suscetível")
limits_x <- 30
file_name_dateset_10  <- "statistic.param-exp-mean-0_1.csv"
file_name_dateset_25  <- "statistic.param-exp-mean-0_25.csv"
file_name_dateset_50  <- "statistic.param-exp-mean-0_50.csv"
file_name_dateset_75  <- "statistic.param-exp-mean-0_75.csv"
file_name_dateset_90  <- "statistic.param-exp-mean-0_9.csv"
df_10 <-read.table(file_name_dateset_10, header = T, quote = '"',  row.names = NULL,  stringsAsFactors = FALSE, sep=";")
df_25 <-read.table(file_name_dateset_25, header = T, quote = '"',  row.names = NULL,  stringsAsFactors = FALSE, sep=";")
df_50 <-read.table(file_name_dateset_50, header = T, quote = '"',  row.names = NULL,  stringsAsFactors = FALSE, sep=";")
df_75 <-read.table(file_name_dateset_75, header = T, quote = '"',  row.names = NULL,  stringsAsFactors = FALSE, sep=";")
df_90 <-read.table(file_name_dateset_90, header = T, quote = '"',  row.names = NULL,  stringsAsFactors = FALSE, sep=";")

df_10 <- df_10[df_10$time <= limits_x, ]
df_25 <- df_25[df_25$time <= limits_x, ]
df_50 <- df_50[df_50$time <= limits_x, ]
df_75 <- df_75[df_75$time <= limits_x, ]
df_90 <- df_90[df_90$time <= limits_x, ]
df_10 <- normalize_dataset(df_10);
df_25 <- normalize_dataset(df_25);
df_50 <- normalize_dataset(df_50);
df_75 <- normalize_dataset(df_75);
df_90 <- normalize_dataset(df_90);


#my_title <-"$\\epsilon \\times \\alpha \\times \\gamma$"
g <- ggplot() +
  geom_point(data = df_10, aes(x = time, y = ave_epsilon,  color="df_10"), size=2.5)+
  geom_line(data = df_10, aes(x = time, y = ave_epsilon,  color="df_10"))+
  
  geom_point(data = df_25, aes(x = time, y = ave_epsilon,  color="df_25"), size=2.5) +
  geom_line(data = df_25, aes(x = time, y = ave_epsilon,  color="df_25"))+
  
  geom_point(data = df_50, aes(x = time, y = ave_epsilon,  color="df_50"), size=2.5) +
  geom_line(data = df_50, aes(x = time, y = ave_epsilon,  color="df_50"))+

  geom_point(data = df_75, aes(x = time, y = ave_epsilon,  color="df_75"), size=2.5) +
  geom_line(data = df_75, aes(x = time, y = ave_epsilon,  color="df_75"))+

  geom_point(data = df_90, aes(x = time, y = ave_epsilon,  color="df_90"), size=2.5) +
  geom_line(data = df_90, aes(x = time, y = ave_epsilon,  color="df_90"))+
  
  
  
  coord_cartesian(ylim = c(0, 1)) +
  scale_y_continuous(breaks = seq(0, 1, by = 0.1 )) +
  scale_x_continuous(breaks = seq(0, limits_x, by = 1)) +
  scale_color_manual(values = c (
    "df_10" = "green", 
    "df_25" = "blue", 
    "df_50" = "red", 
    "df_75" = "yellow",
    "df_90" = "black"),
    
    labels = c(
      df_10 = TeX("$\\lambda = 10$"),
      df_25 = TeX("$\\lambda = 4$"),
      df_50 = TeX("$\\lambda = 2$"),
      df_75 = TeX("$\\lambda = 1.3333$"),
      df_90 = TeX("$\\lambda = 1.1111$")
    )) +  # Custom colors
  labs(color = "States:", x = "Time", y = "Average value") +
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



#file_name_pdf <- "curva-epsilon.pdf"
#ggsave(filename = file_name_pdf, width = 10, height = 8)

# SN_data <-read.table(file_name_dateset, header = T, quote = '"', 
#                      row.names = NULL, 
#                      stringsAsFactors = FALSE, sep=";")
# SN_data$alpha_gamma =  SN_data$ave_alpha / SN_data$ave_gamma
# #Total value calculated to obtain normalize graphics
# 
# 
# 
# 

# #print("File saved")
