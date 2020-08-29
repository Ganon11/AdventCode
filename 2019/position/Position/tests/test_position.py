import position

def test_comparisons_with_self():
  o = position.Position()
  assert o == o # pylint: disable=R0124
  assert o <= o # pylint: disable=R0124
  assert o >= o # pylint: disable=R0124
  assert not o < o # pylint: disable=R0124,C0113
  assert not o > o # pylint: disable=R0124,C0113
  assert not o != o # pylint: disable=R0124,C0113

def test_comparisons():
  o = position.Position()
  p = position.Position(1, 2, 3)
  assert o < p
  assert o <= p
  assert o != p
  assert not o > p # pylint: disable=R0124,C0113
  assert not o >= p # pylint: disable=R0124,C0113
  assert not o == p # pylint: disable=R0124,C0113

def test_default_constructor():
  o = position.Position()
  assert o.x == 0
  assert o.y == 0

def test_constructor():
  p = position.Position(3, 5)
  assert p.x == 3
  assert p.y == 5

def test_distance_from_origin():
  p1 = position.Position(3, 5)
  assert p1.distance() == 8
  p2 = position.Position(3, -5)
  assert p2.distance() == 8
  p3 = position.Position(-3, -5)
  assert p3.distance() == 8
  p4 = position.Position(-3, 5)
  assert p4.distance() == 8

def test_manhattan_distance():
  p1 = position.Position(3, 5)
  p2 = position.Position(-3, -5)
  assert p1.distance(p2) == p2.distance(p1)
  assert p1.distance(p2) == 16

def test_north():
  o = position.Position()
  p = o.north()
  assert p.x == 0 and p.y == 1

def test_south():
  o = position.Position()
  p = o.south()
  assert p.x == 0 and p.y == -1

def test_east():
  o = position.Position()
  p = o.east()
  assert p.x == 1 and p.y == 0

def test_west():
  o = position.Position()
  p = o.west()
  assert p.x == -1 and p.y == 0

def test_north_east():
  o = position.Position()
  ne = o.north().east()
  assert ne.x == 1 and ne.y == 1
  en = o.east().north()
  assert ne == en

def test_north_west():
  o = position.Position()
  nw = o.north().west()
  assert nw.x == -1 and nw.y == 1
  wn = o.west().north()
  assert nw == wn

def test_south_east():
  o = position.Position()
  se = o.south().east()
  assert se.x == 1 and se.y == -1
  es = o.east().south()
  assert se == es

def test_south_west():
  o = position.Position()
  sw = o.south().west()
  assert sw.x == -1 and sw.y == -1
  ws = o.west().south()
  assert sw == ws

def test_adjacency():
  o = position.Position()
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

def test_add():
  position1 = position.Position(1, 2, 3)
  position2 = position.Position(3, 4, 5)

  result1 = position1 + position2
  result2 = position2 + position1
  assert result1 == result2
  assert result1.x == 4
  assert result1.y == 6
  assert result1.z == 8
