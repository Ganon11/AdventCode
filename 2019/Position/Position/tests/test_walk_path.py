import position

def test_empty_path():
  path = position.walk_path([])
  assert len(path) == 1
  assert path[0].x == 0 and path[0].y == 0

def test_empty_path_from_point():
  p = position.Position(3, 5)
  path = position.walk_path([], start=p)
  assert len(path) == 1
  assert path[0] == p

def test_simple_walk():
  path = position.walk_path(['L1'])
  assert len(path) == 2
  assert path[0].x == 0 and path[0].y == 0
  assert path[1].x == -1 and path[1].y == 0

  path = position.walk_path(['R1'])
  assert len(path) == 2
  assert path[0].x == 0 and path[0].y == 0
  assert path[1].x == 1 and path[1].y == 0

  path = position.walk_path(['U1'])
  assert len(path) == 2
  assert path[0].x == 0 and path[0].y == 0
  assert path[1].x == 0 and path[1].y == 1

  path = position.walk_path(['D1'])
  assert len(path) == 2
  assert path[0].x == 0 and path[0].y == 0
  assert path[1].x == 0 and path[1].y == -1

def test_complex_walk():
  path = position.walk_path(['L1', 'U1', 'R1', 'D2', 'R3'])
  assert len(path) == 9
  assert path[0].x == 0 and path[0].y == 0
  assert path[1].x == -1 and path[1].y == 0
  assert path[2].x == -1 and path[2].y == 1
  assert path[3].x == 0 and path[3].y == 1
  assert path[4].x == 0 and path[4].y == 0
  assert path[5].x == 0 and path[5].y == -1
  assert path[6].x == 1 and path[6].y == -1
  assert path[7].x == 2 and path[7].y == -1
  assert path[8].x == 3 and path[8].y == -1