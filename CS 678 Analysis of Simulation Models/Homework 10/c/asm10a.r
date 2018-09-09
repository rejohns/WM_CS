realma <- function(iq, parms, xmu, sig, n) {

       x = c(1:n)
       z = rnorm(iq, xmu, sig)
       T = c(1:n)
       t = 0

       while (t < n) {
       	     t = t + 1
	     z = c(rnorm(1,xmu,sig), z)
	     for (i in 1:iq){
	     	 x[t] = parms[i]*z[i]}
	}

	plot(T,x, type = "l")
	return(x)
}


	     