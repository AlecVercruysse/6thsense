"""
Map visualization window for balloon/base station. Should read gps balloon coordinates from a text file, and
base station balloon coordinates from another text file. Low Prioritiy, but extremely helpful if we're chasing
the balloon live. Especially with a directional antenna.

also gets RSSI pwm out in ms (from basestation_gps.txt)

Requirements to get this working:
balloon_gps_out.txt
basestation_gps.txt

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
latest_rssi = 0

latest_base_coords, latest_balloon_coords = "", ""

balloon_gps = open("balloon_gps_out.txt")
base_gps = open("basestation_gps.txt")  # similar formatting as balloon_gps.txt, but no corrupted data, and RSSI


def dms2dd(degrees, minutes, seconds, direction):
    dd = float(degrees) + float(minutes) / 60 + float(seconds) / (60 * 60);
    if direction == 'W' or direction == 'S':
        dd *= -1
    return dd


def update_buffers(a):
    global latest_rssi, latest_balloon_coords, latest_base_coords
    global balloon_gps_raw_buffer, base_gps_buffer
    next_line = balloon_gps.readline()
    # print("nl: " + next_line)
    # print("last: " + str(balloon_gps_raw_buffer.split("\n")))
    if next_line != "" and (len(balloon_gps_raw_buffer) == 0 or next_line != balloon_gps_raw_buffer.split("\n")[-2]):
        balloon_gps_raw_buffer = a.balloon_raw_buffer_text.get("1.0", "end-1c") + next_line
        latest_balloon_coords = next_line
        a.update_bal_buffer()
    next_line = base_gps.readline()
    # print("nl: " + next_line)
    # print("last: " + str(base_gps_buffer.split("\n")))
    if next_line != "":
        base_gps_match = re.match(r"^(.*), (.*)$", next_line)
        if base_gps_match and (len(base_gps_buffer) == 0 or base_gps_match[1] != base_gps_buffer.split("\n")[-2]):
            latest_base_coords = base_gps_match[1] + "\n"
            base_gps_buffer = a.base_raw_buffer_text.get("1.0", "end-1c") + latest_base_coords
            latest_rssi = base_gps_match[2]
            a.update_base_buffer()


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()
        self.create_widgets()

    def create_widgets(self):
        self.reading = tk.Label(self, text="Location Visualizer", font=("Helvetica", 16), anchor="w")
        self.reading.grid(row=0, column=0, sticky="w")

        self.gen_labels = tk.Label(self, text="Balloon GPS data:\t\t\t\t\t\tBase GPS data:", font=("Helvetica", 16),
                                   anchor="w")
        self.gen_labels.grid(row=1, column=0, sticky="w")

        self.rawbuffer_text_frame = tk.Frame(self)
        self.rawbuffer_text_frame.grid(row=2, column=0, sticky="nw")

        self.balloon_raw_buffer_text = tk.Text(self.rawbuffer_text_frame)
        self.base_raw_buffer_text = tk.Text(self.rawbuffer_text_frame)

        self.balloon_raw_buffer_text.grid(row=0, column=0, sticky="nw")
        self.base_raw_buffer_text.grid(row=0, column=1, sticky="nw")

        self.paused = False
        self.pause_button = tk.Button(self.rawbuffer_text_frame, text="\t||\t", fg="blue", command=self.toggle_pause)
        self.pause_button.grid(row=1, column=0, sticky="w")

        self.map_options_frame = tk.Frame(self)
        self.map_options_frame.grid(row=3, sticky="new")

        self.gps_resolution_label = tk.Label(self.map_options_frame, text="gps_resolution")
        self.gps_resolution = tk.Scale(self.map_options_frame, from_=1, to=100, orient="horizontal")
        self.gps_amnt_label = tk.Label(self.map_options_frame, text="gps_amnt")
        self.gps_amnt = tk.Scale(self.map_options_frame, from_=1, to=100, orient="horizontal")
        self.sbalvar = tk.IntVar()
        self.show_balloon = tk.Checkbutton(self.map_options_frame, text="balloon", variable=self.sbalvar)
        self.sbasvar = tk.IntVar()
        self.show_base = tk.Checkbutton(self.map_options_frame, text="base", variable=self.sbasvar)
        self.mapButton = tk.Button(self.map_options_frame, text="generate map", command=self.genmap)

        self.gps_resolution_label.grid(row=0, column=0, sticky="nw")
        self.gps_resolution.grid(row=1, column=0, sticky="nw")
        self.gps_amnt_label.grid(row=0, column=1, sticky="nw")
        self.gps_amnt.grid(row=1, column=1, sticky="nw")
        self.show_balloon.grid(row=0, column=2, sticky="nw")
        self.show_base.grid(row=0, column=3, sticky="nw")
        self.mapButton.grid(row=0, column=4, sticky="nw")

    def genmap(self):
        center_coords = self.format_gps_str(balloon_gps_raw_buffer.split("\n")[-2])
        if not self.sbalvar.get():
            center_coords = self.format_gps_str(base_gps_buffer.split("\n")[-1])
        map = gmplot.GoogleMapPlotter(*center_coords, 13, apikey=getapikey())
        if self.sbalvar.get():
            [map.marker(*self.format_gps_str(balloon_gps_raw_buffer.split("\n")[idx]),
                        "red") for idx in
             range(min(len(balloon_gps_raw_buffer.split("\n"))-1, int(self.gps_amnt.get())))]
        if self.sbasvar.get():
            [map.marker(*self.format_gps_str(base_gps_buffer.split("\n")[idx]), "green") for
             idx in
             range(min(len(base_gps_buffer.split("\n"))-1, int(self.gps_amnt.get())))]
        map.draw("full_map.html")
        webbrowser.open('file://' + os.path.realpath("full_map.html"))

    def format_gps_str(self, gps_str):
        print(gps_str)
        gps = re.search(r"Lat: (.*)(\w) Long: (.*)(\w)", gps_str)
        lat = re.search(r"(\d+)(\d\d).(\d+)", gps[1])
        latdd = dms2dd(int(lat[1]), int(lat[2]), float(lat[3]) / 100, gps[2])
        long = re.search(r"(\d+)(\d\d).(\d+)", gps[3])
        longdd = dms2dd(int(long[1]), int(long[2]), float(long[3]) / 100, gps[4])
        #print("dd lat: " + str(latdd))
        #print("dd long: " + str(longdd))
        return latdd, longdd

    def update_base_buffer(self):
        if self.paused:
            return
        if len(base_gps_buffer.split()[-1]) > 0:
            self.base_raw_buffer_text.insert("end", latest_base_coords)
        self.reading["text"] = "Location Visualizer\t\tRSSI: " + str(latest_rssi)

    def update_bal_buffer(self):
        if self.paused:
            return
        if len(balloon_gps_raw_buffer.split()[-1]) > 0:
            self.balloon_raw_buffer_text.insert("end", latest_balloon_coords)

    def toggle_pause(self):
        self.paused = not self.paused
        if self.paused:
            self.pause_button["text"] = "\t>>\t"
        else:
            self.text_from_buffer.see("end")
            self.pause_button["text"] = "\t||\t"


root = tk.Tk()
root.title("6thsense balloon launch -- mapping")
app = Application(master=root)

while True:
    # for _ in range(10):
    update_buffers(app)
    app.update_idletasks()
    app.update()
# print("balraw: \"" + balloon_gps_raw_buffer + "\"")
# print("baseraw: \"" + base_gps_buffer + "\"")
