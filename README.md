# 6thsense
ASR Balloon Launch. Paper under paper/final_comp.pdf.

## Abstract 

This paper details 6thsense, a mission to send a weather balloon to to edge of space in order to study Earth's atmosphere. The weather balloon is outfitted with a payload that includes a camera, inside and outside temperature sensors, a barometer, multiple GPS tracking devices, as well as a host of individual experiments. The balloon most likely reached an altitude of 20,000 meters, into the stratosphere and ozone layer. The balloon was successfully recovered after approximately one and one half hours of flight.
 
### Team:

| Team Member | Role |
|:------------|:-----|
|Luke Bowsher|Software Engineer|
|John Kim|Quality Control|
|Vivian Liu|Experiment Designer|
|Simon Oros|Electrical Engineer|
|Caroline Pang|Mechanical Engineer|
|Alec Vercruysse|Project Leader|
 
 ### Directories @ root:
 | File/directory | Creator | Desciption |
 |:---------------|:--------|:-----------|
 |GPSLogger|Luke (SE)|arduino mega code for logging GPS to SD thru Serial1|
 |ThermistorReaders|Luke (SE)|arduino mega code for a calibrated thermistor|
 |radio_down|Alec (PL)|arduino mega code for interfacing with the up/downlink trancievers|
 |av_experiment|Alec (PL)|Spectrometer experiment (requires mega i2c)| 
 |basestation|Alec (PL)|Basestation arduino mega code and mac/ubuntu python code for processing transmissions|
 |**final_balloon**|Team|all files for the final balloon launch as it currently stands|
 |**final_data**|Team|Final parsed and unparsed data from Nov. 1 Launch|
 |**paper**|Team|All files relating to the paper, written in rmd compiled to TeX|
 |radio_coms|Alec (PL)| Arduino code to interface with up/downlink 9XTend Modules|
 |secondary arduino|Luke (SE)| Code for second payload arduino uno, for Luke and Simon's individual experiments|
 |test_launch_data|Team|Data from tethered test launch|

