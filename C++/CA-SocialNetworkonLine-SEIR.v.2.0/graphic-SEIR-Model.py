import pandas as pd
import matplotlib.pyplot as plt
import sys
if __name__ == "__main__":
    filename = sys.argv[1]
    df = pd.read_csv(filename, delimiter=";", engine="python")
    total = df.iloc[1]["susceptible"] + df.iloc[1]["recovered"] + df.iloc[1]["infected"] + df.iloc[1]["exposed"]
    print(total)
    plt.figure(figsize=(10, 8))
    plt.plot(df["time"], df["susceptible"]/total, linestyle='--', marker='o', color='blue', label='susceptible')
    plt.plot(df["time"], df["exposed"]/total, linestyle='--', marker='p', color='orange', label='exposed')
    plt.plot(df["time"], df["infected"]/total, linestyle='--', marker='*', color='red', label='infected')
    plt.plot(df["time"], df["recovered"]/total, linestyle='--', marker='D', color='green', label='recovered')
    plt.legend()
    plt.xlim(0, 30)
    plt.title("File: {}".format(filename))
    plt.ylabel("Nodes normalized (%)")
    plt.xlabel("Time in days")
    plt.show()