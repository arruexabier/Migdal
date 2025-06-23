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
        return np.any((self.cluster_list[:, 3] > 29) & (self.cluster_list[:, 3] < 30))
    def ArEnergy(self):
        return np.any((self.cluster_list[:, 3] > 1.2) & (self.cluster_list[:, 3] < 5.2))





all_events=np.load("MigdalEvents.npy",allow_pickle=True)
all_events_bg=np.load("background.npy",allow_pickle=True)



spectrum = np.array([event.getEnergy() for event in all_events_bg])
mask1 = np.array([not event.hasEdge() for event in all_events_bg])
mask2 = np.array([event.getClusterNum() > 1 for event in all_events_bg])
mask3 = np.array([event.XeEnergy() for event in all_events_bg])
#mask3 = np.array([event.ArEnergy() for event in all_events])


#ADD EFFECT OF RESOLUTION:
spectrum_resol=[]
def energy_resolution(e, res_factor=0.01, norm_factor=2500):
    return res_factor * np.sqrt(e / norm_factor)

spectrum_resol = np.random.normal(spectrum, energy_resolution(spectrum))

spectrum_cut1 = spectrum_resol[mask1]
spectrum_cut2 = spectrum_resol[mask1 & mask2]
spectrum_cut3 = spectrum_resol[mask1 & mask2 & mask3]




# Plot histograms
emax = 120
bins = np.linspace(0, emax, 120)
scale_factor_bg = 1000*30*30*86400/10000000
weights_bg = np.ones_like(spectrum_resol) * scale_factor_bg
weights_bg1 = np.ones_like(spectrum_cut1) * scale_factor_bg
weights_bg2 = np.ones_like(spectrum_cut2) * scale_factor_bg
weights_bg3 = np.ones_like(spectrum_cut3) * scale_factor_bg
plt.hist(spectrum_resol, bins,weights=weights_bg ,color="black", label="total")
plt.hist(spectrum_cut1, bins,weights=weights_bg1, color="blue", label="cut1")
plt.hist(spectrum_cut2, bins,weights=weights_bg2, color="red", label="cut2")
plt.hist(spectrum_cut3, bins,weights=weights_bg3,color="gray",label="cut3")



#MIGDAL
spectrum_me = np.array([event.getEnergy() for event in all_events])
spectrum_resol_me = np.random.normal(spectrum_me, energy_resolution(spectrum_me))
scale_factor = 1000*5.81*6*6.6e-7*0.89*86400/1000000
weights = np.ones_like(spectrum_me) * scale_factor
plt.hist(spectrum_resol_me, bins=100,weights=weights ,histtype='step', color='black', linewidth=1.5)


plt.xlabel("Energy Deposition (keV)")
plt.ylabel("Counts/day")
plt.yscale('log')
plt.xlim(0, emax)
plt.ylim(bottom=0.1)
plt.legend(loc='upper right')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.show()










