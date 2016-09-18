from math import sqrt, pow, acos, pi, atan, degrees


l = 176.7 # length of arm
h = 87 # height of first pivot
r = 250 / 2 + 154 # distance of tip to base of first pivot

# given a, b, c, return C
def cosine_law(a,b,c):
   return acos((pow(a,2) + pow(b,2) - pow(c,2)) / (2*a*b))

# hyp
H = sqrt(pow(h,2) + pow(r,2))
print("H=%f" % H)

# theta2
C = cosine_law(l, l, H)
print("C=%f" % degrees(C))
theta2 = pi + (pi - C)
print("theta2=%f" % degrees(theta2))

# theta1
C = cosine_law(l, H, l)
gamma = atan(h/r)
theta1 = C - gamma
print("theta1=%f" % degrees(theta1))

print("theta2=%f" % degrees(pi + 1.21))
print("theta1=%f" % degrees(0.516))
