x <- c(2.7, 0.4, 6.8, 0.5, 0.1, 1.8)
alpha = 0.01
alphaLikelihood <- c()
L <- c()

while (alpha < 4){
      alphaLikelihood <- c(alphaLikelihood, alpha)
      L <- c(L, prod((1/alpha)*exp(-x/alpha)))
      alpha = alpha + 0.01
}

plot(alphaLikelihood, L, type = "l")