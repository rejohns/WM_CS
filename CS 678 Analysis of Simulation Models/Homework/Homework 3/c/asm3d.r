r1 <- c(0.999900, 0.900721, 0.820818, 0.750933, 0.689028, 0.636327, 0.591295, 0.550071, 0.512054, 0.478687, 0.449613, 0.422463, 0.394719, 0.365821, 0.337226, 0.311733, 0.290882, 0.270343, 0.247945, 0.231832)

r2 <- c(0.999897, 0.877849, 0.781792, 0.707452, 0.644569, 0.588450, 0.537041, 0.492160, 0.448514, 0.411380, 0.377346, 0.344806, 0.315493, 0.291039, 0.267672, 0.248550, 0.230034, 0.210591, 0.192171, 0.178278)

r3 <- c(0.999900, 0.891567, 0.803818, 0.730693, 0.667101, 0.611331, 0.560145, 0.515214, 0.472439, 0.436156, 0.402589, 0.372272, 0.345710, 0.318548, 0.293653, 0.271313, 0.252643, 0.235147, 0.222665, 0.212390)

g_range <- range(0, r1, r2, r3)

plot(r1, type = "o", xlab = "Lag", ylab = "Sample Autocorrelation", ylim = g_range)

box()

lines(r2, type = "o", pch=22)

lines(r3, type = "o",pch=20)

legend(10, g_range[2], c("run 1","run 2", "run 3"), cex=0.8, 
   col=c("black","black","black"), pch=c(21,22,20))