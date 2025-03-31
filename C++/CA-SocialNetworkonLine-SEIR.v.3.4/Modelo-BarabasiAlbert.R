library(igraph)
library(ggraph)
library(tidygraph)
rm(list = ls())
args<-commandArgs(TRUE)
nodes <- as.numeric(args[1]) #1000
n_power <- as.numeric(args[2]) #1
m_connection <- as.numeric(args[3]) # 6 
is_save_pajek <- TRUE
is_save_plot  <- FALSE
file_label <- sprintf("BA-N-%d-P-%d-C-%d", nodes, n_power, m_connection);
if (is_save_pajek){
  file_graph <- sprintf("%s.net", file_label)  
}


if (is_save_plot){
  file_plot <- sprintf("%s.pdf", file_label)  
}


print("Barabási–Albert (BA) Model")

################################################################################
################################################################################
print("Build graph")
g <- barabasi.game(nodes, power = n_power, m = m_connection, directed = TRUE)
# 
if (is_save_pajek){
  cat("Saving graph [", file_graph, "]\n")
  write_graph(g, file = file_graph, format = "pajek")
  print("Graph Saved")
}

if (is_save_plot){
  cat("Ploting graph [", file_plot, "]\n")
   
  g_tbl <- as_tbl_graph(g)
  gg <- ggraph(g_tbl, layout = "fr") +  # Fruchterman-Reingold layout for better spacing
    geom_edge_link(aes(end_cap = circle(3, 'mm')),
                   arrow = arrow(length = unit(2, 'mm')),
                   color = "gray") +  # Add arrows to edges
    geom_node_point(size=2.5, color="blue") +  # Nodes in blue
    #geom_node_text(aes(label = name), repel = TRUE, size = 4) +  # Node labels
    theme_void() +
    ggtitle("Directed Scale-Free Network (BA Model)")
    print(gg)
    #ggsave(filename = file_plot, plot=gg,  width = 20, height = 10)
    #print("Graph Saved")
}


  
