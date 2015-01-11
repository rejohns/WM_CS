#parameters
N = 16

#raw data, convert to time series
data = c(1.6, 0.8, 1.2, 0.5, 0.9, 1.1, 1.1, 0.6, 1.5, 0.8, 0.9, 1.2, 0.5, 1.3, 0.8, 1.2)
datats = ts(data)

# first 15 observations
xt = data[1:15]

#last 15 observations
xt1 = data[2:16]

#means of raw data, first and last 15 observations
mean  = mean(data)
mean1 = mean(xt)
mean2 = mean(xt1)

#calculate r1 using 2.3
r1_2_3 = (sum((xt - mean1)*(xt1 - mean2))) / 
	(sqrt(sum((xt - mean1)^2)*sum((xt1 - mean2)^2)))
print("d: r1 using 2.3", quote = FALSE)
print(r1_2_3)

#calculate r1 using 2.4
r1_2_4 = (sum((xt - mean)*(xt1 - mean))) /
	( ((N - 1) / N) * sum((data - mean)^2) )
print("e: r1 using 2.4", quote = FALSE)
print(r1_2_4)

#calculate r1 using 2.5
r1_2_5 = (sum((xt - mean)*(xt1 - mean))) /
	sum((data - mean)^2)
print("f: r1 using 2.5", quote = FALSE)
print(r1_2_5)

# calculate r1 using 2.8
c1 = (1 / (N - 1)) * sum( (xt - mean)*(xt1 - mean) )
c0 = (1 / N) * sum( (data - mean)^2 )
r1_2_8 = c1 / c0
print("g: r1 using 2.8", quote = FALSE)
print(r1_2_8)

#acf(x, n, m) - vector x, first n elements, lags 1 through m
func = function(x, n, m) {
	max = n
	mean = mean(x)
	c0 = (1 / N) * sum( (data - mean)^2 )
	for(i in 1:m) {
		new1 = max-i
		new2 = 1+i
		y = x[1:new1]
		z = x[new2:max]
		cn = (1 / (max)) *
			sum( (y - mean) * (z - mean))
		print(cn / c0)
	}
}
print("h: first five lags", quote = FALSE)
func(data, 16, 5)

#test outlier
newData = data
newData[6] = 9.9
print("i: first five lags with outlier", quote = FALSE)
func(newData, 16, 5)

#plot time series, first 15 observations against second 15 observations
#plot(datats)
#plot(xt, xt1)