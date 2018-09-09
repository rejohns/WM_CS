x <- c(2.7, 0.4, 6.8, 0.5, 0.1, 1.8)
x <- sort(x)
D_plus <- c(1:6)
D_minus <- c(1:6)
D_n = 0

for (i in 1:6){
    D_plus[i] = ((i/6) - pexp(x[i], 1/2.05))
    D_minus[i] = (pexp(x[i], 1/2.05) - (i-1)/6)
}

D_n = max(c(max(D_plus), max(D_minus)))

result = (D_n - 0.2/6)*(sqrt(6) + 0.26 + 0.5/sqrt(6))

print("critical value:")
print(1.094)
print("adjusted test statistic:")
print(result)

