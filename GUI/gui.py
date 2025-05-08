import tkinter as tk
from tkinter import ttk
import json
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt

def load_metrics():
    with open("metrics.json") as f:
        return json.load(f)
    
def save_program():
    content = text_area.get("1.0",tk.END)
    with open("program.txt","w") as file:
        file.write(content)

def plot_metric(tab, title, values, ylabel):
    fig, ax = plt.subplots(figsize=(5, 3))
    bars =  ax.bar(range(len(values)), values, tick_label=[f"PE{i}" for i in range(len(values))])
    ax.set_title(title)
    ax.set_ylabel(ylabel)
    canvas = FigureCanvasTkAgg(fig, master=tab)
    canvas.draw()
    canvas.get_tk_widget().pack()
    return fig, ax, bars, canvas

def update_grap():
    with open("metrics.json", "r") as file:
        data = json.load(file)
    # Asegurarse de ordenar por PE
    sorted_data = [data[f"PE{i}"] for i in range(8)]
    for i, d in enumerate(sorted_data):
        bars_bw[i].set_height(d["bandwidth"])
        bars_traffic[i].set_height(d["traffic"])
        bars_lat[i].set_height(d["latency"])

    # Redibujar
    canvas_bw.draw()
    canvas_traffic.draw()
    canvas_lat.draw()





root = tk.Tk()
root.title("Simulador de Procesador G++")
root.geometry("1440x900")

notebook = ttk.Notebook(root)
notebook.pack(fill='both', expand=True)
# Tab del Editor de Texto 
ide_tab = ttk.Frame(notebook)
notebook.add(ide_tab,text="Editor de Texto")
text_area = tk.Text(ide_tab, wrap="none", font=("Courier", 12))
text_area.pack(fill="both", expand=True)

btn_save_program = tk.Button(ide_tab,text="Guardar Programa", command=save_program)
btn_save_program.pack()
# Tab de métricas
metrics_tab = ttk.Frame(notebook)
notebook.add(metrics_tab, text="Métricas")


# Cargar y graficar métricas
data = load_metrics()
bandwidth = [data[f"PE{i}"]["bandwidth"] for i in range(8)]
traffic = [data[f"PE{i}"]["traffic"] for i in range(8)]
latency = [data[f"PE{i}"]["latency"] for i in range(8)]
# Variables globales para acceso en actualizar_graficas()
fig_bw, ax_bw, bars_bw, canvas_bw = plot_metric(metrics_tab, "Ancho de Banda", bandwidth,"Bytes/s")
fig_traffic, ax_traffic, bars_traffic, canvas_traffic = plot_metric(metrics_tab, "Tráfico", traffic, "Bytes")
fig_lat, ax_lat, bars_lat, canvas_lat = plot_metric(metrics_tab, "Latencia", latency, "Ciclos")

btn_update = tk.Button(metrics_tab, text="Actualizar Gráficas", command=update_grap)
btn_update.pack()

root.mainloop()
