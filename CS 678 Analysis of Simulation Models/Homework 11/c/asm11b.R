
data = scan("tmp.txt")

plot(data, type = 'l')

datats = ts(data)

spec.pgram(datats)