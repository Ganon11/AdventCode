#pragma warning disable CS8604 // Possible null reference argument.
#pragma warning disable CS8625 // Cannot convert null literal to non-nullable reference type.

namespace AdventOfCode.Solutions.Year2022.Day23;

using System;
using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<Elf[]>
{
   public Solution() : base(23, 2022, "Unstable Diffusion", false) { }

   public override Elf[] ParseInput(string input)
   {
      long row = 0;
      var elves = new List<Elf>();
      foreach (var line in input.SplitByNewline(shouldTrim: true))
      {
         long col = 0;
         foreach (var ch in line)
         {
            if (ch == '#')
            {
               elves.Add(new Elf(new Position(col, row)));
            }

            ++col;
         }

         --row;
      }

      return elves.ToArray();
   }

   private static bool Round(Dictionary<Position, Elf> map, List<Direction> directions)
   {
      var proposingElves = new List<Elf>();
      foreach (var elf in map.Values)
      {
         var adjacentPositions = elf.Current.GetAdjacentPositions(includeZ: false, includeDiagonals: true);
         // If no adjacent elves, skip
         if (!adjacentPositions.Any(map.ContainsKey))
         {
            continue;
         }

         foreach (var direction in directions)
         {
            if (direction == Direction.North)
            {
               var northDirections = adjacentPositions.Where(p => p.Y == elf.Current.Y + 1);
               if (!northDirections.Any(map.ContainsKey))
               {
                  elf.Proposed = elf.Current.North;
                  break;
               }
            }
            else if (direction == Direction.South)
            {
               var southDirections = adjacentPositions.Where(p => p.Y == elf.Current.Y - 1);
               if (!southDirections.Any(map.ContainsKey))
               {
                  elf.Proposed = elf.Current.South;
                  break;
               }
            }
            else if (direction == Direction.West)
            {
               var westDirections = adjacentPositions.Where(p => p.X == elf.Current.X - 1);
               if (!westDirections.Any(map.ContainsKey))
               {
                  elf.Proposed = elf.Current.West;
                  break;
               }
            }
            else if (direction == Direction.East)
            {
               var eastDirections = adjacentPositions.Where(p => p.X == elf.Current.X + 1);
               if (!eastDirections.Any(map.ContainsKey))
               {
                  elf.Proposed = elf.Current.East;
                  break;
               }
            }
         }

         if (elf.Proposed == null)
         {
            continue;
         }

         proposingElves.Add(elf);
      }

      if (!proposingElves.Any())
      {
         return false;
      }

      var conflicts = new HashSet<Position>();
      foreach (var elf in proposingElves)
      {
         if (conflicts.Contains(elf.Proposed))
         {
            elf.Proposed = null;
         }

         var otherElves = proposingElves.Where(e => e.Proposed == elf.Proposed);
         if (otherElves.Count() == 1)
         {
            map.Remove(elf.Current);
            map.Add(elf.Proposed, elf);
            elf.Current = elf.Proposed;
            elf.Proposed = null;
         }
         else
         {
            conflicts.Add(elf.Proposed);
            elf.Proposed = null;
         }
      }

      return true;
   }

   public static long CountEmptySpots(Dictionary<Position, Elf> map)
   {
      var minX = map.Keys.Select(p => p.X).Min();
      var maxX = map.Keys.Select(p => p.X).Max();
      var minY = map.Keys.Select(p => p.Y).Min();
      var maxY = map.Keys.Select(p => p.Y).Max();

      var width = maxX - minX + 1;
      var height = maxY - minY + 1;

      //Console.WriteLine($"Dimensions:");
      //Console.WriteLine($"X from {minX} to {maxX}");
      //Console.WriteLine($"Y from {minY} to {maxY}");

      //Console.WriteLine($"Width: {width}");
      //Console.WriteLine($"Height: {height}");

      //Console.WriteLine($"Elf Count: {map.Keys.Count}");

      return (width * height) - map.Keys.Count;
   }

   public static string PrintMap(Dictionary<Position, Elf> map)
   {
      var sb = new StringBuilder();
      var minX = map.Keys.Select(p => p.X).Min();
      var maxX = map.Keys.Select(p => p.X).Max();
      var minY = map.Keys.Select(p => p.Y).Min();
      var maxY = map.Keys.Select(p => p.Y).Max();

      for (var y = maxY; y >= minY; --y)
      {
         for (var x = minX; x <= maxX; ++x)
         {
            var p = new Position(x, y);
            if (map.ContainsKey(p))
            {
               sb.Append('#');
            }
            else
            {
               sb.Append('.');
            }
         }
         sb.AppendLine();
      }

      return sb.ToString();
   }

   public override string SolvePartOne()
   {
      var directions = new List<Direction>
      {
         Direction.North,
         Direction.South,
         Direction.West,
         Direction.East
      };

      var map = this.ParsedInput.Copy()
         .ToDictionary(e => e.Current);

      //Console.WriteLine(PrintMap(map));

      for (var count = 0; count < 10; ++count)
      {
         _ = Round(map, directions);
         var d = directions.First();
         _ = directions.Remove(d);
         directions.Add(d);
      }

      //Console.WriteLine(PrintMap(map));

      return CountEmptySpots(map).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var directions = new List<Direction>
      {
         Direction.North,
         Direction.South,
         Direction.West,
         Direction.East
      };

      var map = this.ParsedInput.Copy()
         .ToDictionary(e => e.Current);

      //Console.WriteLine(PrintMap(map));

      var count = 1;
      while (Round(map, directions))
      {
         var d = directions.First();
         _ = directions.Remove(d);
         directions.Add(d);
         ++count;
      }

      //Console.WriteLine(PrintMap(map));

      return count.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal sealed class Elf : IEquatable<Elf>
{
   public Position Current { get; set; }
   public Position? Proposed { get; set; }

   public Elf(Position current)
   {
      this.Current = current;
   }

   public bool Equals(Elf? other) => other != null && this.Current == other.Current;

   public override bool Equals(object? obj) => this.Equals(obj as Elf);

   public override int GetHashCode() => this.Current.GetHashCode();
}

internal enum Direction
{
   North,
   South,
   West,
   East
}

#pragma warning restore CS8625 // Cannot convert null literal to non-nullable reference type.
#pragma warning restore CS8604 // Possible null reference argument.
