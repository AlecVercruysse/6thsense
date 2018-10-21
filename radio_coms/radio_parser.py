import tkinter as tk
import re
import gmplot
from sensitive_data import getapikey
import webbrowser
import os

data_stream = open("radio_raw.txt")
data_buffer = ""
latest_vals_indexes = ["time", "GPS", "pressure", "outside temp", "humidity", "inside temp", "spectrometer"]
latest_vals = {}
readrate = 10
for k in latest_vals_indexes:
    latest_vals[k] = "x"



    def togglereadrate():
        global readrate
        readrates=[1,2,5,10,100]
        readrate = readrates[(readrates.index(readrate) + 1) % len(readrates)]

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


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()
        #self.columnconfigure(0, minsize=300)
        self.create_widgets()

    def create_widgets(self):
        self.reading = tk.Label(self, text="Reading from output.txt...", font=("Helvetica", 16), anchor="w")
        self.reading.grid(row=0, column=0, sticky="w")

        self.rawbufferframe = tk.Frame(self, width=150, bg="black")
        self.rawbufferscroll = tk.Scrollbar(self.rawbufferframe)
        self.textfrombuffer = tk.Text(master=self.rawbufferframe, width=140, height=20, yscrollcommand=self.rawbufferscroll.set)
        self.textfrombuffer.grid(row=0, column=0)

        self.rawbufferscroll.config(command=self.textfrombuffer.yview)
        #self.rawbufferscroll.grid(row=0, column=6, sticky="nsw")
        self.rawbufferscroll.grid(row=0, column=1, sticky="nsw")

        self.rawbufferframe.grid(row=1, column=0, columnspan=4, sticky="w")

        self.vislabel = tk.Label(self, text="Data v. Time:", font=("Helvetica", 16))
        self.vislabel.grid(row=0, column=3, sticky="w")

        self.paused = False
        self.pause = tk.Button(self, text="\t||\t", fg="blue", command=self.togglePause)
        self.pause.grid(row=2, column=0, sticky="w")

        self.readratebtn = tk.Button(self, text="\tx"+str(readrate)+" \t", command=togglereadrate)
        self.readratebtn.grid(row=2, column=1, sticky="w")

        self.genmap = tk.Button(self, text="generate position map", fg="green", command=self.generateMap)
        self.genmap.grid(row=2, column=2, sticky="w")

        self.latestvalsdisplay = [tk.Label(self, text=k+": "+str(v), width=35, anchor="w") for k, v in latest_vals.items()]
        [self.latestvalsdisplay[i].grid(row=3+int(i/4), column=i%4, sticky="nw") for i in range(len(self.latestvalsdisplay))]


    def togglePause(self):
        self.paused = not self.paused
        if self.paused:
            self.pause["text"] = "\t>>\t"
        else:
            self.textfrombuffer.see("end")
            self.pause["text"] = "\t||\t"

    def updatedata(self, db):
        self.updatetextfrombuffer(db)
        i = 0
        for k, v in latest_vals.items():
            self.latestvalsdisplay[i]["text"] = k+": "+str(v)
            i=i+1
        self.readratebtn["text"] = "\tx" + str(readrate) + "\t"

    def updatetextfrombuffer(self, db):
        if self.paused:
            return
        self.textfrombuffer.config(state="normal")
        self.textfrombuffer.delete(1.0, "end")
        self.textfrombuffer.insert("end", data_buffer)
        self.textfrombuffer.config(state="disabled")
        if not self.paused:
            self.textfrombuffer.see("end")
        #if not self.paused and self.rawbufferscroll.get()[1] != 1.0 and len(self.rawbufferscroll.get()) == 2:
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
        latdd = self.dms2dd(int(lat[1]), int(lat[2]), float(lat[3])/100, gps[2])
        long = re.search(r"(\d+)(\d\d).(\d+)", gps[3])
        longdd = self.dms2dd(int(long[1]), int(long[2]), float(long[3]) / 100, gps[4])
        print(latdd)
        print(longdd)
        map = gmplot.GoogleMapPlotter(latdd, longdd, 13, apikey=getapikey())
        map.marker(latdd, longdd, 'cornflowerblue')
        map.draw("map.html")
        #print(webbrowser.open_new_tab("map.html"))
        webbrowser.open('file://' + os.path.realpath("map.html"))

def getVals(db):
    latest = re.search(r".*\n(.*)\n[^\n]*$", db, re.S)
    vals = re.split(r'-+', (latest[1] if latest else ""))
    #[vals.append("x") for _ in range(7 - len(vals))]
    if len(vals) != 7:
        return latest_vals
    else:
        r = {}
        for i in range (len(latest_vals_indexes)):
            r[latest_vals_indexes[i]] = vals[i]
        return r
        #return {"time": vals[0], "GPS": vals[1], "pressure": vals[2], "outside_temp": vals[3], "humidity": vals[4],
        #    "inside_temp": vals[5], "spectrometer": vals[6]}


root = tk.Tk()
app = Application(master=root)
while True:
    data_buffer = data_buffer + data_stream.read(readrate)
    data_buffer = re.sub(r"\*+", "\n", data_buffer)
    data_buffer = re.sub(r"\n+", "\n", data_buffer)
    latest_vals = getVals(data_buffer)
    app.updatedata(data_buffer)
    app.update_idletasks()
    app.update()
