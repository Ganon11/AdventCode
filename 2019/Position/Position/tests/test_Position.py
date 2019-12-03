import Position

def test_comparisons_with_self():
  o = Position.Position()
  assert o == o
  assert o <= o
  assert o >= o
  assert not o < o
  assert not o > o
  assert not o != o

def test_comparisons():
  o = Position.Position()
  p = Position.Position(1, 2)
  assert o < p
  assert o <= p
  assert o != p
  assert not o > p
  assert not o >= p
  assert not o == p

def test_default_constructor():
  o = Position.Position()
  assert o.x == 0
  assert o.y == 0

def test_constructor():
  p = Position.Position(3, 5)
  assert p.x == 3
  assert p.y == 5

def test_distance_from_origin():
  p1 = Position.Position(3, 5)
  assert p1.distance() == 8
  p2 = Position.Position(3, -5)
  assert p2.distance() == 8
  p3 = Position.Position(-3, -5)
  assert p3.distance() == 8
  p4 = Position.Position(-3, 5)
  assert p4.distance() == 8

def test_manhattan_distance():
  p1 = Position.Position(3, 5)
  p2 = Position.Position(-3, -5)
  assert p1.distance(p2) == p2.distance(p1)
  assert p1.distance(p2) == 16

def test_north():
  o = Position.Position()
  p = o.north()
  assert p.x == 0 and p.y == 1

def test_south():
  o = Position.Position()
  p = o.south()
  assert p.x == 0 and p.y == -1

def test_east():
  o = Position.Position()
  p = o.east()
  assert p.x == 1 and p.y == 0

def test_west():
  o = Position.Position()
  p = o.west()
  assert p.x == -1 and p.y == 0

def test_north_east():
  o = Position.Position()
  ne = o.north().east()
  assert ne.x == 1 and ne.y == 1
  en = o.east().north()
  assert ne == en

def test_north_west():
  o = Position.Position()
  nw = o.north().west()
  assert nw.x == -1 and nw.y == 1
  wn = o.west().north()
  assert nw == wn

def test_south_east():
  o = Position.Position()
  se = o.south().east()
  assert se.x == 1 and se.y == -1
  es = o.east().south()
  assert se == es

def test_south_west():
  o = Position.Position()
  sw = o.south().west()
  assert sw.x == -1 and sw.y == -1
  ws = o.west().south()
  assert sw == ws

def test_adjacency():
  o = Position.Position()
  n = o.north()
  s = o.south()
  e = o.east()
  w = o.west()

  assert o.is_adjacent_to(n)
  assert o.is_adjacent_to(s)
  assert o.is_adjacent_to(e)
  assert o.is_adjacent_to(w)

  ne = o.north().east()
  nw = o.north().west()
  se = o.south().east()
  sw = o.south().west()

  assert not o.is_adjacent_to(ne)
  assert not o.is_adjacent_to(nw)
  assert not o.is_adjacent_to(se)
  assert not o.is_adjacent_to(sw)