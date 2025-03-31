library(ggplot2)
rm(list = ls())
print("Scale-free network distribution")
in_file_name  <- "teste-001000.in_degree.csv"
out_file_name <- "teste-001000.out_degree.csv"


scalefree_in_nt <-read.table(in_file_name, header = T, quote = '"', 
                       row.names = NULL, 
                       stringsAsFactors = FALSE, sep=";")


scalefree_out_nt <-read.table(out_file_name, header = T, quote = '"', 
                             row.names = NULL, 
                             stringsAsFactors = FALSE, sep=";")


g <- ggplot() +
  geom_point(data = scalefree_in_nt, aes(x = degree, y = frequency , color = "In degree"), size = 3) +
  geom_point(data = scalefree_out_nt, aes(x = degree, y = frequency, color = "Out degree"), size = 3) +
  scale_x_log10()+
  scale_y_log10() +
  scale_color_manual(values = c("In degree" = "blue", "Out degree" = "red")) +  # Custom colors
  labs(color = "Legend") +  # Legend title
  theme_minimal()

print(g)