namespace AdventOfCode.Solutions.Year2022.Day19;
internal sealed class Solution : SolutionBase<Blueprint[]>
{
   public Solution() : base(19, 2022, "Not Enough Minerals", true) { }

   public override Blueprint[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line => new Blueprint(line))
      .ToArray();

   private static int TIME_LIMIT = 24;
   private static long MaxGeodes(Blueprint blueprint)
   {
      var start = new GeodeState
      {
         OreCache = 0,
         ClayCache = 0,
         ObsidianCache = 0,
         GeodeCache = 0,
         OreRobots = 1,
         ClayRobots = 0,
         ObsidianRobots = 0,
         GeodeRobots = 0
      };

      var frontier = new PriorityQueue<GeodeState, long>();
      frontier.Enqueue(start, 0);

      var cameFrom = new Dictionary<GeodeState, GeodeState?>();
      cameFrom[start] = null;

      var costSoFar = new Dictionary<GeodeState, long>();
      costSoFar[start] = 0;

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();

         // Build an ore robot?
         // We have the resources to build an ore robot
         if (current.OreCache >= blueprint.RobotCosts[Resource.Ore][Resource.Ore])
         {
            var newCost = costSoFar[current] + 1;
            if (newCost <= TIME_LIMIT)
            {
               var newState = new GeodeState()
               {
                  OreCache = current.OreCache - blueprint.RobotCosts[Resource.Ore][Resource.Ore] + current.OreRobots,
                  ClayCache = current.ClayCache + current.ClayRobots,
                  ObsidianCache = current.ObsidianCache + current.ObsidianRobots,
                  GeodeCache = current.GeodeCache + current.GeodeRobots,
                  OreRobots = current.OreRobots + 1,
                  ClayRobots = current.ClayRobots,
                  ObsidianRobots = current.ObsidianRobots,
                  GeodeRobots = current.GeodeRobots
               };
               if (!costSoFar.TryGetValue(newState, out var existingCost) || newCost < existingCost)
               {
                  costSoFar[newState] = newCost;
                  frontier.Enqueue(newState, newCost);
                  cameFrom[newState] = current;
               }
            }
         }
         // You require more ore!
         else
         {
            var oreNeeded = blueprint.RobotCosts[Resource.Ore][Resource.Ore] - current.OreCache;

         }

         // Build a clay robot?

         // Build an obsidian robot?

         // Build a geode robot?
      }
   }

   public override string SolvePartOne()
   {
      return "";
   }

   public override string SolvePartTwo()
   {
      return "";
   }
}

internal enum Resource
{
   Ore,
   Clay,
   Obsidian,
   Geode
}

internal class Blueprint
{
   public int Id { get; init; }
   public Dictionary<Resource, Dictionary<Resource, long>> RobotCosts { get; init; }

   public Blueprint(string line)
   {
      var tokens = line.Split(' ');

      this.Id = int.Parse(tokens[1], System.Globalization.CultureInfo.CurrentCulture);

      var oreRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[6], System.Globalization.CultureInfo.CurrentCulture) },
      };
      var clayRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[12], System.Globalization.CultureInfo.CurrentCulture) },
      };
      var obsidianRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[18], System.Globalization.CultureInfo.CurrentCulture) },
         { Resource.Clay, long.Parse(tokens[21], System.Globalization.CultureInfo.CurrentCulture) }
      };
      var geodeRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[27], System.Globalization.CultureInfo.CurrentCulture) },
         { Resource.Obsidian, long.Parse(tokens[30], System.Globalization.CultureInfo.CurrentCulture) }
      };

      this.RobotCosts = new Dictionary<Resource, Dictionary<Resource, long>>
      {
         { Resource.Ore, oreRobotCost },
         { Resource.Clay, clayRobotCost },
         { Resource.Obsidian, obsidianRobotCost },
         { Resource.Geode, geodeRobotCost }
      };
   }
}

internal class GeodeState : IEquatable<GeodeState>
{
   public long OreCache { get; init; }
   public long ClayCache { get; init; }
   public long ObsidianCache { get; init; }
   public long GeodeCache { get; init; }

   public long OreRobots { get; init; }
   public long ClayRobots { get; init; }
   public long ObsidianRobots { get; init;}
   public long GeodeRobots { get; init; }

   public bool Equals(GeodeState? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.OreRobots == other.OreRobots &&
         this.ClayRobots == other.ClayRobots &&
         this.ObsidianRobots == other.ObsidianRobots &&
         this.GeodeRobots == other.GeodeRobots;
   }

   public override bool Equals(object? obj) => this.Equals(obj as GeodeState);
}
