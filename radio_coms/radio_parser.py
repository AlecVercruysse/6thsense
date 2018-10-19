import tkinter as tk
import re

data_stream = open("radio_raw.txt")
data_buffer = ""


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
        self.title = tk.Label(self, text="Reading from output.txt...", font=("Helvetica", 16))
        self.title.grid(row=0)

        self.rawbufferscroll = tk.Scrollbar(root)

        self.textfrombuffer = tk.Text(self, width=150, yscrollcommand=self.rawbufferscroll.set)
        self.textfrombuffer.grid(row=1, column=0)

        self.rawbufferscroll.config(command=self.textfrombuffer.yview)
        self.rawbufferscroll.grid(row=0, column=2, sticky="nsw")

        self.paused = False
        self.pause = tk.Button(self, text="\t||\t", fg="blue", command=self.togglePause)
        self.pause.grid(row=2, column=0, sticky="w")

    def togglePause(self):
        self.paused = not self.paused
        if self.paused:
            self.pause["text"] = "\t>>\t"
        else:
            self.textfrombuffer.see("end")
            self.pause["text"] = "\t||\t"

    def updatedata(self, db):
        self.updatetextfrombuffer(db)

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


def getVals(db):
    latest = re.search(r".*\n(.*)\n[^\n]*$", db, re.S)
    vals = re.split(r'-+', (latest[1] if latest else ""))
    print(len(vals))
    [vals.append("x") for _ in range(7 - len(vals))]
    return {"time": vals[0], "GPS": vals[1], "pressure": vals[2], "outside_temp": vals[3], "humidity": vals[4],
            "inside_temp": vals[5], "spectrometer": vals[6]}

root = tk.Tk()
app = Application(master=root)
while True:
    data_buffer = data_buffer + data_stream.read(1)
    data_buffer = re.sub(r"\*+", "\n", data_buffer)
    data_buffer = re.sub(r"\n+", "\n", data_buffer)
    print(getVals(data_buffer))
    app.updatedata(data_buffer)
    app.update_idletasks()
    app.update()
