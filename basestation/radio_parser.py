"""
General Data visualization for the balloon. Reads from a text file, logs (unvalidaded) gps coordinates to
balloon_gps_out.txt for map_vis.py to better parse. gives basic val vs. time graphs, and basic location
visualization.

This serves as the base parser for the system -- this controls stream parse rate, and allows visualization for
"raw" packets.
"""

__author__ = "Alec Vercruysse"

import tkinter as tk
import re
import gmplot
from sensitive_data import getapikey
import webbrowser
import os

data_stream = open("final_tx.txt")
data_buffer = ""

gps_out = open("balloon_gps_out.txt", "w")

latest_vals_indexes = ["time", "GPS", "altitude", "pressure", "outside temp", "humidity", "inside temp", "spectrometer", "uvb", "methane", "solar", "photoresistor"]
latest_vals = {}
for k in latest_vals_indexes:
    latest_vals[k] = "x"
read_rate = 1

vals_to_graph = ["altitude", "pressure", "outside temp", "inside temp"]

vals_history = {}
for k in vals_to_graph:
    vals_history[k] = [0, 0, 0, 0]
vals_history["time"] = [0]

start_time = 0  # start time in seconds
current_time = 0
time_since_start = 0


def get_vals(db):
    latest = re.search(r".*\n(.*)\n[^\n]*$", db, re.S)
    vals = re.split(r'-+', (latest[1] if latest else ""))
    # [vals.append("x") for _ in range(7 - len(vals))]
    if len(vals) != len(latest_vals_indexes):
        return latest_vals
    else:
        r = {}
        for i in range(len(latest_vals_indexes)):
            r[latest_vals_indexes[i]] = vals[i]
        gps_out.write(latest_vals["GPS"] + "\n")
        return r


def toggle_read_rate():
    global read_rate, graph_resolution
    read_rates = [1, 2, 5, 10, 100]
    read_rate = read_rates[(read_rates.index(read_rate) + 1) % len(read_rates)]


def to_seconds(strin):
    time = re.match(r"(\d+):(\d+):(\d+)", strin)
    if time:
        return 60 * 60 * int(time[1]) + 60 * int(time[2]) + int(time[3])
    else:
        return 0


def update_times():
    global start_time, current_time, time_since_start
    latest_current = to_seconds(latest_vals["time"])
    if latest_current != 0 and latest_current - start_time > 0:
        current_time = latest_current
        time_since_start = current_time - start_time


def update_vals_history():
    if vals_history["time"][-1] < time_since_start:
        vals_make_sense = True
        if time_since_start - vals_history["time"][-1] < 60: # check vals aren't jumping around
            for key, val in vals_history.items():
                new_val = latest_vals[key]
                if key == "time":
                    new_val = time_since_start
                latest_avg = sum(val[-5:])/5
                if latest_avg != 0 and abs((latest_avg - float(new_val))/latest_avg) > 1:
                    vals_make_sense = False
        #vals_make_sense = True # disable value checking
        if vals_make_sense:
            for key, val in vals_history.items():
                if key == "time":
                    val.append(time_since_start)
                else:
                    val.extend([time_since_start, float(latest_vals[key])])


def update_plot(canvases, scale, gtexts):
    # adapted from https://arduino.stackexchange.com/questions/17486/graph-plotting-on-python-using-tkinter-canvas/17529
    for i in range(len(canvases)):
        point_list = vals_history[vals_to_graph[i]][-scale * 2:]
        x_min = min([point_list[xi] for xi in range(len(point_list)) if xi % 2 == 0])
        y_max = max([point_list[yi] for yi in range(len(point_list)) if yi % 2 == 1])
        if y_max == 0:
            y_max = 1
        if x_min == 0:
            x_min = 1
        point_list = [
            (point_list[xi] - x_min) / (2 + time_since_start - x_min) * canvases[i].winfo_width() if xi % 2 == 0
            else (y_max + 1 - .75 * point_list[xi]) / y_max * canvases[i].winfo_height() for xi in range(len(point_list))]
        canvases[i].coords("line", *point_list)
        canvases[i].itemconfigure(gtexts[i], text=str(latest_vals[vals_to_graph[i]])[:5])


