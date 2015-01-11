x <- c(2.7, 0.4, 6.8, 0.5, 0.1, 1.8)
x <- sort(x)
y <- c()
for (i in 1:6){
    y <- c(y, qexp((i-0.5)/6, 1/2.05))
}
plot(x,y, type = "o")