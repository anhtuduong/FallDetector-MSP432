import matplotlib.pyplot as plot
import matplotlib.ticker as ticker

import pandas as pd

Stat = pd.read_csv("Stationary.csv", names = ["TIME","X","Y","Z"])
WalkingClock = pd.read_csv("WalkingClock.csv", names = ["TIME","X","Y","Z"])
FreeFall = pd.read_csv("FreeFall.csv", names = ["TIME","X","Y","Z"])
Pendolum = pd.read_csv("Pendulum.csv", names = ["TIME","X","Y","Z"])
Walking = pd.read_csv("Walking.csv", names = ["TIME","X","Y","Z"])
randRot = pd.read_csv("randRot.csv", names = ["TIME","X","Y","Z"])

def plotting(name, title):
    figure, ax = plot.subplots(figsize=(10, 5))

    # customizations of the lines
    ax.scatter(name['TIME'], name['X'], label="X", color="orange", marker="v")
    ax.scatter(name['TIME'], name['Y'], label="Y", color="blue", marker="o")
    ax.scatter(name['TIME'], name['Z'], label="Z", color="green", marker="*")
    ax.legend()

    # customizations of the axes
    ax.set_xlabel("TIME")
    ax.set_ylabel("X-Y-Z changes")
    ax.set_xlim([0, 1000])
    ax.set_ylim([0, 15000])
    ax.xaxis.set_major_locator(ticker.MultipleLocator(50))
    ax.yaxis.set_major_locator(ticker.MultipleLocator(1000))
    setTitle = 'Changes of 3-axis accelerometer values: ' + title
    saveTitle = title + '.png'
    ax.set_title(setTitle)
    ax.grid(True);
    plot.xticks(rotation=45)
    plot.show();
    figure.savefig(saveTitle)
def plottingHigN(name, title):
    figure, ax = plot.subplots(figsize=(10, 5))

    # customizations of the lines
    ax.scatter(name['TIME'], name['X'], label="X", color="orange", marker="v")
    ax.scatter(name['TIME'], name['Y'], label="Y", color="blue", marker="o")
    ax.scatter(name['TIME'], name['Z'], label="Z", color="green", marker="*")
    ax.legend()

    # customizations of the axes
    ax.set_xlabel("TIME")
    ax.set_ylabel("X-Y-Z changes")
    ax.set_xlim([0, 3000])
    ax.set_ylim([0, 15000])
    ax.xaxis.set_major_locator(ticker.MultipleLocator(100))
    ax.yaxis.set_major_locator(ticker.MultipleLocator(1000))
    setTitle = 'Changes of 3-axis accelerometer values: ' + title
    saveTitle = title + '.png'
    ax.set_title(setTitle)
    ax.grid(True);
    plot.xticks(rotation=45)
    plot.show();
    figure.savefig(saveTitle)


plotting(Stat, "Stationary")
plotting(WalkingClock,"WalkinWithDeviceOnHand")
plottingHigN(FreeFall, "FreeFall")
plotting(Pendolum, "Pendolum")
plotting(Walking, "Walking")
plottingHigN(randRot, "Random Rotation")


