import numpy as np
import matplotlib.pyplot as plt

# Load and parse the file
filename = "MeXe_n1.txt"

# Read and flatten the data
with open(filename, 'r') as file:
    raw = file.read().split()

# Convert to float and reshape into pairs: energy (eV), probability density (eV^-1)
data = np.array(raw, dtype=float).reshape(-1, 2)
qe=511
energies = data[:, 0]/1000  # in eV
pdf = data[:, 1] *1000*qe*qe/2/np.pi      # in eV^-1

# Plot the probability distribution function (PDF)
plt.figure(figsize=(8, 5))
plt.plot(energies, pdf, label='PDF', color='blue')
plt.xlabel('Energy (eV)')
plt.ylabel('Probability Density (eV$^{-1}$)')
plt.title('Migdal Electron Energy Distribution')
plt.grid(True)
plt.yscale('log')
plt.xscale('log')
plt.xlim(1e-3,20)
plt.ylim(1e-8,1e2)
plt.legend()
plt.tight_layout()
plt.show()



# Compute the cumulative distribution function (CDF)
cdf = np.cumsum(pdf * np.gradient(energies))  # Trapezoidal integration approx
cdf /= cdf[-1]  # Normalize to 1

# Write output to cdf.txt
with open("cdf.txt", 'w') as out:
    out.write("ME_EnergyCDF = new G4PhysicsOrderedFreeVector();\n")
    for cdf, energy in zip(cdf, energies):
        out.write(f"ME_EnergyCDF->InsertValues({cdf:.8f}, {energy:.6f}*keV);\n")

