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
  assert p1.Distance() == 8
  p2 = Position.Position(3, -5)
  assert p2.Distance() == 8
  p3 = Position.Position(-3, -5)
  assert p3.Distance() == 8
  p4 = Position.Position(-3, 5)
  assert p4.Distance() == 8

def test_manhattan_distance():
  p1 = Position.Position(3, 5)
  p2 = Position.Position(-3, -5)
  assert p1.Distance(p2) == p2.Distance(p1)
  assert p1.Distance(p2) == 16

def test_north():
  o = Position.Position()
  p = o.North()
  assert p.x == 0 and p.y == 1

def test_south():
  o = Position.Position()
  p = o.South()
  assert p.x == 0 and p.y == -1

def test_east():
  o = Position.Position()
  p = o.East()
  assert p.x == 1 and p.y == 0

def test_west():
  o = Position.Position()
  p = o.West()
  assert p.x == -1 and p.y == 0

def test_north_east():
  o = Position.Position()
  ne = o.North().East()
  assert ne.x == 1 and ne.y == 1
  en = o.East().North()
  assert ne == en

def test_north_west():
  o = Position.Position()
  nw = o.North().West()
  assert nw.x == -1 and nw.y == 1
  wn = o.West().North()
  assert nw == wn

def test_south_east():
  o = Position.Position()
  se = o.South().East()
  assert se.x == 1 and se.y == -1
  es = o.East().South()
  assert se == es

def test_south_west():
  o = Position.Position()
  sw = o.South().West()
  assert sw.x == -1 and sw.y == -1
  ws = o.West().South()
  assert sw == ws

def test_adjacency():
  o = Position.Position()
  n = o.North()
  s = o.South()
  e = o.East()
  w = o.West()

  assert o.IsAdjacentTo(n)
  assert o.IsAdjacentTo(s)
  assert o.IsAdjacentTo(e)
  assert o.IsAdjacentTo(w)

  ne = o.North().East()
  nw = o.North().West()
  se = o.South().East()
  sw = o.South().West()

  assert not o.IsAdjacentTo(ne)
  assert not o.IsAdjacentTo(nw)
  assert not o.IsAdjacentTo(se)
  assert not o.IsAdjacentTo(sw)