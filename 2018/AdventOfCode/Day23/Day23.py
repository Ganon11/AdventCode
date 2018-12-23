import re
from collections import defaultdict

def gan(s):
  return list(map(int, re.findall(r'-?\d+', s)))

def lenr(l):
  return range(len(l))

d = '''pos=<10,12,12>, r=2
pos=<12,14,12>, r=2
pos=<16,12,12>, r=4
pos=<14,14,14>, r=6
pos=<50,50,50>, r=200
pos=<10,10,10>, r=5'''.split('\n')
#nanobots = map(gan, d)
nanobots = [((n[0], n[1], n[2]), n[3]) for n in map(gan, d)]

def dist(p1, p2):
  return abs(p1[0]-p2[0]) + abs(p1[1]-p2[1]) + abs(p1[2]-p2[2])

srad = 0
rad_idx = 0
in_range = defaultdict(int)
for i in lenr(nanobots):
  pos, rng = nanobots[i]
  strength = 0
  if rng > srad:
    srad = rng
    rad_idx = i
    for j in lenr(nanobots):
      npos, _ = nanobots[j]
      if dist(pos, npos) <= rng:
        in_range[i] += 1

print("a %d" % in_range[rad_idx])

from z3 import *
def zabs(x):
  return If(x >= 0,x,-x)
(x, y, z) = (Int('x'), Int('y'), Int('z'))
in_ranges = [
  Int('in_range_' + str(i)) for i in lenr(nanobots)
]
range_count = Int('sum')
o = Optimize()
for i in lenr(nanobots):
  (nx, ny, nz), nrng = nanobots[i]
  o.add(in_ranges[i] == If(zabs(x - nx) + zabs(y - ny) + zabs(z - nz) <= nrng, 1, 0))
o.add(range_count == sum(in_ranges))
dist_from_zero = Int('dist')
o.add(dist_from_zero == zabs(x) + zabs(y) + zabs(z))
h1 = o.maximize(range_count)
h2 = o.minimize(dist_from_zero)
print(o.check())
#print o.lower(h1)
#print o.upper(h1)
print("b %d %d" % (o.lower(h2), o.upper(h2)))
#print o.model()[x]
#print o.model()[y]
#print o.model()[z]