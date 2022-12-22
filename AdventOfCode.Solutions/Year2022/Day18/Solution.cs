namespace AdventOfCode.Solutions.Year2022.Day18;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<HashSet<Position>>
{
   public Solution() : base(18, 2022, "Boiling Boulders", false) { }

   public override HashSet<Position> ParseInput(string input) => input.SplitByNewline(shouldTrim: true).Select(line => new Position(line)).ToHashSet();

   public override string SolvePartOne()
   {
      var sum = 0;
      foreach (var cube in this.ParsedInput)
      {
         foreach (var adjacentPosition in cube.GetAdjacentPositions(includeZ: true, includeDiagonals: false))
         {
            if (!this.ParsedInput.Contains(adjacentPosition))
            {
               ++sum;
            }
         }
      }

      return sum.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public static HashSet<Position> ReachableFrom(Position start, HashSet<Position> lava)
   {
      var minX = lava.Select(p => p.X).Min() - 1;
      var maxX = lava.Select(p => p.X).Max() + 1;
      var minY = lava.Select(p => p.Y).Min() - 1;
      var maxY = lava.Select(p => p.Y).Max() + 1;
      var minZ = lava.Select(p => p.Z).Min() - 1;
      var maxZ = lava.Select(p => p.Z).Max() + 1;

      var frontier = new Queue<Position>();
      frontier.Enqueue(start);
      var reached = new HashSet<Position>
      {
         start
      };

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();
         foreach (var neighbor in current.GetAdjacentPositions(includeZ: true, includeDiagonals: false))
         {
            // Check in bounds
            if (neighbor.X < minX || neighbor.X > maxX ||
               neighbor.Y < minY || neighbor.Y > maxY ||
               neighbor.Z < minZ || neighbor.Z > maxZ)
            {
               continue;
            }

            // Check if it's lava
            if (lava.Contains(neighbor))
            {
               continue;
            }

            // Check if we've seen it before
            if (reached.Contains(neighbor))
            {
               continue;
            }

            // Add it!
            frontier.Enqueue(neighbor);
            _ = reached.Add(neighbor);
         }
      }

      return reached;
   }

   public override string SolvePartTwo()
   {
      var sum = 0;

      // Find the outside "air pocket"
      var minX = this.ParsedInput.Select(p => p.X).Min() - 1;
      var minY = this.ParsedInput.Select(p => p.Y).Min() - 1;
      var minZ = this.ParsedInput.Select(p => p.Z).Min() - 1;
      var outside = ReachableFrom(new Position(minX, minY, minZ), this.ParsedInput);

      foreach (var cube in outside)
      {
         foreach (var adjacentPosition in cube.GetAdjacentPositions(includeZ: true, includeDiagonals: false))
         {
            if (this.ParsedInput.Contains(adjacentPosition))
            {
               ++sum;
            }
         }
      }

      return sum.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
