sacf <- function(j, y) {
 cj <- length(y) - j
 y1 <- y[1:cj] - mean(y)
 y2 <- y[-1:-j] - mean(y)
 cc <- sum(y1 * y2)/cj
 ac <- cc / var(y)
 ac
}

y = scan("tmp.txt")
for(j in 1:10) {
	print(sacf(j, y))
}