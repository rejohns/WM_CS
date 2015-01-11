data <- scan("data.txt")

print("min:")
print(min(data))
print("max:")
print(max(data))
print("quartiles")
print(quantile(data, c(.25, .5, .75	)))