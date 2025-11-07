import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("polynomial_evaluation.txt", skiprows=1)
x = data[:, 0]

for i in range(1, data.shape[1]):
    plt.plot(x, data[:, i], label=f"P^{i-1}(x)")

plt.legend()
plt.grid(True)
plt.xlabel("x")
plt.ylabel("Value")
plt.title("Polynomial and Derivatives")
plt.savefig("polynomial_plot.pdf")
plt.savefig("polynomial_plot.png", dpi=150)
print("Plot saved as polynomial_plot.pdf and polynomial_plot.png")

