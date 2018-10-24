"""
Map visualization window for balloon/base station. Should read gps balloon coordinates from a text file, and
base station balloon coordinates from another text file. Low Prioritiy, but extremely helpful if we're chasing
the balloon live. Especially with a directional antenna.

TODO: read gps balloon coordinates and print them to an editable text on tkinter.
TODO: Same ^^ but with basestation gps
TODO: validate balloon gps coordinates
TODO: add tools for plotting gps with gmap
TODO: add set start point for balloon when gps data first transmitted
TODO: compute distance from start, distance from base station, speed, heading, angle in the sky
"""
import tkinter as tk
import re
from sensitive_data import getapikey
import gmplot
import webbrowser
import os


balloon_gps_raw_buffer = ""
base_gps_buffer = ""

balloon_gps = open("balloon_gps_out.txt")
base_gps = open("basestation_gps.txt") # similar formatting as balloon_gps.txt,but we know theres no corrupted data


def update_buffers():
    global balloon_gps_raw_buffer, base_gps_buffer
    balloon_gps_raw_buffer = balloon_gps_raw_buffer + balloon_gps.readline() + "\n"
    base_gps_buffer = base_gps_buffer + base_gps.readline() + "\n"


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()
        self.create_widgets()

    def create_widgets(self):
        self.reading = tk.Label(self, text="Location Visualizer", font=("Helvetica", 16), anchor="w")
        self.reading.grid(row=0, column=0, sticky="w")


root = tk.Tk()
root.title("6thsense balloon launch -- mapping")
app = Application(master=root)

while True:
    update_buffers()
    app.update_idletasks()
    app.update()