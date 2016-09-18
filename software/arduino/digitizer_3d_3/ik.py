from math import sqrt, pow, acos, pi, atan, degrees


l = 176.8 # length of arm - from cad
h = 87.0 # height of 2nd pivot - measured
r = 180.0 # distance from centre of base to first pivot centre - from cad

# given a, b, c, return C
def cosine_law(a,b,c):
   return acos((pow(a,2) + pow(b,2) - pow(c,2)) / (2*a*b))

# hyp
H = sqrt(pow(h,2) + pow(r,2))
#print("H=%f" % H)

# theta2
C = cosine_law(l, l, H)
#print("C=%f" % degrees(C))
theta2 = pi + C
print("theta2=%f" % degrees(theta2))

# theta1
C = cosine_law(l, H, l)
#print("C=%f" % degrees(C))
gamma = atan(h/r)
#print("gamma=%f" % degrees(gamma))
theta1 = C - gamma
print("theta1=%f" % degrees(theta1))

print("theta2=%f" % degrees(pi + 1.21))
print("theta1=%f" % degrees(0.516))
