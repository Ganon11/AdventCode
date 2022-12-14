namespace AdventOfCode.Solutions.Year2022.Day14;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<Dictionary<Position, char>>
{
   public Solution() : base(14, 2022, "Regolith Reservoir", false) { }

   public override Dictionary<Position, char> ParseInput(string input)
   {
      var map = new Dictionary<Position, char>();
      var barriers = input.SplitByNewline(shouldTrim: true);
      foreach (var barrier in barriers)
      {
         var positions = barrier.Split(" -> ")
            .Select(p => new Position(p))
            .ToArray();
         for (var index = 0; index < (positions.Length - 1); ++index)
         {
            foreach (var position in Position.GetPointsInLine(positions[index], positions[index + 1]))
            {
               map[position] = '#';
            }
         }
      }

      map[new Position(500, 0)] = '+';
      return map;
   }

   private static void DropFrom(Dictionary<Position, char> map, Position p, int? floor = null, int? maxY = null)
   {
      if (map.TryGetValue(p, out var value) && value != '+')
      {
         return;
      }

      if (floor != null && p.Y == floor)
      {
         return;
      }

      if (floor == null && maxY != null && p.Y >= maxY)
      {
         throw new InfiniteAbyssException();
      }

      var deltaXValues = new int[] { 0, -1, 1 };
      foreach (var deltaX in deltaXValues)
      {
         DropFrom(map, new Position(p.X + deltaX, p.Y + 1), floor, maxY);
      }

      map[p] = 'o';
   }

   public override string SolvePartOne()
   {
      var map = this.ParsedInput.Copy();
      var maxY = map.Keys.Select(p => p.Y).Max();
      try
      {
         DropFrom(map, new Position(500, 0), maxY: maxY);
      }
      catch (InfiniteAbyssException)
      { }

      return map.Count(kvp => kvp.Value == 'o').ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var map = this.ParsedInput.Copy();
      var floor = map.Keys.Select(p => p.Y).Max() + 2;
      DropFrom(map, new Position(500, 0), floor: floor);

      return map.Count(kvp => kvp.Value == 'o').ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal sealed class InfiniteAbyssException : Exception
{ }
