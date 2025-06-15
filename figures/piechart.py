import matplotlib.pyplot as plt

# Datos de porcentaje
labels = ['Energía Oscura', 'Materia Oscura', 'Materia Ordinaria']
sizes = [68.5, 26.6, 4.9]
colors = ['#6a0dad', '#1f77b4', '#ff7f0e']  # Colores agradables
explode = (0.1, 0.05, 0)  # Destacar las secciones

# Crear el gráfico de pastel
plt.figure(figsize=(7, 7))
plt.pie(sizes, explode=explode, labels=labels, colors=colors, 
        autopct='%1.1f%%', shadow=True, startangle=140, textprops={'fontsize': 12})
plt.title('Contenido del Universo según el Modelo Cosmológico ΛCDM', fontsize=14)
plt.axis('equal')  # Asegura que el gráfico sea circular
plt.tight_layout()
plt.show()

