x <- c(5, 50, 500, 5000)
y = c(0, 0, 0, 0, 0, 0, 0)
for(i in x){
	print("****************")
	print("now running with")
	print(i)
	print("variates")
	for(j in 1:7){
		print("-----------")
		print("run number:")
		print(j)
		a = rcauchy(i)
		print("mean:")
		mean = mean(a)
		print(mean)
		print("variance:")
		print(var(a))
		y[j] = mean
	}
	print("/////////////")
	print("mean of means")
	print(mean(y))
	print("var of means:")
	print(var(y))
}