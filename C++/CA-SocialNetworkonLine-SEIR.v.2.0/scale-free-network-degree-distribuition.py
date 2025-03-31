import sys
import matplotlib.pyplot as plt
import pandas as pd

if __name__ == "__main__":
    filename = sys.argv[1]
    df = pd.read_csv(filename, delimiter=";", engine="python")
    total = df["frequency"].sum()
    df["normalize"] = df["frequency"] / total

    plt.figure(1)
    plt.scatter(df["degree"], df["normalize"], color="blue")

    plt.ylabel("Frequency")
    plt.xlabel("Degree")
    plt.yscale('log')
    plt.xscale('log')
    plt.show()

