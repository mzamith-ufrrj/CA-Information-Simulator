import pandas as pd
import matplotlib.pyplot as plt
import sys
if __name__ == "__main__":
    filename = sys.argv[1]
    df = pd.read_csv(filename, delimiter=";", engine="python")

    plt.figure(figsize=(10, 8))
    plt.plot(df["time"], df["ave_alpha"], linestyle='--', marker='o', color='blue', label=r'$\bar{\alpha_i}$')
    plt.plot(df["time"], df["ave_gamma"], linestyle='--', marker='+', color='green', label=r'$\bar{\gamma_i}$')

    plt.legend()
    plt.xlim(0, 30)
    plt.title("File:    {}".format(filename))
    #plt.ylabel(r'$\bar{\alpha}$')
    plt.xlabel("Time in days")
    plt.show()