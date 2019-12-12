import position

def test_parse_origin():
  p = position.parse_string('<x=0, y=0, z=0>')
  assert p.x == 0
  assert p.y == 0
  assert p.z == 0

def test_positive_values():
  p = position.parse_string('<x=1, y=2, z=3>')
  assert p.x == 1
  assert p.y == 2
  assert p.z == 3

def test_negative_values():
  p = position.parse_string('<x=-1, y=-2, z=-3>')
  assert p.x == -1
  assert p.y == -2
  assert p.z == -3
