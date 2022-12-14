namespace AdventOfCode.Position;

public class Position : IComparable<Position>, IEquatable<Position>
{
   public int X { get; set; }
   public int Y { get; set; }
   public int Z { get; set; }

   public Position(int x = 0, int y = 0, int z = 0)
   {
      this.X = x;
      this.Y = y;
      this.Z = z;
   }

   public static IEnumerable<Position> GetPointsInLine(Position p1, Position p2)
   {
      var good = false;
      var numPoints = 0;

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

         if (x < p2.X)
         {
            ++x;
         }
         else if (x > p2.X)
         {
            --x;
         }

         if (y < p2.Y)
         {
            ++y;
         }
         else if (y > p2.Y)
         {
            --y;
         }

         if (z < p2.Z)
         {
            ++z;
         }
         else if (z > p2.Z)
         {
            --z;
         }
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
      other ??= new Position();

      if (this.Equals(other))
      {
         return false;
      }

      if (!includeZ)
      {
         if (!includeDiagonals)
         {
            var totalDifference = Math.Abs(this.X - other.X) + Math.Abs(this.Y - other.Y);
            return totalDifference <= 1;
         }
         else
         {
            return Math.Abs(this.X - other.X) <= 1 && Math.Abs(this.Y - other.Y) <= 1;
         }
      }
      else
      {
         if (!includeDiagonals)
         {
            var totalDifference = Math.Abs(this.X - other.X) + Math.Abs(this.Y - other.Y) + Math.Abs(this.Z - other.Z);
            return totalDifference <= 1;
         }
         else
         {
            return Math.Abs(this.X - other.X) <= 1 && Math.Abs(this.Y - other.Y) <= 1 && Math.Abs(this.Z - other.Z) <= 1;
         }
      }
   }

   public Position North => new(this.X, this.Y + 1, this.Z);

   public Position South => new(this.X, this.Y - 1, this.Z);

   public Position East => new(this.X + 1, this.Y, this.Z);

   public Position West => new(this.X - 1, this.Y, this.Z);

   public Position Up => new(this.X, this.Y, this.Z + 1);

   public Position Down => new(this.X, this.Y, this.Z - 1);

   public IEnumerable<Position> GetAdjacentPositions(bool includeZ = true, bool includeDiagonals = false)
   {
      var result = new HashSet<Position>
      {
         this.North,
         this.West,
         this.East,
         this.South
      };

      if (includeZ)
      {
         _ = result.Add(this.Up);
         _ = result.Add(this.Down);
      }

      if (includeDiagonals)
      {
         _ = result.Add(this.North.West);
         _ = result.Add(this.North.East);
         _ = result.Add(this.South.West);
         _ = result.Add(this.South.East);
      }

      if (includeZ && includeDiagonals)
      {
         _ = result.Add(this.North.Up);
         _ = result.Add(this.North.Down);

         _ = result.Add(this.South.Up);
         _ = result.Add(this.South.Down);

         _ = result.Add(this.East.Up);
         _ = result.Add(this.East.Down);

         _ = result.Add(this.West.Up);
         _ = result.Add(this.West.Down);

         _ = result.Add(this.Up.North.West);
         _ = result.Add(this.Up.North.East);
         _ = result.Add(this.Up.South.West);
         _ = result.Add(this.Up.South.East);

         _ = result.Add(this.Down.North.West);
         _ = result.Add(this.Down.North.East);
         _ = result.Add(this.Down.South.West);
         _ = result.Add(this.Down.South.East);
      }

      return result;
   }

   public override int GetHashCode()
   {
      Tuple<int, int, int> us = new(this.X, this.Y, this.Z);
      return us.GetHashCode();
   }

   public override string ToString() => $"({this.X}, {this.Y}, {this.Z})";

   public int CompareTo(Position? other)
   {
      other ??= new Position();

      if (this.Z < other.Z)
      {
         return 1;
      }

      if (this.Z == other.Z && this.Y < other.Y)
      {
         return 1;
      }

      if (this.Z == other.Z && this.Y == other.Y && this.X < other.X)
      {
         return 1;
      }

      if (this.Z == other.Z && this.Y == other.Y && this.X == other.X)
      {
         return 0;
      }

      return -1;
   }

   public bool Equals(Position? other)
   {
      other ??= new Position();

      return this.X == other.X && this.Y == other.Y && this.Z == other.Z;
   }

   public override bool Equals(object? obj)
   {
      obj ??= new Position();

      if (ReferenceEquals(this, obj))
      {
         return true;
      }

      if (obj.GetType() != this.GetType())
      {
         return false;
      }

      var other = (Position)obj;

      return this.Equals(other);
   }

   public static bool operator ==(Position left, Position right)
   {
      if (left is null)
      {
         return right is null;
      }

      return left.Equals(right);
   }

   public static bool operator !=(Position left, Position right) => !(left == right);

   public static bool operator <(Position left, Position right) => left is null ? right is not null : left.CompareTo(right) < 0;

   public static bool operator <=(Position left, Position right) => left is null || left.CompareTo(right) <= 0;

   public static bool operator >(Position left, Position right) => left is not null && left.CompareTo(right) > 0;

   public static bool operator >=(Position left, Position right) => left is null ? right is null : left.CompareTo(right) >= 0;

   public static Position operator +(Position left, Position right) => new(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
}
