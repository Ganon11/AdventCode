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
  assert p1.DistanceToOrigin() == 8
  p2 = Position.Position(3, -5)
  assert p2.DistanceToOrigin() == 8
  p3 = Position.Position(-3, -5)
  assert p3.DistanceToOrigin() == 8
  p4 = Position.Position(-3, 5)
  assert p4.DistanceToOrigin() == 8

def test_manhattan_distance():
  p1 = Position.Position(3, 5)
  p2 = Position.Position(-3, -5)
  assert p1.ManhattanDistance(p2) == p2.ManhattanDistance(p1)
  assert p1.ManhattanDistance(p2) == 16