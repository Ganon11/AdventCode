import position

def test_points_in_line_empty_for_invalid_lines():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(2, 4, 9)
  assert [] == position.Position.get_points_in_line(p1, p2)

  p1 = position.Position(1, 2, 3)
  p2 = position.Position(1, 4, 9)
  assert [] == position.Position.get_points_in_line(p1, p2)

  p1 = position.Position(1, 2, 3)
  p2 = position.Position(2, 2, 9)
  assert [] == position.Position.get_points_in_line(p1, p2)

  p1 = position.Position(1, 2, 3)
  p2 = position.Position(2, 4, 3)
  assert [] == position.Position.get_points_in_line(p1, p2)

def test_points_in_horizontal_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(4, 2, 3)

  subpositions = [p1, position.Position(2, 2, 3), position.Position(3, 2, 3), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_vertical_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(1, 5, 3)

  subpositions = [p1, position.Position(1, 3, 3), position.Position(1, 4, 3), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_z_axis_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(1, 2, 6)

  subpositions = [p1, position.Position(1, 2, 4), position.Position(1, 2, 5), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_xy_diagonal_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(4, 5, 3)

  subpositions = [p1, position.Position(2, 3, 3), position.Position(3, 4, 3), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_xz_diagonal_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(4, 2, 6)

  subpositions = [p1, position.Position(2, 2, 4), position.Position(3, 2, 5), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_yz_diagonal_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(1, 5, 6)

  subpositions = [p1, position.Position(1, 3, 4), position.Position(1, 4, 5), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_xyz_diagonal_line():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(4, 5, 6)

  subpositions = [p1, position.Position(2, 3, 4), position.Position(3, 4, 5), p2]
  line = position.Position.get_points_in_line(p1, p2)
  assert len(line) == 4
  assert subpositions == line

def test_points_in_line_order_doesnt_matter():
  p1 = position.Position(1, 2, 3)
  p2 = position.Position(1, 2, 6)
  line1 = position.Position.get_points_in_line(p1, p2)
  line2 = position.Position.get_points_in_line(p2, p1)

  assert len(line1) == len(line2)
  assert all(p in line2 for p in line1)

def test_points_in_line_one_point():
  p = position.Position(1, 2, 3)
  line = position.Position.get_points_in_line(p, p)
  assert len(line) == 1
  assert p in line

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
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_x_comparisons():
  o = position.Position(0, 0, 0)
  p = position.Position(1, 0, 0)

  assert o < p
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_y_comparisons():
  o = position.Position(0, 0, 0)
  p = position.Position(0, 1, 0)

  assert o < p
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_z_comparisons():
  o = position.Position(0, 0, 0)
  p = position.Position(0, 0, 1)

  assert o < p
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_z_trumps_x_in_comparisons():
  o = position.Position(0, 0, 0)
  p = position.Position(-1, 0, 1)

  assert o < p
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_y_trumps_x_in_comparisons():
  o = position.Position(0, 0, 0)
  p = position.Position(0, -1, 1)

  assert o < p
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_z_trumps_y_in_comparisons():
  o = position.Position(0, 0, 0)
  p = position.Position(-1, 1, 0)

  assert o < p
  assert p > o

  assert o <= p
  assert p >= o

  assert o != p
  assert p != o

  assert not o > p # pylint: disable=R0124,C0113
  assert not p < o # pylint: disable=R0124,C0113

  assert not o >= p # pylint: disable=R0124,C0113
  assert not p <= o # pylint: disable=R0124,C0113

  assert not o == p # pylint: disable=R0124,C0113
  assert not p == o # pylint: disable=R0124,C0113

def test_default_constructor():
  o = position.Position()
  assert o.x == 0
  assert o.y == 0
  assert o.z == 0

def test_one_param_constructor():
  o = position.Position(1)
  assert o.x == 1
  assert o.y == 0
  assert o.z == 0

def test_two_param_constructor():
  o = position.Position(1, 2)
  assert o.x == 1
  assert o.y == 2
  assert o.z == 0

def test_three_param_constructor():
  o = position.Position(1, 2, 3)
  assert o.x == 1
  assert o.y == 2
  assert o.z == 3

def test_distance_from_origin():
  p1 = position.Position(3, 5)
  assert p1.distance() == 8
  p2 = position.Position(3, -5)
  assert p2.distance() == 8
  p3 = position.Position(-3, -5)
  assert p3.distance() == 8
  p4 = position.Position(-3, 5)
  assert p4.distance() == 8

  p5 = position.Position(3, 5, 7)
  assert p5.distance() == 15
  p6 = position.Position(3, 5, -7)
  assert p6.distance() == 15

  p7 = position.Position(3, -5, 7)
  assert p7.distance() == 15
  p8 = position.Position(3, -5, -7)
  assert p8.distance() == 15

  p9 = position.Position(-3, 5, 7)
  assert p9.distance() == 15
  p10 = position.Position(-3, 5, -7)
  assert p10.distance() == 15

  p11 = position.Position(-3, -5, 7)
  assert p11.distance() == 15
  p12 = position.Position(-3, -5, -7)
  assert p12.distance() == 15

def test_manhattan_distance():
  p1 = position.Position(3, 5)
  p2 = position.Position(-3, -5)
  assert p1.distance(p2) == p2.distance(p1)
  assert p1.distance(p2) == 16

  p3 = position.Position(1, 2, 3)
  p4 = position.Position(-1, 2, -3)
  assert p3.distance(p4) == p4.distance(p3)
  assert p3.distance(p4) == 8

def test_north():
  o = position.Position()
  p = o.north()
  assert p.x == 0 and p.y == 1 and p.z == 0

def test_south():
  o = position.Position()
  p = o.south()
  assert p.x == 0 and p.y == -1 and p.z == 0

def test_east():
  o = position.Position()
  p = o.east()
  assert p.x == 1 and p.y == 0 and p.z == 0

def test_west():
  o = position.Position()
  p = o.west()
  assert p.x == -1 and p.y == 0 and p.z == 0

def test_up():
  o = position.Position()
  p = o.up()
  assert p.x == 0 and p.y == 0 and p.z == 1

def test_down():
  o = position.Position()
  p = o.down()
  assert p.x == 0 and p.y == 0 and p.z == -1

def test_north_east():
  o = position.Position()
  ne = o.north().east()
  assert ne.x == 1 and ne.y == 1 and ne.z == 0
  en = o.east().north()
  assert ne == en

def test_north_west():
  o = position.Position()
  nw = o.north().west()
  assert nw.x == -1 and nw.y == 1 and nw.z == 0
  wn = o.west().north()
  assert nw == wn

def test_north_up():
  o = position.Position()
  nu = o.north().up()
  assert nu.x == 0 and nu.y == 1 and nu.z == 1
  un = o.up().north()
  assert nu == un

def test_north_down():
  o = position.Position()
  nd = o.north().down()
  assert nd.x == 0 and nd.y == 1 and nd.z == -1
  dn = o.down().north()
  assert nd == dn

def test_south_east():
  o = position.Position()
  se = o.south().east()
  assert se.x == 1 and se.y == -1 and se.z == 0
  es = o.east().south()
  assert se == es

def test_south_west():
  o = position.Position()
  sw = o.south().west()
  assert sw.x == -1 and sw.y == -1 and sw.z == 0
  ws = o.west().south()
  assert sw == ws

def test_south_up():
  o = position.Position()
  su = o.south().up()
  assert su.x == 0 and su.y == -1 and su.z == 1
  us = o.up().south()
  assert su == us

def test_south_down():
  o = position.Position()
  sd = o.south().down()
  assert sd.x == 0 and sd.y == -1 and sd.z == -1
  ds = o.down().south()
  assert sd == ds

def test_east_up():
  o = position.Position()
  eu = o.east().up()
  assert eu.x == 1 and eu.y == 0 and eu.z == 1
  ue = o.up().east()
  assert eu == ue

def test_east_down():
  o = position.Position()
  ed = o.east().down()
  assert ed.x == 1 and ed.y == 0 and ed.z == -1
  de = o.down().east()
  assert ed == de

def test_west_up():
  o = position.Position()
  wu = o.west().up()
  assert wu.x == -1 and wu.y == 0 and wu.z == 1
  uw = o.up().west()
  assert wu == uw

def test_west_down():
  o = position.Position()
  wd = o.west().down()
  assert wd.x == -1 and wd.y == 0 and wd.z == -1
  dw = o.down().west()
  assert wd == dw

def test_adjacency():
  o = position.Position()
  n = o.north()
  s = o.south()
  e = o.east()
  w = o.west()
  u = o.up()
  d = o.down()

  assert o.is_adjacent_to(n)
  assert o.is_adjacent_to(s)
  assert o.is_adjacent_to(e)
  assert o.is_adjacent_to(w)
  assert o.is_adjacent_to(u)
  assert o.is_adjacent_to(d)

  ne = o.north().east()
  nw = o.north().west()
  nu = o.north().up()
  nd = o.north().down()
  se = o.south().east()
  sw = o.south().west()
  su = o.south().up()
  sd = o.south().down()

  assert not o.is_adjacent_to(ne)
  assert not o.is_adjacent_to(nw)
  assert not o.is_adjacent_to(nu)
  assert not o.is_adjacent_to(nd)
  assert not o.is_adjacent_to(se)
  assert not o.is_adjacent_to(sw)
  assert not o.is_adjacent_to(su)
  assert not o.is_adjacent_to(sd)

def test_add():
  position1 = position.Position(1, 2, 3)
  position2 = position.Position(3, 4, 5)

  result1 = position1 + position2
  result2 = position2 + position1
  assert result1 == result2
  assert result1.x == 4
  assert result1.y == 6
  assert result1.z == 8

def test_real_distance_from_origin():
  position1 = position.Position(1, 0, 0)
  position2 = position.Position(0, 1, 0)
  position3 = position.Position(0, 0, 1)

  distance1 = position1.real_distance()
  assert distance1 == 1

  distance2 = position2.real_distance()
  assert distance2 == 1

  distance3 = position3.real_distance()
  assert distance3 == 1

  position4 = position.Position(3, 4, 0)
  distance4 = position4.real_distance()
  assert distance4 == 5

def test_real_distance_from_two_points():
  position1 = position.Position(1, 2, 3)
  position2 = position.Position(4, -2, 3)
  distance1 = position1.real_distance(position2)
  distance2 = position2.real_distance(position1)

  assert distance1 == distance2
  assert distance1 == 5
