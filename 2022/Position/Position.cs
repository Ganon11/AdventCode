namespace Position
{
  public class Position : IComparable<Position>, IEquatable<Position>
  {
    public int X { get; set; }
    public int Y { get; set; }
    public int Z { get; set; }

    public Position(int x = 0, int y = 0, int z = 0)
    {
      X = x;
      Y = y;
      Z = z;
    }

    public static IEnumerable<Position> GetPointsInLine(Position p1, Position p2)
    {
      bool good = false;
      int numPoints = 0;

      if (p1.Equals(p2))
      {
        good = true;
        numPoints = 1;
      }
      // Horizontal lines
      else if (p1.X != p2.X && p1.Y == p2.Y && p1.Z == p2.Z)
      {
        good = true;
        numPoints = Math.Abs(p1.X - p2.X) + 1;
      }
      // Vertical lines
      else if (p1.X == p2.X && p1.Y != p2.Y && p1.Z == p2.Z)
      {
        good = true;
        numPoints = Math.Abs(p1.Y - p2.Y) + 1;
      }
      // Stacked lines
      else if (p1.X == p2.X && p1.Y == p2.Y && p1.Z != p2.Z)
      {
        good = true;
        numPoints = Math.Abs(p1.Z - p2.Z) + 1;
      }
      // X/Y Diagonals
      else if (p1.Z == p2.Z && Math.Abs(p1.X - p2.X) == Math.Abs(p1.Y - p2.Y))
      {
        good = true;
        numPoints = Math.Abs(p1.Y - p2.Y) + 1;
      }
      // X/Z Diagonals
      else if (p1.Y == p2.Y && Math.Abs(p1.X - p2.X) == Math.Abs(p1.Z - p2.Z))
      {
        good = true;
        numPoints = Math.Abs(p1.Z - p2.Z) + 1;
      }
      // Y/Z Diagonals
      else if (p1.X == p2.X && Math.Abs(p1.Y - p2.Y) == Math.Abs(p1.Z - p2.Z))
      {
        good = true;
        numPoints = Math.Abs(p1.Z - p2.Z) + 1;
      }
      // 3D Diagonals
      else if (Math.Abs(p1.X - p2.X) == Math.Abs(p1.Y - p2.Y) && Math.Abs(p1.X - p2.X) == Math.Abs(p1.Z - p2.Z))
      {
        good = true;
        numPoints = Math.Abs(p1.Z - p2.Z) + 1;
      }

      if (!good)
      {
        return Enumerable.Empty<Position>();
      }

      List<Position> points = new List<Position>();
      int x = p1.X;
      int y = p1.Y;
      int z = p1.Z;

      for (int index = 0; index < numPoints; index++)
      {
        points.Add(new Position(x, y, z));

        if (x < p2.X) ++x;
        else if (x > p2.X) --x;

        if (y < p2.Y) ++y;
        else if (y > p2.Y) --y;

        if (z < p2.Z) ++z;
        else if (z > p2.Z) --z;
      }

      return points;
    }

    public int ManhattanDistance(Position? other = null)
    {
      other ??= new Position();

      return Math.Abs(other.X - this.X)
        + Math.Abs(other.Y - this.Y)
        + Math.Abs(other.Z - this.Z);
    }

    public double Distance(Position? other = null)
    {
      other ??= new Position();

      int deltaX = Math.Abs(other.X - this.X);
      int deltaY = Math.Abs(other.Y - this.Y);
      int deltaZ = Math.Abs(other.Z - this.Z);

      return Math.Sqrt(Math.Pow(deltaX, 2) + Math.Pow(deltaY, 2) + Math.Pow(deltaZ, 2));
    }

    public bool IsAdjacentTo(Position? other = null, bool includeZ = true, bool includeDiagonals = false)
    {
      if (other is null)
      {
        other = new Position();
      }

      return GetAdjacentPositions(includeZ, includeDiagonals).Any(adj => adj == other);
    }

    public Position North
    {
      get
      {
        return new Position(X, Y + 1, Z);
      }
    }

    public Position South
    {
      get
      {
        return new Position(X, Y - 1, Z);
      }
    }

    public Position East
    {
      get
      {
        return new Position(X + 1, Y, Z);
      }
    }

    public Position West
    {
      get
      {
        return new Position(X - 1, Y, Z);
      }
    }

    public Position Up
    {
      get
      {
        return new Position(X, Y, Z + 1);
      }
    }

    public Position Down
    {
      get
      {
        return new Position(X, Y, Z - 1);
      }
    }

    public IEnumerable<Position> GetAdjacentPositions(bool includeZ = true, bool includeDiagonals = false)
    {
      HashSet<Position> result = new HashSet<Position>
      {
          North,
          West,
          East,
          South
      };

      if (includeZ)
      {
        result.Add(Up);
        result.Add(Down);
      }

      if (includeDiagonals)
      {
        result.Add(North.West);
        result.Add(North.East);
        result.Add(South.West);
        result.Add(South.East);
      }

      if (includeZ && includeDiagonals)
      {
        result.Add(Up.North.West);
        result.Add(Up.North.East);
        result.Add(Up.South.West);
        result.Add(Up.South.East);

        result.Add(Down.North.West);
        result.Add(Down.North.East);
        result.Add(Down.South.West);
        result.Add(Down.South.East);
      }

      return result;
    }

    public override int GetHashCode()
    {
      Tuple<int, int, int> us = new(X, Y, Z);
      return us.GetHashCode();
    }

    public override string ToString()
    {
      return $"({X}, {Y}, {Z})";
    }


    public int CompareTo(Position? other)
    {
      other ??= new Position();

      if (Z < other.Z)
      {
        return 1;
      }

      if (Z == other.Z && Y < other.Y)
      {
        return 1;
      }

      if (Z == other.Z && Y == other.Y && X < other.X)
      {
        return 1;
      }

      if (Z == other.Z && Y == other.Y && X == other.X)
      {
        return 0;
      }

      return -1;
    }

    public bool Equals(Position? other)
    {
      other ??= new Position();

      return X == other.X && Y == other.Y && Z == other.Z;
    }

    public override bool Equals(object? obj)
    {
      obj ??= new Position();

      if (ReferenceEquals(this, obj)) return true;

      if (obj.GetType() != GetType()) return false;

      Position other = (Position)obj;

      return Equals(other);
    }
  }
}