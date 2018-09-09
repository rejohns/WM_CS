data = scan("data.txt")

datats = ts(data, frequency = 12)

plot(datats)

aggregate(datats, 5 mean)