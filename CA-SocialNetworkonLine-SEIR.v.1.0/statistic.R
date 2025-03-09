#library(lubridate)
library(dplyr)
library(xtable)
library(plyr)
library(ggplot2)
rm(list = ls())
# df <-read.table("statistic-E_1.00-A_0.35-D_0.50.csv", header = T, quote = '"', 
#                        row.names = NULL, 
#                        stringsAsFactors = FALSE, sep=";")

df <-read.table("statistic.csv", header = T, quote = '"', 
                row.names = NULL, 
                stringsAsFactors = FALSE, sep=";")

g <- ggplot2::ggplot(data=df) +
  geom_line(aes(x=time, y=susceptible, colour="susceptible" ))+
  geom_point(aes(x=time, y=susceptible, colour="susceptible"), shape=15, size=4)+
  geom_line(aes(x = time, y=exposed, colour="exposed")) +
  geom_point(aes(x = time, y=exposed, colour="exposed"), shape=16, size=4) +
  geom_line(aes(x = time, y=infected, colour="infected")) +
  geom_point(aes(x = time, y=infected, colour="infected"), shape=17, size=4) +
  geom_line(aes(x = time, y=recovered, colour="recovered")) +
  geom_point(aes(x = time, y=recovered, colour="recovered"), shape=18, size=4) +
  scale_colour_manual(values = c( "green", "yellow", "blue", "red"),  name = "STATE") +
  #geom_bar(stat="identity")+
  coord_cartesian(xlim = c(0, 20)) +
  scale_x_continuous(breaks = seq(0, 20, by = 1)) +
  labs(x = "Time (t)", y = "People") +
  #geom_text(aes(y=label_ypos, label=total), vjust=0, color="black", size=4)+
  #scale_fill_manual(values=c('green', 'blue', 'yellow', 'red', 'cyan'),  name = "STATUS")+
  #scale_fill_brewer(palette="Blues", name = "STATUS")+
  
  theme_bw() +
  theme(#legend.position = "none",
    legend.position = "top",
    axis.text.x = element_text(angle = 45, vjust = 1, hjust=0.8),
    text=element_text(size=16), #change font size of all text
    axis.text=element_text(size=20), #change font size of axis text
    axis.title=element_text(size=22), #change font size of axis titles
    plot.title=element_text(size=16), #change font size of plot title
    legend.text=element_text(size=16), #change font size of legend text
    legend.title=element_text(size=22))
print(g)