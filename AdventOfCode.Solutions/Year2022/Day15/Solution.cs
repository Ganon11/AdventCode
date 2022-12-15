namespace AdventOfCode.Solutions.Year2022.Day15;

using System.Text.RegularExpressions;
using AdventOfCode.Position;

internal sealed partial class Solution : SolutionBase<Sensor[]>
{
   public Solution() : base(15, 2022, "Beacon Exclusion Zone", false) { }

   public override Sensor[] ParseInput(string input)
   {
      var sensors = new List<Sensor>();
      foreach (var line in input.SplitByNewline(shouldTrim: true))
      {
         var match = SensorBeaconRegex().Match(line);
         if (!match.Success)
         {
            throw new ArgumentException($"Input in unexpected format", nameof(input));
         }

         var sensorX = int.Parse(match.Groups["sensorX"].Value, System.Globalization.CultureInfo.CurrentCulture);
         var sensorY = int.Parse(match.Groups["sensorY"].Value, System.Globalization.CultureInfo.CurrentCulture);
         var beaconX = int.Parse(match.Groups["beaconX"].Value, System.Globalization.CultureInfo.CurrentCulture);
         var beaconY = int.Parse(match.Groups["beaconY"].Value, System.Globalization.CultureInfo.CurrentCulture);

         var s = new Position(sensorX, sensorY);
         var b = new Position(beaconX, beaconY);
         sensors.Add(new Sensor(s, b));
      }

      return sensors.ToArray();
   }

   public static string DrawMap(Dictionary<Position, char> map, int? targetRow = null, int? bound = null)
   {
      int minX, maxX, minY, maxY;
      if (bound.HasValue)
      {
         minX = 0;
         maxX = bound.Value;
         minY = 0;
         maxY = bound.Value;
      }
      else
      {
         minX = map.Keys.Select(p => p.X).Min();
         maxX = map.Keys.Select(p => p.X).Max();
         minY = map.Keys.Select(p => p.Y).Min();
         maxY = map.Keys.Select(p => p.Y).Max();
      }

      var sb = new StringBuilder();
      for (var row = minY; row <= maxY; row++)
      {
         for (var col = minX; col <= maxX; col++)
         {
            if (map.TryGetValue(new Position(col, row), out var value))
            {
               _ = sb.Append(value);
            }
            else
            {
               _ = sb.Append(' ');
            }
         }
         if (targetRow != null && row == targetRow)
         {
            _ = sb.Append("<-- RIGHT HERE");
         }
         _ = sb.AppendLine();
      }

      return sb.ToString();
   }

   public override string SolvePartOne()
   {
      var targetRow = this.Debug ? 10 : 2000000;
      var map = new Dictionary<Position, char>();
      foreach (var sensor in this.ParsedInput)
      {
         map[sensor.Position] = 'S';
         map[sensor.ClosestBeacon] = 'B';
         var manhattanDistance = sensor.Position.ManhattanDistance(sensor.ClosestBeacon);

         // Does the generated diamond intersect the target row?
         var distanceToTargetRow = sensor.Position.ManhattanDistance(new Position(sensor.Position.X, targetRow));
         if (distanceToTargetRow <= manhattanDistance)
         {
            // Points could contribute to solution
            var horizontalRange = manhattanDistance - distanceToTargetRow;
            var left = new Position(sensor.Position.X - horizontalRange, targetRow);
            var right = new Position(sensor.Position.X + horizontalRange, targetRow);
            foreach (var p in Position.GetPointsInLine(left, right))
            {
               if (!map.ContainsKey(p))
               {
                  map[p] = '#';
               }
            }
         }
      }

      //Console.WriteLine(DrawMap(map, targetRow: targetRow));
      var targetPositions = map.Keys.Where(p => p.Y == targetRow);
      var minCol = targetPositions.Select(p => p.X).Min();
      var maxCol = targetPositions.Select(p => p.X).Max();
      var sum = 0;
      for (var col = minCol; col <= maxCol; ++col)
      {
         if (map.TryGetValue(new Position(col, targetRow), out var value) && value == '#')
         {
            ++sum;
         }
      }

      return sum.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var bound = this.Debug ? 20 : 4000000;
      Position? emptyTarget = null;
      foreach (var source in this.ParsedInput)
      {
         foreach (var boundingPosition in source.BoundingLines)
         {
            if (boundingPosition.X < 0 || boundingPosition.X > bound || boundingPosition.Y < 0 || boundingPosition.Y > bound)
            {
               continue;
            }

            if (this.ParsedInput.Any(sensor => boundingPosition == sensor.ClosestBeacon))
            {
               continue;
            }

            var inRange = false;
            foreach (var otherSensor in this.ParsedInput)
            {
               if (source.Position == otherSensor.Position)
               {
                  continue;
               }

               if (otherSensor.Position.ManhattanDistance(boundingPosition) <= otherSensor.Distance)
               {
                  inRange = true;
                  break;
               }
            }

            if (!inRange)
            {
               emptyTarget = boundingPosition;
               break;
            }
         }

         if (emptyTarget != null)
         {
            break;
         }
      }

      if (emptyTarget != null)
      {
         //Console.WriteLine(emptyTarget);
         var tuningFrequency = ((long)emptyTarget.X * 4000000) + (long)emptyTarget.Y;
         return tuningFrequency.ToString(System.Globalization.CultureInfo.CurrentCulture);
      }

      return "";
   }

   [GeneratedRegex("Sensor at x=(?<sensorX>.+), y=(?<sensorY>.+): closest beacon is at x=(?<beaconX>.+), y=(?<beaconY>.+)")]
   private static partial Regex SensorBeaconRegex();
}

internal sealed record Sensor
{
   public Position Position { get; init; }
   public Position ClosestBeacon { get; init; }
   public int Distance { get; init; }
   public HashSet<Position> BoundingLines { get; init; }

   public Sensor(Position p, Position b)
   {
      this.Position = p;
      this.ClosestBeacon = b;

      this.BoundingLines = new HashSet<Position>();
      this.Distance = this.Position.ManhattanDistance(this.ClosestBeacon);

      var top = new Position(this.Position.X, this.Position.Y + this.Distance + 1);
      var left = new Position(this.Position.X - this.Distance - 1, this.Position.Y);
      var bottom = new Position(this.Position.X, this.Position.Y - this.Distance - 1);
      var right = new Position(this.Position.X + this.Distance + 1, this.Position.Y);
      foreach (var p1 in Position.GetPointsInLine(top, left))
      {
         _ = this.BoundingLines.Add(p1);
      }
      foreach (var p1 in Position.GetPointsInLine(left, bottom))
      {
         _ = this.BoundingLines.Add(p1);
      }
      foreach (var p1 in Position.GetPointsInLine(bottom, right))
      {
         _ = this.BoundingLines.Add(p1);
      }
      foreach (var p1 in Position.GetPointsInLine(right, top))
      {
         _ = this.BoundingLines.Add(p1);
      }
   }
}
