library(mosaic)
df <- read.csv("heatshrink_measurements.csv")
minfreq = min(df$freq, na.rm=TRUE)
maxfreq = max(df$freq, na.rm=TRUE)

for(trial in df$run[!duplicated(df$run)]) {
  trial_data <- subset(df, df$run == trial)
  #print(trial_data)
  if (max(trial_data$val >= 300, na.rm = TRUE)) {
    a <- ggplot(trial_data, aes(freq, val)) + 
      geom_point() + 
      coord_cartesian(xlim=c(minfreq, maxfreq), ylim=c(0,1115))
    print(a)
  }
}