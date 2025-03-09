library(igraph)
library(ggraph)
library(tidygraph)
rm(list = ls())
print("Dorogovtsev–Mendes (DM) Model")
# Generate a directed BA model graph with 50 nodes
g <- sample_pa_age(n = 100, pa.exp = 1, aging.exp = -1, directed = TRUE)

# Convert to a tidygraph object for ggraph
g_tbl <- as_tbl_graph(g)

gg <- ggraph(g_tbl, layout = "fr") +  # Fruchterman-Reingold layout for better spacing
  geom_edge_link(aes(end_cap = circle(3, 'mm')), 
                 arrow = arrow(length = unit(2, 'mm')), 
                 color = "gray") +  # Add arrows to edges
  geom_node_point(size=2.5, color="blue") +  # Nodes in blue
  #geom_node_text(aes(label = name), repel = TRUE, size = 4) +  # Node labels
  theme_void() +
  ggtitle("Dorogovtsev–Mendes (DM) Model")
  
print(gg)
ggsave(filename = "DM-MODEL.pdf", plot=gg,  width = 20, height = 10)