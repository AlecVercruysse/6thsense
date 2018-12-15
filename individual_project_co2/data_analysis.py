import glob
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

file_list = sorted(glob.glob('/Users/johnkim/Documents/ASR/individual_project/*.csv'))
ppm = []

for file in file_list:
	df = pd.read_csv(file)
	ppm.append(np.mean(df['Latest: CO2 (ppm)']))

names = ["Deep in the mountains", "In the mountains, close to roads", "721 West Remington Drive",
		 "22424 Riverside Drive", "Menlo School", "BJ bus stop", "Cupertino sports center",
		 "Fremont high school", "Homestead high school"]

colors = ['', 'FFA07A', 'FA8072', 'E9967A', 'F08080', 'CD5C5C', 'DC143C', 'B22222', 'FF0000', '8B0000']

order = [2, 3, 5, 1, 8, 4, 7, 6, 9]

gps = [[37.279600, -122.073580],     # pretty far away
		[37.306350, -122.074643],    # not that far away
		[37.359678, -122.042752],    # 721 w remington drive
		[37.310732, -122.068313],    # home
		[37.453212, -122.191202],    # menlo school
		[37.331340, -122.032014],    # bj stop
		[37.323024, -122.043139],    # cupertino sports center
		[37.353207, -122.033027],    # fremont high school
		[37.337291, -122.049752]]    # homestead high school

mapquestKey = "LAK9khflgkYGJ5WB8gRM8tTJtMysqTlo"

longitude = "-122.097232"
latitude = "37.382690"
quake_js = ""

# This is an example of making map markers for every item in a list
for i, location in enumerate(gps):
  qlat = str(location[0])
  qlong = str(location[1])
  quake_js += """L.marker(["""+ qlat + ", " + qlong + """], {
        icon: L.mapquest.icons.marker({primaryColor: '#""" + colors[order[i]] + """', secondaryColor: '#""" + colors[order[i]] + """', size: 'md', symbol: '""" + str(order[i]) + """'}),
        draggable: false
      }).bindPopup('""" + str(names[i]) + """').addTo(map);

      """

# The beginning boiler plate for a mapquest map.
# Note the zoom parameter, which you may wish to change.
geo_start="""<html>
  <head>
    <script src="https://api.mqcdn.com/sdk/mapquest-js/v1.3.2/mapquest.js"></script>
    <link type="text/css" rel="stylesheet" href="https://api.mqcdn.com/sdk/mapquest-js/v1.3.2/mapquest.css"/>

    <script type="text/javascript">
      window.onload = function() {
        L.mapquest.key = '""" + mapquestKey + """';

        var map = L.mapquest.map('map', {
          center: ["""+ latitude + ", " + longitude + """],
          layers: L.mapquest.tileLayer('map'),
          zoom: 11
        });

        L.circle(["""+ latitude + ", " + longitude + """], { radius: 14000 }).addTo(map);
  """

# The ending boilerplate for a mapquest map.
# Note the height parameter, which you may wish to change.
geo_end="""      };
    </script>
  </head>

  <body style="border: 0; margin: 0;">
    <div id="map" style="width: 100%; height: 1000px;"></div>
  </body>
</html>"""

# Write the html for our custom map.
myfile = open("mymap.html","w")
myfile.write(geo_start + quake_js + geo_end)
myfile.close()

raw_data = []
error = []
data = []

import math
for h, file in enumerate(file_list):
	df = pd.read_csv(file)
	raw_data.append(df)

	df.columns = ['time', str(order[h])]
	gas = pd.DataFrame(df[str(order[h])])
	
	a = list((100/gas).values.flatten())
	a_squared = [x**2 for x in a]
	te = pd.DataFrame([math.sqrt(x + (4.0/1024)**2 + (12.0/1024)**2) for x in a_squared])
	loc = pd.DataFrame([order[h]]*len(gas))
	if order[h] <= 4:
		env = pd.DataFrame(['Far from infrastructure']*len(gas))
	else:
		env = pd.DataFrame(['Close to infrastructure']*len(gas))
	te.columns = ['te']
	gas.columns = ['CO2 concentration (ppm)']
	loc.columns = ['loc']
	env.columns = ['Environment']
	combined = pd.concat([gas, te, loc, env], axis=1)
	error.append(te)
	data.append(combined)

df = pd.DataFrame([])
for d in data:
	df = df.append(d)

ax = raw_data[0].plot(x='time', y=str(order[0]))
plt.errorbar(x=raw_data[0]['time'], y=raw_data[0][str(order[0])], yerr=[a*b for a,b in zip(list((error[0]).values.flatten()),list(raw_data[0][str(order[0])]))])
for i, a in enumerate(raw_data):
	if not i == 0:
		a.plot(x='time', y=str(order[i]), ax=ax)
		plt.errorbar(x=a['time'], y=a[str(order[i])], yerr=[a*b for a,b in zip(list((error[i]).values.flatten()),list(a[str(order[i])]))])
plt.ylim(0)
plt.ylabel('CO2 concentration (ppm)')
plt.xlabel('Time (s)')


import seaborn as sns
p1 = sns.boxplot(data=df, x='loc', y='CO2 concentration (ppm)', hue='Environment')
p1.set(xlabel='Location', ylabel='CO2 concentration (ppm)')
plt.show()



import scipy.stats as stats
ffi = df['CO2 concentration (ppm)'][df.Environment == 'Far from infrastructure']
cti = df['CO2 concentration (ppm)'][df.Environment == 'Close to infrastructure']

stats.ttest_ind(ffi, cti, equal_var=False)

sns.distplot(ffi); sns.distplot(cti)
