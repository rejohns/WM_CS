x1 <- state.x77[,1]
x2 <- state.x77[,3]
x <- cbind(1,x1,x2)
y <- state.x77[,5]
#num <- (t(x) %*% y)
#denom <- (t(x) %*% x)
#denom <- solve(denom)
beta = solve(t(x)%*%x)%*%(t(x)%*%y)
print(beta)