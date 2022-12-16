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
      var minX = long.MaxValue;
      var maxX = long.MinValue;
      foreach (var sensor in this.ParsedInput)
      {
         // Does the generated diamond intersect the target row?
         var distanceToTargetRow = sensor.Position.ManhattanDistance(new Position(sensor.Position.X, targetRow));
         if (distanceToTargetRow <= sensor.Distance)
         {
            // Points could contribute to solution
            var horizontalRange = sensor.Distance - distanceToTargetRow;
            var left = sensor.Position.X - horizontalRange;
            var right = sensor.Position.X + horizontalRange;
            if (left < minX)
            {
               minX = left;
            }

            if (right > maxX)
            {
               maxX = right;
            }
         }
      }

      var beaconsOnTargetRow = this.ParsedInput
         .Select(sensor => sensor.ClosestBeacon)
         .Distinct()
         .Where(b => b.Y == targetRow && minX <= b.X && b.X <= maxX);

      var blocked = 1 + maxX - minX - beaconsOnTargetRow.Count();

      return blocked.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

#pragma warning disable CS8604,CS8625
   public override string SolvePartTwo()
   {
      var bound = this.Debug ? 20 : 4000000;
      Position? emptyTarget = null;

      var duplicateFormulas = new HashSet<Line>();
      var formulaCache = new HashSet<Line>();
      foreach (var line in this.ParsedInput.SelectMany(sensor => sensor.GetLines()))
      {
         if (formulaCache.Contains(line))
         {
            _ = duplicateFormulas.Add(line);
         }
         else
         {
            _ = formulaCache.Add(line);
         }
      }

      var intersections = new HashSet<Position>();
      for (var index1 = 0; index1 < duplicateFormulas.Count; ++index1)
      {
         for (var index2 = index1 + 1; index2 < duplicateFormulas.Count; ++index2)
         {
            Position? intersection = Line.Intersection(duplicateFormulas.ElementAt(index1), duplicateFormulas.ElementAt(index2));
            if (intersection == null)
            {
               continue;
            }

            if (intersection.X < 0 || intersection.X > bound || intersection.Y < 0 || intersection.Y > bound)
            {
               continue;
            }

            _ = intersections.Add(intersection);
         }
      }
      
      foreach (var intersection in intersections)
      {
         var clearIntersection = true;
         foreach (var sensor in this.ParsedInput)
         {
            if (sensor.Position.ManhattanDistance(intersection) <= sensor.Distance)
            {
               clearIntersection = false;
               break;
            }
         }

         if (clearIntersection)
         {
            emptyTarget = intersection;
            break;
         }
      }

      if (emptyTarget != null)
      {
         var tuningFrequency = (emptyTarget.X * 4000000) + emptyTarget.Y;
         return tuningFrequency.ToString(System.Globalization.CultureInfo.CurrentCulture);
      }

      return "";
   }
#pragma warning restore CS8604, CS8625

   [GeneratedRegex("Sensor at x=(?<sensorX>.+), y=(?<sensorY>.+): closest beacon is at x=(?<beaconX>.+), y=(?<beaconY>.+)")]
   private static partial Regex SensorBeaconRegex();
}

// Line of the form y = mx + b
internal sealed record Line
{
   public long Slope { get; init; }
   public long Intercept { get; init; }

   public Line(int slope, long intercept)
   {
      this.Slope = slope;
      this.Intercept = intercept;
   }

   public Line(Position p1, Position p2)
   {
      this.Slope = (p2.Y - p1.Y) / (p2.X - p1.X);
      // y = mx + b
      // y - mx = b
      this.Intercept = p1.Y - (this.Slope * p1.X);
   }

   public static Position? Intersection(Line l1, Line l2)
   {
      if (l1.Slope == l2.Slope)
      {
         return null;
      }

      // y = m1x + b1
      // y = m2x + b2
      // m1x + b1 = m2x + b2
      // m1x = m2x + b2 - b1
      // m1x - m2x = b2 - b1
      // x(m1 - m2) = b2 - b1
      // x = (b2 - b1) / (m1 - m2)
      long x = (l2.Intercept - l1.Intercept) / (l1.Slope - l2.Slope);
      long y = (l1.Slope * x) + l1.Intercept;

      return new Position(x, y);
   }
}

internal sealed class Sensor
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

   public IEnumerable<Line> GetLines()
   {
      var top = new Position(this.Position.X, this.Position.Y + this.Distance + 1);
      var left = new Position(this.Position.X - this.Distance - 1, this.Position.Y);
      var bottom = new Position(this.Position.X, this.Position.Y - this.Distance - 1);
      var right = new Position(this.Position.X + this.Distance + 1, this.Position.Y);
      yield return new Line(top, left);
      yield return new Line(left, bottom);
      yield return new Line(bottom, right);
      yield return new Line(right, top);
   }
}
