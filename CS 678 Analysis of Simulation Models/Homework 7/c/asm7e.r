a = 0
x = c(1:1000)
q = 0
for (i in 1:1000){
    x[i] = 0
    a = rnorm(7)
    for (j in 1:7){
    	x[i] = x[i] + (a[j]*a[j])}
}

x = sort(x)
q = x[680]

print("Estimate:")
print(q)
print("Exact Result:")
print(qchisq(0.68, 7))