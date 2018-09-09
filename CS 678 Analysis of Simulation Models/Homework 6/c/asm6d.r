a = 2
b = 3
n = 789
u = runif(200)
u = (1 - u)^(1/n)
w = b*(-log(u))^(1/a)
mw = mean(w)
print(w)
print(mw)
hist(w)
abline(v = mw, lwd = 2)