namespace AdventOfCode.Solutions.Year2022.Day24;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<InputInfo>
{
   public Solution() : base(24, 2022, "Blizzard Basin", false) { }

   public override InputInfo ParseInput(string input)
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

      var maxCol = lines.First().Length - 1;
      var maxRow = lines.Length - 1;

      return new InputInfo(blizzards, maxRow, maxCol);
   }

   private static long ShortestPath(Position start, Position end, long maxRow, long maxCol, Dictionary<long, HashSet<Position>> blizzardPositions, long startTime = 0)
   {
      long cycleLength = blizzardPositions.Keys.Count;

      var startState = new BlizzardState(start, startTime % cycleLength);
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
            return costSoFar[current];
         }

         var newCost = costSoFar[current] + 1;
         foreach (var neighbor in current.Position.GetAdjacentPositions(includeZ: false).Append(current.Position))
         {
            var nextState = new BlizzardState(neighbor, (current.BlizzardCycle + 1) % cycleLength);

            // Blizzard occupies the space
            if (blizzardPositions[nextState.BlizzardCycle].Contains(neighbor))
            {
               continue;
            }

            // Out of bounds Col
            if (neighbor.X <= 0 || neighbor.X >= maxCol)
            {
               continue;
            }

            // Out of bounds Row
            if (neighbor.Y <= 0 && neighbor != end && neighbor != start)
            {
               continue;
            }

            // Out of bounds Row
            if (neighbor.Y >= maxRow && neighbor != end && neighbor != start)
            {
               continue;
            }

            // Valid - check if this state is better
            if (!costSoFar.ContainsKey(nextState) || newCost < costSoFar[nextState])
            {
               frontier.Enqueue(nextState, newCost + neighbor.ManhattanDistance(end));
               cameFrom[nextState] = current;
               costSoFar[nextState] = newCost;
            }
         }
      }

      return long.MaxValue;
   }

   public override string SolvePartOne()
   {
      var cost = ShortestPath(this.ParsedInput.Start, this.ParsedInput.End, this.ParsedInput.MaxRow, this.ParsedInput.MaxCol, this.ParsedInput.BlizzardPositions);
      return cost.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var result1 = ShortestPath(this.ParsedInput.Start, this.ParsedInput.End, this.ParsedInput.MaxRow, this.ParsedInput.MaxCol, this.ParsedInput.BlizzardPositions);
      var result2 = ShortestPath(this.ParsedInput.End, this.ParsedInput.Start, this.ParsedInput.MaxRow, this.ParsedInput.MaxCol, this.ParsedInput.BlizzardPositions, result1);
      var result3 = ShortestPath(this.ParsedInput.Start, this.ParsedInput.End, this.ParsedInput.MaxRow, this.ParsedInput.MaxCol, this.ParsedInput.BlizzardPositions, result1 + result2);
      return (result1 + result2 + result3).ToString(System.Globalization.CultureInfo.CurrentCulture);
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

   public Blizzard(long row, long col, Direction direction)
   {
      this.Position = new Position(col, row);
      this.Direction = direction;
   }

   public void Move(long maxRow, long maxCol)
   {
      Position nextPosition;
      switch (this.Direction)
      {
         case Direction.Up:
            nextPosition = this.Position.North;
            if (nextPosition.Y >= maxRow)
            {
               nextPosition.Y = 1;
            }
            break;
         case Direction.Down:
            nextPosition = this.Position.South;
            if (nextPosition.Y <= 0)
            {
               nextPosition.Y = maxRow - 1;
            }
            break;
         case Direction.Left:
            nextPosition = this.Position.West;
            if (nextPosition.X <= 0)
            {
               nextPosition.X = maxCol - 1;
            }
            break;
         case Direction.Right:
            nextPosition = this.Position.East;
            if (nextPosition.X >= maxCol)
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

   public BlizzardState(Position p, long cycle)
   {
      this.Position = p;
      this.BlizzardCycle = cycle;
   }
}

internal sealed record InputInfo
{
   public Dictionary<long, HashSet<Position>> BlizzardPositions { get; set; }
   public long MaxRow { get; set; }
   public long MaxCol { get; set; }
   public Position Start { get; set; }
   public Position End { get; set; }

   private static void MoveBlizzards(IEnumerable<Blizzard> blizzards, long maxRow, long maxCol)
   {
      foreach (var blizzard in blizzards)
      {
         blizzard.Move(maxRow, maxCol);
      }
   }

   private static Dictionary<long, HashSet<Position>> GetBlizzardPositions(IEnumerable<Blizzard> blizzards, long maxRow, long maxCol)
   {
      var positions = new Dictionary<long, HashSet<Position>>
      {
         { 0, blizzards.Select(b => b.Position).ToHashSet() }
      };

      var rowCount = maxRow - 1;
      var colCount = maxCol - 1;
      var cycleLength = (long)CalculationUtils.FindLCM(rowCount, colCount);
      for (var index = 1; index < cycleLength; ++index)
      {
         MoveBlizzards(blizzards, maxRow, maxCol);
         positions[index] = blizzards.Select(b => b.Position).ToHashSet();
      }

      return positions;
   }

   public InputInfo(IEnumerable<Blizzard> blizzards, long maxRow, long maxCol)
   {
      this.BlizzardPositions = GetBlizzardPositions(blizzards, maxRow, maxCol);
      this.MaxRow = maxRow;
      this.MaxCol = maxCol;
      this.Start = new Position(1, maxRow);
      this.End = new Position(maxCol - 1, 0);
   }
}
