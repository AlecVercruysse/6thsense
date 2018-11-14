library(mosaic)
pv <- read.csv("data/pv.csv")
aprs <- read.csv("data/aprs.csv")
thermcal <- read.csv("data/temp2.csv")
toSeconds <- function(x){
  if (!is.character(x)) stop("x must be a character string of the form H:M:S")
  if (length(x)<=0)return(x)
  
  unlist(
    lapply(x,
           function(i){
             i <- as.numeric(strsplit(i,':',fixed=TRUE)[[1]])
             if (length(i) == 3) 
               i[1]*3600 + i[2]*60 + i[3]
             else if (length(i) == 2) 
               i[1]*60 + i[2]
             else if (length(i) == 1) 
               i[1]
           }  
    )  
  )  
} 

start_s_9xtend = toSeconds("12:59:57")
secondsSinceStart9xtend <- function(x) {
  return(toSeconds(toString(x)) - start_s_9xtend)
}
pv$sfromstart = sapply(pv$time, secondsSinceStart9xtend)

secondsSinceStartAPRS <- function(x) {
  return(toSeconds(toString(x)) - start_s_9xtend - 25200)
}
aprs$sfromsstart = sapply(aprs$time, secondsSinceStartAPRS)

tempFromAltitude <- function(h){
  if (h < 11000) {
    return(15.04 - 0.00649*h)
  }
  else {
    return(-56.46)
  }
}
  
pv$cTemp = sapply(pv$height, tempFromAltitude)

a <- ggplot(pv, aes(x=height, y=temp)) + geom_point() + geom_point(aes(y=cTemp), color="purple")
a

temp <- thermcal$extech
cal <- lm(thermcal$thermistor ~ temp)
cal
ggplot(thermcal, aes(x=temp, y=thermistor)) + geom_point() + geom_smooth(method="lm")
  
recaled <- predict(cal, data.frame(temp = pv$temp), se.fit=T)
pv$temp_recal = recaled$fit

a <- ggplot(pv, aes(x=height, y=temp_recal)) + geom_point(aes(color="Measured Inside")) + 
  geom_point(aes(y=cTemp, color="Calculated Outside")) + 
  scale_color_manual("Temperature", breaks=c("Measured Inside", "Calculated Outside"),
                        values=c("black", "purple")) + 
  theme(legend.position = c(0.7, 0.9)) + labs(title="Calculated External and Measured Internal Temperature\n vs. Height",
                                        x="Height (m)", y="Temperature (C)")

a
#Vivian's requested graph (temps vs altitude)

#Altitude of Pico and 9xtend vs time (relies on the time code above for setup)
b <- ggplot(pv, aes(x=sfromstart, y=height)) + geom_point(aes(color="From Barometer")) +
  geom_point(data=aprs, aes(x=sfromsstart, y=altitude, color="From PicoAPRS")) + 
  scale_color_manual("Calculated Altitude", breaks=c("From Barometer", "From PicoAPRS"), 
                                            values=c("black", "purple")) +
  theme(legend.position = c(0.9, 0.9)) + labs(title="Altitude from PicoAPRS and Transmitted Barometer Data\n as a function of time",
                                              x="Time elapsed From Startup (s)", y="Calculated Altitude (m)")
b