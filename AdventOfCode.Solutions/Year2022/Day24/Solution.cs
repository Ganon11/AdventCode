namespace AdventOfCode.Solutions.Year2022.Day24;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<Blizzard[]>
{
   public Solution() : base(24, 2022, "Blizzard Basin", false) { }

   public override Blizzard[] ParseInput(string input)
   {
      var blizzards = new List<Blizzard>();

      var lines = input.SplitByNewline(shouldTrim: true);
      var row = lines.Length - 1;
      foreach (var line in lines)
      {
         for (var col = 0; col < line.Length; ++col)
         {
            switch (line[col])
            {
               case '^':
                  blizzards.Add(new Blizzard(row, col, Direction.Up));
                  break;
               case 'v':
                  blizzards.Add(new Blizzard(row, col, Direction.Down));
                  break;
               case '<':
                  blizzards.Add(new Blizzard(row, col, Direction.Left));
                  break;
               case '>':
                  blizzards.Add(new Blizzard(row, col, Direction.Right));
                  break;
               default:
                  break;
            }
         }

         --row;
      }

      Blizzard.MaxCol = lines.First().Length - 1;
      Blizzard.MaxRow = lines.Length - 1;

      return blizzards.ToArray();
   }

   private static void MoveBlizzards(Blizzard[] blizzards)
   {
      foreach (var blizzard in blizzards)
      {
         blizzard.Move();
      }
   }

   private static (long, Dictionary<long, Blizzard[]>) GetBlizzardPositions(Blizzard[] blizzards)
   {
      var positions = new Dictionary<long, Blizzard[]>
      {
         [0] = blizzards.Copy()
      };

      var rowCount = Blizzard.MaxRow - 1;
      var colCount = Blizzard.MaxCol - 1;
      var cycleLength = (long)CalculationUtils.FindLCM(rowCount, colCount);
      for (var index = 1; index < cycleLength; ++index)
      {
         MoveBlizzards(blizzards);
         positions[index] = blizzards.Copy();
      }

      return (cycleLength, positions);
   }

   private static (long, long) BFS(Position start, Position end, long cycleLength, Dictionary<long, Blizzard[]> blizzardPositions, long startTime = 0)
   {
      Console.WriteLine(PrintBlizzards(start, end, blizzardPositions[startTime % cycleLength]));

      var startState = new BlizzardState { Position = start, BlizzardCycle = startTime % cycleLength };
      var frontier = new PriorityQueue<BlizzardState, long>();
      frontier.Enqueue(startState, 0);

      var costSoFar = new Dictionary<BlizzardState, long>
      {
         { startState, 0 }
      };

      var cameFrom = new Dictionary<BlizzardState, BlizzardState?>
      {
         { startState, null }
      };

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();

         if (current.Position == end)
         {
            return (costSoFar[current], current.BlizzardCycle);
         }

         var newCost = costSoFar[current] + 1;
         foreach (var neighbor in current.Position.GetAdjacentPositions(includeZ: false).Append(current.Position))
         {
            var nextState = new BlizzardState
            {
               Position = neighbor,
               BlizzardCycle = newCost % cycleLength
            };

            // Blizzard occupies the space
            if (blizzardPositions[nextState.BlizzardCycle].Any(b => b.Position == neighbor))
            {
               continue;
            }

            // Out of bounds Col
            if (neighbor.X <= 0 || neighbor.X >= Blizzard.MaxCol)
            {
               continue;
            }

            if (neighbor.Y <= 0 && neighbor != end && neighbor != start)
            {
               continue;
            }

            if (neighbor.Y >= Blizzard.MaxRow && neighbor != end && neighbor != start)
            {
               continue;
            }

            if (!costSoFar.ContainsKey(nextState) || newCost < costSoFar[nextState])
            {
               frontier.Enqueue(nextState, newCost + neighbor.ManhattanDistance(end));
               cameFrom[nextState] = current;
               costSoFar[nextState] = newCost;
            }
         }
      }

      return (int.MaxValue, 0);
   }

   private static string PrintBlizzards(Position start, Position end, Blizzard[] blizzards)
   {
      var sb = new StringBuilder();
      for (var row = Blizzard.MaxRow; row >= 0; --row)
      {
         for (var col = 0; col <= Blizzard.MaxCol; ++col)
         {
            var p = new Position(col, row);
            if (p == start)
            {
               _ = sb.Append('S');
               continue;
            }

            if (p == end)
            {
               _ = sb.Append('E');
               continue;
            }

            if (p.X == 0 || p.Y == 0 || p.X == Blizzard.MaxCol || p.Y == Blizzard.MaxRow)
            {
               _ = sb.Append('#');
               continue;
            }

            var blizzardsAtPosition = blizzards.Where(b => b.Position == p);
            if (blizzardsAtPosition.Count() >= 2)
            {
               _ = sb.Append(blizzardsAtPosition.Count());
            }
            else if (blizzardsAtPosition.Any())
            {
               _ = blizzardsAtPosition.First().Direction switch
               {
                  Direction.Up => sb.Append('^'),
                  Direction.Down => sb.Append('v'),
                  Direction.Left => sb.Append('<'),
                  Direction.Right => sb.Append('>'),
                  _ => sb.Append('?'),
               };
            }
            else
            {
               _ = sb.Append('.');
            }
         }
         _ = sb.AppendLine();
      }

      return sb.ToString();
   }

   public override string SolvePartOne()
   {
      var blizzardInfo = GetBlizzardPositions(this.ParsedInput.Copy());
      var start = new Position(1, Blizzard.MaxRow);
      var end = new Position(Blizzard.MaxCol - 1, 0);

      var cost = BFS(start, end, blizzardInfo.Item1, blizzardInfo.Item2);
      return cost.Item1.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var blizzardInfo = GetBlizzardPositions(this.ParsedInput.Copy());
      var start = new Position(1, Blizzard.MaxRow);
      var end = new Position(Blizzard.MaxCol - 1, 0);

      var result1 = BFS(start, end, blizzardInfo.Item1, blizzardInfo.Item2);
      var result2 = BFS(end, start, blizzardInfo.Item1, blizzardInfo.Item2, result1.Item2);
      var result3 = BFS(start, end, blizzardInfo.Item1, blizzardInfo.Item2, result2.Item2);
      return (result1.Item1 + result2.Item1 + result3.Item1).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal enum Direction
{
   Up,
   Down,
   Left,
   Right
}

internal sealed class Blizzard
{
   public Position Position { get; private set; }
   public Direction Direction { get; init; }

   public static long MaxRow { get; set; }
   public static long MaxCol { get; set; }

   public Blizzard(long row, long col, Direction direction)
   {
      this.Position = new Position(col, row);
      this.Direction = direction;
   }

   public void Move()
   {
      Position nextPosition;
      switch (this.Direction)
      {
         case Direction.Up:
            nextPosition = this.Position.North;
            if (nextPosition.Y >= MaxRow)
            {
               nextPosition.Y = 1;
            }
            break;
         case Direction.Down:
            nextPosition = this.Position.South;
            if (nextPosition.Y <= 0)
            {
               nextPosition.Y = MaxRow - 1;
            }
            break;
         case Direction.Left:
            nextPosition = this.Position.West;
            if (nextPosition.X <= 0)
            {
               nextPosition.X = MaxCol - 1;
            }
            break;
         case Direction.Right:
            nextPosition = this.Position.East;
            if (nextPosition.X >= MaxCol)
            {
               nextPosition.X = 1;
            }
            break;
         default:
            nextPosition = this.Position;
            break;
      }

      this.Position = nextPosition;
   }
}

internal sealed record BlizzardState
{
   public Position Position { get; init; }
   public long BlizzardCycle { get; init; }
}
