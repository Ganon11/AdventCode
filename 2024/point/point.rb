class Point
  attr_reader :x, :y

  def initialize(x, y)
    @x = x
    @y = y
  end

  def eql?(other)
    self.x == other.x && self.y == other.y
  end

  def hash
    [x, y].hash
  end

  def to_s
    "(#{x}, #{y})"
  end
end
