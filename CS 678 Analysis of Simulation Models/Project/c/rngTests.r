# rngTests.r	Robert Johns	April 29, 2014
# scans a sequence of random variates valued between
# 0 and 9999 and runs statistical analysis on them 
# to check that they are indeed uniformly distributed
# and that they are sufficiently random.

data = scan("a3725.txt")

data = data / 10000

print(data[1])
print(mean(data))
print(var(data))

sdata  = sort(data)
plot(sdata, pch = '.')
abline(h = mean)
acf(data)
spec.pgram(data)