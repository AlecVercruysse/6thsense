# Using the live data analysis software:

### setup your python environment by downloading all required packages:
  1. navigate in terminal to this directory (we'll assume this is the working directory from now on)
  2. run `pip3 install -r requirements.txt` to download all required packages. should give a tkinter error, dw about that
  3. run `sudo apt-get install python-tk` to get tkinter (visualization library)
  3. ensure success by opening a new interactive python window with `python3`, and running `import tkinter` and import `gmplot`
  4. talk to Alec (PL) for the file containing the google maps api key. Please don't upload this to github ever (it should be in the shared .gitignore so no worries...)

### How to collect and monitor data during the launch:
  ~~1. run `ls -la /dev/ > /tmp/devices.txt`
  2. connect the tx basestation to the computer
  3. run `ls -la /dev/ > tmp/devices1.txt`
  4. run `diff /tmp/devices.txt /tmp/devices1.txt` you should somewhere see a file that looks like the serial port to that arduino, keep track of that.
  5. connect the secondary basestation to the computer
  6. run `ls -la /dev/ > /tmp/devices2.txt`
  7. run `diff /tmp/devices2.txt /tmp/devices3.txt` you should somewhere see the serial port of the second basestation.~~
  
  7. connect both arduinos to your computers, open two separate instances of the arduino app, and figure out what serial ports they're on (should be /dev/<something>)
  8. run `tail -f <main serial port> > radio_raw.txt` to write all output text to radio_raw.txt. Note that you need to open an instance of the serial monitor of the arduino ide to get output to start writing. Confirm this is working by running `tail -f radio_raw.txt` to see live output!!
  9. in a new terminal tab, run `tail -f <secondary serial port> > basestation_gps.txt` similarly to step 8
  10. in a new terminal tab, run `python3 radio_parser.py` to start the first visualization. Ensure no errors then move on.
  11. in a new terminal tab, run `python3 map_vis.py` to start the map-centic visualization. We should be good to go.  
