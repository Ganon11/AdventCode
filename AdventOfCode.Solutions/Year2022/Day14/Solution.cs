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
         var rocks = new List<Position>();
         for (var index = 0; index < (positions.Length - 1); ++index)
         {
            rocks.AddRange(Position.GetPointsInLine(positions[index], positions[index + 1]));
         }

         foreach (var position in rocks)
         {
            map[position] = '#';
         }
      }

      map[new Position(500, 0)] = '+';
      return map;
   }

   private static bool DropSand(Dictionary<Position, char> map, int? floorHeight = null)
   {
      var start = new Position(500, 0);
      var sand = start.Copy();
      var maxY = floorHeight ?? map.Keys.Select(p => p.Y).Max();
      while (sand.Y <= maxY)
      {
         var nextPosition = sand.North;

         if (map.ContainsKey(nextPosition))
         {
            nextPosition = sand.North.West;
         }
         if (map.ContainsKey(nextPosition))
         {
            nextPosition = sand.North.East;
         }
         if (map.ContainsKey(nextPosition))
         {
            map[sand] = 'o';
            return sand != start;
         }

         if (floorHeight != null && nextPosition.Y == maxY)
         {
            map[sand] = 'o';
            map[nextPosition] = '#';
            return true;
         }

         sand = nextPosition;
      }

      return false;
   }

   public override string SolvePartOne()
   {
      var map = this.ParsedInput.Copy();
      while (DropSand(map))
      { }

      return map.Count(kvp => kvp.Value == 'o').ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var map = this.ParsedInput.Copy();

      var floor = map.Keys.Select(p => p.Y).Max() + 2;
      while (DropSand(map, floorHeight: floor))
      { }

      return map.Count(kvp => kvp.Value == 'o').ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
