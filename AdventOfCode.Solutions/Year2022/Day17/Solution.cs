namespace AdventOfCode.Solutions.Year2022.Day17;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<char[]>
{
   public Solution() : base(17, 2022, "Pyroclastic Flow", false) { }

   public override char[] ParseInput(string input) => input.Trim().ToCharArray();

   public static string DrawTetris(Shape currentShape, HashSet<Position> settledBlocks)
   {
      var sb = new StringBuilder();
      var shapeMax = currentShape.Positions.Select(p => p.Y).Max();
      var blockMax = settledBlocks.Any() ? settledBlocks.Select(p => p.Y).Max() : long.MinValue;
      var maxY = Math.Max(blockMax, shapeMax);
      for (var row = maxY; row >= 0; --row)
      {
         _ = sb.Append('|');
         for (var col = 0; col < 7; ++col)
         {
            var drawPosition = new Position(col, row);
            if (currentShape.Positions.Contains(drawPosition))
            {
               _ = sb.Append('@');
            }
            else if (settledBlocks.Contains(drawPosition))
            {
               _ = sb.Append('#');
            }
            else
            {
               _ = sb.Append('.');
            }
         }
         _ = sb.AppendLine("|");
      }

      _ = sb.AppendLine("+-------+");
      return sb.ToString();
   }

   public long PlayTetris(long blockCount)
   {
      long spawnHeight = 3;
      var shapeCounter = 0;
      var currentShape = Shape.GetNextShape(spawnHeight, shapeCounter++);
      var settledCount = 0;
      var step = 0;
      var settledBlocks = new HashSet<Position>();

      var cycleDetection = new Dictionary<TetrisState, long>();

      while (settledCount < blockCount)
      {
         _ = currentShape.Move((Direction)this.ParsedInput[step % this.ParsedInput.Length], settledBlocks);
         if (!currentShape.Move(Direction.Down, settledBlocks))
         {
            ++settledCount;
            foreach (var p in currentShape.Positions)
            {
               _ = settledBlocks.Add(p);
            }

            var maxHeight = currentShape.Positions.Select(p => p.Y).Max();
            spawnHeight = maxHeight + 4;
            currentShape = Shape.GetNextShape(spawnHeight, shapeCounter++);
            shapeCounter %= 5;

            var state = new TetrisState
            {
               TopRows = CalculateTopRows(settledBlocks, maxHeight),
               ShapeIndex = shapeCounter,
               JetIndex = step % this.ParsedInput.Length
            };

            if (cycleDetection.TryGetValue(state, out var previousStep))
            {
               Console.WriteLine($"Cycle detected! Previous {previousStep}, current {step}");
            }
            else
            {
               cycleDetection.Add(state, step);
            }
         }

         ++step;
      }

      var height = settledBlocks.Select(p => p.Y).Max() + 1;
      return height;
   }

   private static ulong CalculateTopRows(HashSet<Position> settledBlocks, long maxY)
   {
      var bits = string.Empty;
      for (var deltaY = 0; deltaY < 9; ++deltaY)
      {
         for (var col = 0; col < 7; ++col)
         {
            var p = new Position(col, maxY - deltaY);
            if (settledBlocks.Contains(p))
            {
               bits += '1';
            }
            else
            {
               bits += '0';
            }
         }
      }

      return Convert.ToUInt64(bits, 2);
   }

   public override string SolvePartOne() => this.PlayTetris(2022).ToString(System.Globalization.CultureInfo.CurrentCulture);

   public override string SolvePartTwo() => this.PlayTetris(202200).ToString(System.Globalization.CultureInfo.CurrentCulture);
   //public override string SolvePartTwo() => "";
}

internal enum Direction
{
   Left = '<',
   Right = '>',
   Down = 'v'
}

internal abstract class Shape
{
   public static Shape GetNextShape(long y, int shapeCounter)
   {
      Shape? newShape;
      switch (shapeCounter)
      {
         case 0:
            newShape = new Minus(y);
            break;
         case 1:
            newShape = new Plus(y);
            break;
         case 2:
            newShape = new Ell(y);
            break;
         case 3:
            newShape = new Vert(y);
            break;
         case 4:
            newShape = new Box(y);
            break;
         default:
            newShape = new Minus(y);
            break;
      }

      return newShape;
   }

