namespace AdventOfCode.Solutions.Year2022.Day12;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<Tuple<Position, Position, Dictionary<Position, char>>>
{
   public Solution() : base(12, 2022, "Hill Climbing Algorithm", false) { }

   public override Tuple<Position, Position, Dictionary<Position, char>> ParseInput(string input)
   {
      var map = new Dictionary<Position, char>();
      Position s = new(-1, -1, -1), e = new(-1, -1, -1);
      bool startFound = false, endFound = false;
      var lines = input.SplitByNewline(shouldTrim: true);
      for (var row = 0; row < lines.Length; ++row)
      {
         for (var col = 0; col < lines[row].Length; ++col)
         {
            var p = new Position(col, row);
            if (lines[row][col] == 'S')
            {
               s = p;
               map.Add(p, 'a');
               startFound = true;
            }
            else if (lines[row][col] == 'E')
            {
               e = p;
               map.Add(p, 'z');
               endFound = true;
            }
            else
            {
               map.Add(p, lines[row][col]);
            }
         }
      }

#pragma warning disable CS8604 // Possible null reference argument.
#pragma warning disable CS8625 // Cannot convert null literal to non-nullable reference type.
      if (!startFound || !endFound)
      {
         throw new ArgumentException($"Input is in unexpected format", nameof(input));
      }

      return new(s, e, map);
#pragma warning restore CS8625 // Cannot convert null literal to non-nullable reference type.
#pragma warning restore CS8604 // Possible null reference argument.
   }

   private static int FindShortestPath(Position start, Position end, Dictionary<Position, char> map)
   {
      var frontier = new PriorityQueue<Position, int>();
      frontier.Enqueue(start, 0);

      var cameFrom = new Dictionary<Position, Position?>
      {
         { start, null }
      };
      var costSoFar = new Dictionary<Position, int>
      {
         { start, 0 }
      };

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();
         if (current == end)
         {
            break;
         }

         var currentHeight = map[current];
         foreach (var next in current.GetAdjacentPositions(includeZ: false, includeDiagonals: false))
         {
            if (!map.ContainsKey(next))
            {
               continue;
            }

            var nextHeight = map[next];
            if (nextHeight > currentHeight + 1)
            {
               continue;
            }

            var newCost = costSoFar[current] + 1;
            if (!costSoFar.ContainsKey(next) || newCost < costSoFar[next])
            {
               costSoFar[next] = newCost;
               var priority = newCost + next.ManhattanDistance(end);
               frontier.Enqueue(next, priority);
               cameFrom[next] = current;
            }
         }
      }

      if (!costSoFar.ContainsKey(end))
      {
         return int.MaxValue;
      }

      return costSoFar[end];
   }

   public override string SolvePartOne() => FindShortestPath(this.ParsedInput.Item1, this.ParsedInput.Item2, this.ParsedInput.Item3)
         .ToString(System.Globalization.CultureInfo.CurrentCulture);

   private static int BreadthFirstSearch(Position start, Dictionary<Position, char> map)
   {
      var frontier = new Queue<Tuple<Position, int>>();
      frontier.Enqueue(new (start, 0));
      var reached = new HashSet<Position>();

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();
         var currentHeight = map[current.Item1];

         foreach (var next in current.Item1.GetAdjacentPositions(includeZ: false, includeDiagonals: false))
         {
            if (!map.ContainsKey(next))
            {
               continue;
            }

            if (reached.Contains(next))
            {
               continue;
            }

            var nextHeight = map[next];
            if (nextHeight < (currentHeight - 1))
            {
               continue;
            }

            var newCost = current.Item2 + 1;
            if (nextHeight == 'a')
            {
               return newCost;
            }

            frontier.Enqueue(new Tuple<Position, int>(next, newCost));
            reached.Add(next);
         }
      }

      return int.MaxValue;
   }

   public override string SolvePartTwo() => BreadthFirstSearch(this.ParsedInput.Item2, this.ParsedInput.Item3)
      .ToString(System.Globalization.CultureInfo.CurrentCulture);
}
