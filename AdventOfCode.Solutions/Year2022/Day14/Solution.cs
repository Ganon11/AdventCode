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

   private static bool DropSand(Dictionary<Position, char> map, int? floorHeight = null)
   {
      var start = new Position(500, 0);
      var sand = start.Copy();
      var maxY = floorHeight ?? map.Keys.Select(p => p.Y).Max();
      while (sand.Y <= maxY)
      {
         var northPosition = sand.North;
         var northWestPosition = sand.NorthWest;
         var northEastPosition = sand.NorthEast;
         Position? nextPosition = null;
         if (!map.ContainsKey(northPosition))
         {
            nextPosition = northPosition;
         }
         else if (!map.ContainsKey(northWestPosition))
         {
            nextPosition = northWestPosition;
         }
         else if (!map.ContainsKey(northEastPosition))
         {
            nextPosition = northEastPosition;
         }
         else
         {
            map[sand] = 'o';
            return map[start] != 'o';
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

   private static void DropFrom(Dictionary<Position, char> map, Position p, int floor)
   {
      if ((map.TryGetValue(p, out var value) && value != '+') || (p.Y == floor))
      {
         return;
      }

      var deltaXValues = new int[] { 0, -1, 1 };
      foreach (var deltaX in deltaXValues)
      {
         DropFrom(map, new Position(p.X + deltaX, p.Y + 1), floor);
      }

      map[p] = 'o';
   }

   public override string SolvePartTwo()
   {
      var map = this.ParsedInput.Copy();
      var floor = map.Keys.Select(p => p.Y).Max() + 2;
      DropFrom(map, new Position(500, 0), floor);

      return map.Count(kvp => kvp.Value == 'o').ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
