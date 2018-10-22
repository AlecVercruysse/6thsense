import cProfile
import tkinter as tk
import re
import gmplot
from sensitive_data import getapikey
import webbrowser
import os

import matplotlib

matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
import matplotlib.animation as animation
from matplotlib.figure import Figure
from matplotlib import style

style.use('ggplot')

data_stream = open("radio_raw.txt")
data_buffer = ""

latest_vals_indexes = ["time", "GPS", "pressure", "outside temp", "humidity", "inside temp", "spectrometer"]
latest_vals = {}
for k in latest_vals_indexes:
    latest_vals[k] = "x"
readrate = 10

graph_resolution = 10#int(50 / readrate)  # graphing is very computer intensive, so we don't store/graph all values
graph_i = 0  # loop iteration num. starts over when reached graph_resolution
vals_to_graph = ["pressure", "inside temp"]
vals_history = {}
for k in vals_to_graph:
    vals_history[k] = [0]
vals_history["time"] = [0]

start_time = 0  # start time in seconds
current_time = 0
time_since_start = 0


def getVals(db):
    latest = re.search(r".*\n(.*)\n[^\n]*$", db, re.S)
    vals = re.split(r'-+', (latest[1] if latest else ""))
    # [vals.append("x") for _ in range(7 - len(vals))]
    if len(vals) != 7:
        return latest_vals
    else:
        r = {}
        for i in range(len(latest_vals_indexes)):
            r[latest_vals_indexes[i]] = vals[i]
        return r


def togglereadrate():
    global readrate, graph_resolution
    readrates = [1, 2, 5, 10, 100]
    readrate = readrates[(readrates.index(readrate) + 1) % len(readrates)]

def to_seconds(strin):
    time = re.match(r"(\d+):(\d+):(\d+)", strin)
    if time:
        return 24 * 60 * int(time[1]) + 60 * int(time[2]) + int(time[3])
    else:
        return 0


def update_times():
    global start_time, current_time, time_since_start
    latest_current = to_seconds(latest_vals["time"])
    if latest_current != 0 and latest_current - start_time > 0:
        current_time = latest_current
        time_since_start = current_time - start_time


def update_vals_history():
    global graph_i
    if vals_history["time"][-1] != time_since_start:
        if graph_i==graph_resolution:
            for key, val in vals_history.items():
                if key == "time":
                    val.append(time_since_start)
                else:
                    val.append(latest_vals[key])
            graph_i = 0
        graph_i = graph_i + 1


