import h5py
import numpy as np
import math
import matplotlib.pyplot as plt
from tqdm import tqdm

POS_RESOLUTION = 1.0
EDGE_CUT = 2


class Event:
    def __init__(self, event_id):
        self.event_id = event_id
        self.energy = 0.0
        self.cluster_list = np.empty((0, 4))  # [x, y, z, energy]

    def add_cluster(self, x, y, z, energy):
        """Merge clusters if close enough, otherwise add a new one."""
        if len(self.cluster_list) > 0:
            distances = np.linalg.norm(self.cluster_list[:, :3] - np.array([x, y, z]), axis=1)
            mask = distances < POS_RESOLUTION
            if np.any(mask):
                idx = np.where(mask)[0][0]  # Get first matching index
                self.cluster_list[idx, 3] += energy
                self.energy += energy
                return

        # Append new cluster
        new_cluster = np.array([[x, y, z, energy]])
        self.cluster_list = np.vstack((self.cluster_list, new_cluster))
        self.energy += energy

    def getEnergy(self):
        return self.energy

    def getClusterNum(self):
        return self.cluster_list.shape[0]

    def hasEdge(self):
        if len(self.cluster_list) == 0:
            return False
        z_vals = np.abs(self.cluster_list[:, 2])
        r_vals = np.sqrt(self.cluster_list[:, 0]**2 + self.cluster_list[:, 1]**2)
        return np.any((z_vals > (15 - EDGE_CUT)) & (r_vals < (11 - EDGE_CUT)))

    def XeEnergy(self):
        return np.any((self.cluster_list[:, 3] > 28) & (self.cluster_list[:, 3] < 35))
    def ArEnergy(self):
        return np.any((self.cluster_list[:, 3] > 1.2) & (self.cluster_list[:, 3] < 5.2))
     
     
     
        

#PROCESS DATA
if not True:
	# Dictionary to store all events
	event_list = {}

	# Read HDF5 file
	with h5py.File("simulation_output.h5", "r") as file:
		dataset = file["Processes"]

		for row in tqdm(dataset, desc="Processing events"):
			event_id = int(row[0])
			x, y, z = row[1], row[2], row[3]
			energy = row[4]

			if event_id not in event_list:
				event_list[event_id] = Event(event_id)

			event_list[event_id].add_cluster(x, y, z, energy)

	####################################################################################           
	print("DATA PROCESSED ----> CREATING HISTOGRAMS...")		

	all_events = list(event_list.values())
	np.save("MigdalEvents.npy", all_events)
else:
	all_events=np.load("MigdalEvents.npy",allow_pickle=True)


spectrum = np.array([event.getEnergy() for event in all_events])
mask1 = np.array([not event.hasEdge() for event in all_events])
mask2 = np.array([event.getClusterNum() >= 2 for event in all_events])
mask3 = np.array([event.XeEnergy() for event in all_events])
#mask3 = np.array([event.ArEnergy() for event in all_events])

#ADD EFFECT OF RESOLUTION:
spectrum_resol=[]
def energy_resolution(e, res_factor=0.3, norm_factor=5.9):  ##res_factor=0.01, norm_factor=2500):
    return res_factor * np.sqrt(e / norm_factor)

spectrum_resol = np.random.normal(spectrum, energy_resolution(spectrum))
spectrum_cut1 = spectrum_resol[mask1]
spectrum_cut2 = spectrum_resol[mask1 & mask2]
spectrum_cut3 = spectrum_resol[mask1 & mask2 & mask3]
print(len(spectrum_resol),len(spectrum_cut1),len(spectrum_cut2),len(spectrum_cut3))



# Filter events with exactly 2 clusters
two_cluster_events = [event for event in all_events if event.getClusterNum() == 2]

# Middle: Energy of the second cluster
second_cluster_energies = [event.cluster_list[1, 3] for event in two_cluster_events]

# Right: Distance between the two clusters
cluster_distances = [
    np.linalg.norm(event.cluster_list[0, :3] - event.cluster_list[1, :3])
    for event in two_cluster_events
]



# Plotting
fig, axs = plt.subplots(1, 3, figsize=(12, 5))
migdal_branch_xe=8.23694e-07 #migdal xenon
migdal_branch_xe=4.6e-6*0.28/2
scale_factor = 1000*5.81*6*migdal_branch_xe*0.89*86400/500000     #Xenon
migdal_branch_ar=8.23694e-07 #migdal argon
#scale_factor = 1000*0.726*0.65*migdal_branch_ar*0.14*86400/100000  #Argon
#scale_factor=1
weights = np.ones_like(spectrum_resol) * scale_factor


counts, bin_edges = np.histogram(spectrum_resol, bins=100, weights=weights)
bin_widths = np.diff(bin_edges)
total_integral = np.sum(counts * bin_widths)
print("Total integral under histogram (Counts/day):", total_integral)

# Left: Total energy spectrum
axs[0].hist(spectrum_resol, bins=120,weights=weights ,histtype='step', color='black', linewidth=1.5)
axs[0].set_title("Total Energy Spectrum")
axs[0].set_xlabel("Energy [keV]")
axs[0].set_ylabel("Counts/day")
#axs[0].set_yscale('log')
#axs[0].set_ylim(0.1,1e12)
#axs[0].set_ylim(0,50)
axs[0].set_xlim(0,120)


# Middle: Second cluster energy
axs[1].hist(second_cluster_energies,bins=50, color='black', alpha=0.7)
axs[1].set_title("Second Cluster Energy (2-cluster events)")
axs[1].set_xlabel("Energy [keV]")
axs[1].set_ylabel("Counts")
axs[1].set_xlim(0,100)

# Right: Distance between two clusters
axs[2].hist(cluster_distances,bins=50, color='black', alpha=0.7)
axs[2].set_title("Distance Between Two Clusters")
axs[2].set_xlabel("Distance [cm]")
axs[2].set_ylabel("Counts")
axs[2].set_xlim(0,20)
axs[2].set_yscale("log")

plt.tight_layout()
plt.show()