def reset_vals():
    global data_buffer, vals_history, start_time, vals_to_graph, time_since_start, current_time
    data_buffer = ""
    vals_history = {}
    for k in vals_to_graph:
        vals_history[k] = [0, 0, 0, 0]
    vals_history["time"] = [0]
    start_time = 0
    while start_time == 0:
        data_buffer = data_buffer + data_stream.read(read_rate)
        data_buffer = re.sub(r"\*+", "\n", data_buffer)
        data_buffer = re.sub(r"\n+", "\n", data_buffer)
        latest_vals = get_vals(data_buffer)
        if "time" in latest_vals:
            start_time = to_seconds(latest_vals["time"])


def format_raw_time(s):
    return str(s // 3600) + "h " + str(s % 3600 // 60) + "m " + str(s % 60) + "s"


def dms2dd(degrees, minutes, seconds, direction):
    dd = float(degrees) + float(minutes) / 60 + float(seconds) / (60 * 60);
    if direction == 'W' or direction == 'S':
        dd *= -1
    return dd


def generate_map():
    global latest_vals
    gps = re.search(r"Lat: (.*)(\w) Long: (.*)(\w)", latest_vals["GPS"])
    lat = re.search(r"(\d+)(\d\d).(\d+)", gps[1])
    latdd = dms2dd(int(lat[1]), int(lat[2]), float(lat[3]) / 100, gps[2])
    long = re.search(r"(\d+)(\d\d).(\d+)", gps[3])
    longdd = dms2dd(int(long[1]), int(long[2]), float(long[3]) / 100, gps[4])
    print(latdd)
    print(longdd)
    map = gmplot.GoogleMapPlotter(latdd, longdd, 13, apikey=getapikey())
    map.marker(latdd, longdd, 'cornflowerblue')
    map.draw("map.html")
    webbrowser.open('file://' + os.path.realpath("map.html"))


def strip_control_characters(input):
    if input:
        # ascii control characters
        input = re.sub(r"(?!\n)[\x01-\x1F\x7F]", "", input)
    return input

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()
        # self.columnconfigure(0, minsize=300)
        self.create_widgets()

    def create_widgets(self):
        self.reading = tk.Label(self, text="Reading from output.txt...", font=("Helvetica", 16), anchor="w")
        self.reading.grid(row=0, column=0, sticky="w")

        self.clear_buffer_button = tk.Button(self, text="!clear buffer!", fg="red", command=reset_vals)
        self.clear_buffer_button.grid(row=0, column=3, sticky="w")

        self.raw_buffer_frame = tk.Frame(self, width=141)
        self.raw_buffer_scroll = tk.Scrollbar(self.raw_buffer_frame)
        self.text_from_buffer = tk.Text(master=self.raw_buffer_frame, width=140, height=20,
                                        yscrollcommand=self.raw_buffer_scroll.set)
        self.text_from_buffer.grid(row=0, column=0, columnspan=4, sticky="new")

        self.raw_buffer_scroll.config(command=self.text_from_buffer.yview)
        self.raw_buffer_scroll.grid(row=0, column=1, sticky="nsw")

        self.raw_buffer_frame.grid(row=1, column=0, columnspan=4, sticky="nw")

        self.paused = False
        self.pause_button = tk.Button(self.raw_buffer_frame, text="\t||\t", fg="blue", command=self.toggle_pause)
        self.pause_button.grid(row=1, column=0, sticky="w")

        self.read_rate_button = tk.Button(self.raw_buffer_frame, text="\tx" + str(read_rate) + " \t", command=toggle_read_rate)
        self.read_rate_button.grid(row=1, column=1, sticky="w")

        self.gen_map_button = tk.Button(self.raw_buffer_frame, text="generate position map", fg="green", command=generate_map)
        self.gen_map_button.grid(row=1, column=2, sticky="w")

        self.latest_vals_display_frame = tk.Frame(self.raw_buffer_frame, width=140)
        self.latest_vals_display_frame.grid(row=3, column=0, columnspan=4, sticky='nw')
        self.latest_vals_display = [tk.Label(self.latest_vals_display_frame, text=k + ": " + str(v), width=35, anchor="w")
                                    for k, v in latest_vals.items()]
        [self.latest_vals_display[i].grid(row=int(i / 3), column=i % 3, sticky="nw") for i in
         range(len(self.latest_vals_display))]

        self.vis_label = tk.Label(self, text="Data v. Time:", font=("Helvetica", 16))
        self.vis_label.grid(row=0, column=4, sticky="w")

        self.graphs_frame = tk.Frame(self, width=100)
        self.graphs_frame.grid(row=1, column=4, sticky="nw")

        self.grah_x_axis_scale_label = tk.Label(self.graphs_frame, text="# points to graph", anchor="w")
        self.grah_x_axis_scale_label.grid(row=1, column=0, sticky="ew")

        self.graph_x_axis_scale_slider = tk.Scale(self.graphs_frame, from_=5, to=3600, orient="horizontal")
        self.graph_x_axis_scale_slider.grid(row=2, column=0, sticky="ew")

        self.graph_x_axis_lowlim_label = tk.Label(self.graphs_frame, text="rewind (s)", anchor="w")
        self.graph_x_axis_lowlim_label.grid(row=3, column=0, sticky="ew")

        self.graph_x_axis_lowlim_slider = tk.Scale(self.graphs_frame, from_=0, to=time_since_start, orient="horizontal")
        self.graph_x_axis_lowlim_slider.grid(row=4, column=0, sticky="ew")

        self.graph_canvases = [tk.Canvas(self.graphs_frame, bg="gray", height=150, width=300) for _ in vals_to_graph]
        self.graph_labels = [tk.Label(self.graphs_frame, text=label) for label in vals_to_graph]
        [self.graph_labels[i].grid(row=i * 2 + 5, column=0, sticky="n") for i in range(len(self.graph_labels))]
        [self.graph_canvases[i].create_line((0, 0, 10, 10), tag="line", fill='darkred', width=1) for i in
         range(len(self.graph_canvases))]
        self.graph_texts = [canvas.create_text(250, 10, text="x") for canvas in self.graph_canvases]
        [self.graph_canvases[i].grid(row=i * 2 + 6, column=0, sticky="nw") for i in
         range(len(self.graph_canvases))]

    def toggle_pause(self):
        self.paused = not self.paused
        if self.paused:
            self.pause_button["text"] = "\t>>\t"
        else:
            self.text_from_buffer.see("end")
            self.pause_button["text"] = "\t||\t"

    def update_data(self, db):
        self.update_text_from_buffer(db)
        self.reading["text"] = "Reading from output.txt...\t\ttime elapsed: " + str(
            time_since_start) + "s\t(" + format_raw_time(time_since_start) + ")"
        i = 0
        for k, v in latest_vals.items():
            self.latest_vals_display[i]["text"] = k + ": " + str(v)
            i = i + 1
        self.read_rate_button["text"] = "\tx" + str(read_rate) + "\t"
        self.graph_x_axis_lowlim_slider.config(to=time_since_start)
        update_plot(self.graph_canvases, self.graph_x_axis_scale_slider.get(), self.graph_texts)

    def update_text_from_buffer(self, db):
        if self.paused:
            return
        self.text_from_buffer.config(state="normal")
        self.text_from_buffer.delete(1.0, "end")
        self.text_from_buffer.insert("end", data_buffer)
        self.text_from_buffer.config(state="disabled")
        if not self.paused:
            self.text_from_buffer.see("end")


root = tk.Tk()
root.title("6thsense balloon launch -- live ")
app = Application(master=root)

while start_time == 0:
    data_buffer = data_buffer + data_stream.read(read_rate)
    data_buffer = strip_control_characters(data_buffer)
    data_buffer = re.sub(r"\*+", "\n", data_buffer)
    data_buffer = re.sub(r"\n+", "\n", data_buffer)
    latest_vals = get_vals(data_buffer)
    print(data_buffer)
    if "time" in latest_vals:
        start_time = to_seconds(latest_vals["time"])
print(start_time)
while True:
    data_buffer = data_buffer + data_stream.read(read_rate)
    data_buffer = strip_control_characters(data_buffer)
    data_buffer = re.sub(r"\*+", "\n", data_buffer)
    data_buffer = re.sub(r"\n+", "\n", data_buffer)
    latest_vals = get_vals(data_buffer)
    update_times()
    update_vals_history()
    app.update_data(data_buffer)
    app.update_idletasks()
    app.update()
