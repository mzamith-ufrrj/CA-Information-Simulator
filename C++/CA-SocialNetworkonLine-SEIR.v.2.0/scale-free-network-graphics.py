from pathlib import Path
import struct
import sys
import matplotlib.pyplot as plt
import networkx as nx
if __name__ == "__main__":
    filename = sys.argv[1]
    print("Graph topology - input file: {}".format(filename))
    data = Path(filename).read_bytes()
    p_end = len(data)
    pos = 0
    delta  = 8
    nodes = int.from_bytes(data[pos:pos+delta], byteorder='little', signed=False)
    #G = nx.Graph()
    G = nx.DiGraph()
    for i in range(0, nodes):
        G.add_node(i)
    print("Nodes: {}".format(nodes))




    while (pos + delta < p_end):
        pos = pos + delta
        source = int.from_bytes(data[pos:pos+delta], byteorder='little', signed=False)
        pos = pos + delta
        target  = int.from_bytes(data[pos:pos+delta], byteorder='little', signed=False)
        G.add_edge(source, target)

# Visualiza a rede gerada
    plt.figure(figsize=(10, 8))
    nx.draw(G, pos=nx.spring_layout(G), node_size=80, edge_color="gray")
    plt.title("Free-scale network", fontsize=16)
    plt.show()