def plot_all(mplobjs, canvases):
    #TODO: fast option in here using slider https://stackoverflow.com/questions/4098131/how-to-update-a-plot-in-matplotlib
    if graph_i == graph_resolution:
        for i in range(len(mplobjs)):
            mplobjs[i].clear()
            mplobjs[i].plot(vals_history["time"], vals_history[vals_to_graph[i]])
            canvases[i].draw()


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()
        # self.columnconfigure(0, minsize=300)
        self.create_widgets()

    def create_widgets(self):
        self.reading = tk.Label(self, text="Reading from output.txt...", font=("Helvetica", 16), anchor="w")
        self.reading.grid(row=0, column=0, sticky="w")

        self.rawbufferframe = tk.Frame(self, width=150, bg="black")
        self.rawbufferscroll = tk.Scrollbar(self.rawbufferframe)
        self.textfrombuffer = tk.Text(master=self.rawbufferframe, width=140, height=20,
                                      yscrollcommand=self.rawbufferscroll.set)
        self.textfrombuffer.grid(row=0, column=0)

        self.rawbufferscroll.config(command=self.textfrombuffer.yview)
        # self.rawbufferscroll.grid(row=0, column=6, sticky="nsw")
        self.rawbufferscroll.grid(row=0, column=1, sticky="nsw")

        self.rawbufferframe.grid(row=1, column=0, columnspan=4, sticky="w")

        self.paused = False
        self.pause = tk.Button(self, text="\t||\t", fg="blue", command=self.togglePause)
        self.pause.grid(row=2, column=0, sticky="w")

        self.readratebtn = tk.Button(self, text="\tx" + str(readrate) + " \t", command=togglereadrate)
        self.readratebtn.grid(row=2, column=1, sticky="w")

        self.genmap = tk.Button(self, text="generate position map", fg="green", command=self.generateMap)
        self.genmap.grid(row=2, column=2, sticky="w")

        self.latestvalsdisplayframe = tk.Frame(self)
        self.latestvalsdisplayframe.grid(row=3, column=0, columnspan=4, sticky='n')
        self.latestvalsdisplay = [tk.Label(self.latestvalsdisplayframe, text=k + ": " + str(v), width=35, anchor="w")
                                  for k, v in latest_vals.items()]
        [self.latestvalsdisplay[i].grid(row=int(i / 4), column=i % 4, sticky="nw") for i in
         range(len(self.latestvalsdisplay))]

        self.vislabel = tk.Label(self, text="Data v. Time:", font=("Helvetica", 16))
        self.vislabel.grid(row=0, column=4, sticky="w")

        self.graphs_frame = tk.Frame(self, width=100)
        self.graphs_frame.grid(row=1, column=4, sticky="n")

        self.grah_x_axis_scale_label = tk.Label(self.graphs_frame, text="Time to graph (s)", anchor="w")
        self.grah_x_axis_scale_label.grid(row=1, column=0, sticky="w")

        self.graph_x_axis_scale_slider = tk.Scale(self.graphs_frame, from_=60, to=3600, orient="horizontal")
        self.graph_x_axis_scale_slider.grid(row=2, column=0, sticky="w")

        self.graph_x_axis_lowlim_label = tk.Label(self.graphs_frame, text="rewind (s)", anchor="w")
        self.graph_x_axis_lowlim_label.grid(row=3, column=0, sticky="w")

        self.graph_x_axis_lowlim_slider = tk.Scale(self.graphs_frame, from_=0, to=time_since_start, orient="horizontal")
        self.graph_x_axis_lowlim_slider.grid(row=4, column=0, sticky="w")

        self.mplfigures = [Figure(figsize=(2, 2), dpi=100) for _ in vals_to_graph]
        self.mplplots = [f.add_subplot(111) for f in self.mplfigures]
        self.graphCanvases = [FigureCanvasTkAgg(fig, master=self) for fig in self.mplfigures]
        plot_all(self.mplplots, self.graphCanvases)
        [self.graphCanvases[i].get_tk_widget().grid(row=i + 2, column=4, sticky="nw") for i in
         range(len(self.graphCanvases))]

    def togglePause(self):
        self.paused = not self.paused
        if self.paused:
            self.pause["text"] = "\t>>\t"
        else:
            self.textfrombuffer.see("end")
            self.pause["text"] = "\t||\t"

    def updatedata(self, db):
        self.updatetextfrombuffer(db)
        self.reading["text"] = "Reading from output.txt...\t\ttime elapsed: " + str(time_since_start) + "s"
        i = 0
        for k, v in latest_vals.items():
            self.latestvalsdisplay[i]["text"] = k + ": " + str(v)
            i = i + 1
        self.readratebtn["text"] = "\tx" + str(readrate) + "\t"
        self.graph_x_axis_lowlim_slider.config(to=time_since_start)
        plot_all(self.mplplots, self.graphCanvases)
        [canvas.draw() for canvas in self.graphCanvases]

    def updatetextfrombuffer(self, db):
        if self.paused:
            return
        self.textfrombuffer.config(state="normal")
        self.textfrombuffer.delete(1.0, "end")
        self.textfrombuffer.insert("end", data_buffer)
        self.textfrombuffer.config(state="disabled")
        if not self.paused:
            self.textfrombuffer.see("end")
        # if not self.paused and self.rawbufferscroll.get()[1] != 1.0 and len(self.rawbufferscroll.get()) == 2:
        #    self.togglePause()

    def dms2dd(self, degrees, minutes, seconds, direction):
        dd = float(degrees) + float(minutes) / 60 + float(seconds) / (60 * 60);
        if direction == 'W' or direction == 'S':
            dd *= -1
        return dd;

    def generateMap(self):
        global latest_vals
        gps = re.search(r"Lat: (.*)(\w) Long: (.*)(\w)", latest_vals["GPS"])
        lat = re.search(r"(\d+)(\d\d).(\d+)", gps[1])
        latdd = self.dms2dd(int(lat[1]), int(lat[2]), float(lat[3]) / 100, gps[2])
        long = re.search(r"(\d+)(\d\d).(\d+)", gps[3])
        longdd = self.dms2dd(int(long[1]), int(long[2]), float(long[3]) / 100, gps[4])
        print(latdd)
        print(longdd)
        map = gmplot.GoogleMapPlotter(latdd, longdd, 13, apikey=getapikey())
        map.marker(latdd, longdd, 'cornflowerblue')
        map.draw("map.html")
        # print(webbrowser.open_new_tab("map.html"))
        webbrowser.open('file://' + os.path.realpath("map.html"))


root = tk.Tk()
root.title("6thsense balloon launch -- live ")
app = Application(master=root)

while start_time == 0:
    data_buffer = data_buffer + data_stream.read(readrate)
    data_buffer = re.sub(r"\*+", "\n", data_buffer)
    data_buffer = re.sub(r"\n+", "\n", data_buffer)
    latest_vals = getVals(data_buffer)
    if "time" in latest_vals:
        start_time = to_seconds(latest_vals["time"])
print(start_time)

def loop():
    global data_buffer, data_stream, latest_vals, app
    data_buffer = data_buffer + data_stream.read(readrate)
    data_buffer = re.sub(r"\*+", "\n", data_buffer)
    data_buffer = re.sub(r"\n+", "\n", data_buffer)
    latest_vals = getVals(data_buffer)
    update_times()
    update_vals_history()
    app.updatedata(data_buffer)
    app.update_idletasks()
    app.update()

def loop1k():
    for _ in range(10000):
        loop()
while True:
    cProfile.run('loop1k()')

# example code:
'''
class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.hi_there = tk.Button(self)
        self.hi_there["text"] = "Hello World\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack(side="top")

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=root.destroy)
        self.quit.pack(side="bottom")

    def say_hi(self):
        print("hi there, everyone!")

root = tk.Tk()
app = Application(master=root)
app.mainloop()
'''
