import cv2
from matplotlib import pyplot as plt
import os
import platform

# Leggi immagini
decrypted = cv2.imread("decrypted.jpg")
ecb_leak = cv2.imread("ecb_leak.jpg")

decrypted = cv2.cvtColor(decrypted, cv2.COLOR_BGR2RGB)
ecb_leak = cv2.cvtColor(ecb_leak, cv2.COLOR_BGR2RGB)

# Crea figura affiancata
fig, axes = plt.subplots(1, 2, figsize=(12, 6))
axes[0].imshow(ecb_leak)
axes[0].set_title("ECB Encrypted")
axes[0].axis("off")
axes[1].imshow(decrypted)
axes[1].set_title("Decrypted")
axes[1].axis("off")

plt.tight_layout()
plt.savefig("comparison.png")  # salva la figura

# Apri automaticamente su host, se possibile
system_name = platform.system()
if system_name == "Windows":
    os.system("start comparison.png")
elif system_name == "Linux":
    os.system("xdg-open comparison.png")
elif system_name == "Darwin":
    os.system("open comparison.png")

print("Saved and opened comparison.png")