   public HashSet<Position> Positions { get; protected set; } = new HashSet<Position>();

   public bool Move(Direction d, HashSet<Position> settledBlocks)
   {
      var newPositions = new HashSet<Position>();
      switch (d)
      {
         case Direction.Left:
            var minX = this.Positions.Select(p => p.X).Min();
            if (minX > 0)
            {
               foreach (var p in this.Positions)
               {
                  var newP = new Position(p.X - 1, p.Y);
                  if (settledBlocks.Contains(newP))
                  {
                     return false;
                  }
                  _ = newPositions.Add(newP);
               }
               this.Positions = newPositions;
            }
            return true;
         case Direction.Right:
            var maxX = this.Positions.Select(p => p.X).Max();
            if (maxX < 6)
            {
               foreach (var p in this.Positions)
               {
                  var newP = new Position(p.X + 1, p.Y);
                  if (settledBlocks.Contains(newP))
                  {
                     return false;
                  }
                  _ = newPositions.Add(newP);
               }
               this.Positions = newPositions;
            }
            return true;
         case Direction.Down:
            var minY = this.Positions.Select(p => p.Y).Min();
            if (minY <= 0)
            {
               return false;
            }

            foreach (var p in this.Positions)
            {
               var newP = new Position(p.X, p.Y - 1);
               if (settledBlocks.Contains(newP))
               {
                  return false;
               }
               _ = newPositions.Add(newP);
            }
            this.Positions = newPositions;
            return true;
         default:
            throw new ArgumentException($"Unknown direction {d}!", nameof(d));
      }
   }
}

internal sealed class Minus : Shape
{
   public Minus(long y)
   {
      _ = this.Positions.Add(new Position(2, y));
      _ = this.Positions.Add(new Position(3, y));
      _ = this.Positions.Add(new Position(4, y));
      _ = this.Positions.Add(new Position(5, y));
   }
}

internal sealed class Plus : Shape
{
   public Plus(long y)
   {
      _ = this.Positions.Add(new Position(3, y));
      _ = this.Positions.Add(new Position(2, y + 1));
      _ = this.Positions.Add(new Position(3, y + 1));
      _ = this.Positions.Add(new Position(4, y + 1));
      _ = this.Positions.Add(new Position(3, y + 2));
   }
}

internal sealed class Ell : Shape
{
   public Ell(long y)
   {
      _ = this.Positions.Add(new Position(2, y));
      _ = this.Positions.Add(new Position(3, y));
      _ = this.Positions.Add(new Position(4, y));
      _ = this.Positions.Add(new Position(4, y + 1));
      _ = this.Positions.Add(new Position(4, y + 2));
   }
}

internal sealed class Vert : Shape
{
   public Vert(long y)
   {
      _ = this.Positions.Add(new Position(2, y));
      _ = this.Positions.Add(new Position(2, y + 1));
      _ = this.Positions.Add(new Position(2, y + 2));
      _ = this.Positions.Add(new Position(2, y + 3));
   }
}

internal sealed class Box : Shape
{
   public Box(long y)
   {
      _ = this.Positions.Add(new Position(2, y));
      _ = this.Positions.Add(new Position(3, y));
      _ = this.Positions.Add(new Position(2, y + 1));
      _ = this.Positions.Add(new Position(3, y + 1));
   }
}

internal sealed class TetrisState : IEquatable<TetrisState>
{
   public ulong TopRows { get; init; }
   public int ShapeIndex { get; init; }
   public int JetIndex { get; init; }

   public bool Equals(TetrisState? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.TopRows == other.TopRows && this.ShapeIndex == other.ShapeIndex && this.JetIndex == other.JetIndex;
   }

   public override bool Equals(object? obj) => this.Equals(obj as TetrisState);

   public override int GetHashCode() => new Tuple<ulong, int, int>(this.TopRows, this.ShapeIndex, this.JetIndex).GetHashCode();
}
