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
         foreach (var boundingPosition in source.GetBoundingLines(bound))
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
         var tuningFrequency = ((long)emptyTarget.X * 4000000) + emptyTarget.Y;
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
   public long Distance { get; init; }

   public Sensor(Position p, Position b)
   {
      this.Position = p;
      this.ClosestBeacon = b;
      this.Distance = this.Position.ManhattanDistance(this.ClosestBeacon);
   }

   public IEnumerable<Position> GetBoundingLines(int bound)
   {
      var top = new Position(this.Position.X, this.Position.Y + this.Distance + 1);
      var left = new Position(this.Position.X - this.Distance - 1, this.Position.Y);

      foreach (var p in Position.GetPointsInLine(top, left))
      {
         if (p.X < 0 || p.X > bound || p.Y < 0 || p.Y > bound)
         {
            continue;
         }

         yield return p;
      }

      var bottom = new Position(this.Position.X, this.Position.Y - this.Distance - 1);
      foreach (var p in Position.GetPointsInLine(left, bottom))
      {
         if (p.X < 0 || p.X > bound || p.Y < 0 || p.Y > bound)
         {
            continue;
         }

         yield return p;
      }

      var right = new Position(this.Position.X + this.Distance + 1, this.Position.Y);
      foreach (var p in Position.GetPointsInLine(bottom, right))
      {
         if (p.X < 0 || p.X > bound || p.Y < 0 || p.Y > bound)
         {
            continue;
         }

         yield return p;
      }

      foreach (var p in Position.GetPointsInLine(right, top))
      {
         if (p.X < 0 || p.X > bound || p.Y < 0 || p.Y > bound)
         {
            continue;
         }

         yield return p;
      }
   }
}
