namespace AdventOfCode.Solutions.Year2022.Day16;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Dictionary<string, Valve>>
{
   public Solution() : base(16, 2022, "Proboscidea Volcanium", false) { }

   public override Dictionary<string, Valve> ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
         .Select(line => new Valve(line))
         .ToDictionary(valve => valve.Name);

   private static readonly Dictionary<(string, string), int> PrecomputedCosts = new();
   private static int CostToActivate(string source, string destination, Dictionary<string, Valve> valves)
   {
      if (PrecomputedCosts.TryGetValue((source, destination), out var value))
      {
         return value;
      }

      var frontier = new Queue<string>();
      frontier.Enqueue(source);
      var cameFrom = new Dictionary<string, string>
      {
         { source, string.Empty }
      };

      while (frontier.Any())
      {
         var current = frontier.Dequeue();
         foreach (var next in valves[current].AdjacentValves)
         {
            if (!cameFrom.ContainsKey(next))
            {
               frontier.Enqueue(next);
               cameFrom[next] = current;
            }
         }
      }

      var cost = 1;
      var currentUnwind = destination;
      while (!currentUnwind.Equals(source, StringComparison.OrdinalIgnoreCase))
      {
         ++cost;
         currentUnwind = cameFrom[currentUnwind];
      }

      PrecomputedCosts.Add((source, destination), cost);
      PrecomputedCosts.Add((destination, source), cost);
      return cost;
   }

   public override string SolvePartOne()
   {
      var maxScore = long.MinValue;
      var usefulValves = this.ParsedInput
         .Where(kvp => kvp.Value.FlowRate > 0)
         .Select(kvp => kvp.Key)
         .ToHashSet();

      var start = new SearchState("AA", new HashSet<string>(), 0);

      var frontier = new PriorityQueue<SearchState, long>(new IntMaxCompare());
      frontier.Enqueue(start, 0);

      var cameFrom = new Dictionary<SearchState, SearchState?>
      {
         { start, null }
      };

      var costSoFar = new Dictionary<SearchState, int>
      {
         { start, 0 }
      };

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();

         if (current.TotalPressure > maxScore)
         {
            maxScore = current.TotalPressure;
         }

         foreach (var usefulValve in usefulValves.Except(current.OpenValves))
         {
            var newCost = costSoFar[current] + CostToActivate(current.CurrentValve, usefulValve, this.ParsedInput);
            if (newCost > 30)
            {
               continue;
            }

            long remainingTime = 30 - newCost;

            var newOpenValves = new HashSet<string>(current.OpenValves);
            _ = newOpenValves.Add(usefulValve);

            var newScore = current.TotalPressure + (this.ParsedInput[usefulValve].FlowRate * remainingTime);

            var newState = new SearchState(usefulValve, newOpenValves, newScore);
            if (!costSoFar.TryGetValue(newState, out var existingCost) || newCost < existingCost)
            {
               costSoFar[newState] = newCost;
               var newPriority = newCost;
               frontier.Enqueue(newState, newPriority);
               cameFrom[newState] = current;
            }
         }
      }

      return maxScore.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      return "";
   }
}

internal sealed class IntMaxCompare : IComparer<long>
{
   public int Compare(long x, long y) => y.CompareTo(x);
}

internal sealed partial record Valve : IEquatable<Valve>
{
   public string Name { get; init; }
   public int FlowRate { get; init; }
   public HashSet<string> AdjacentValves { get; init; }

   public Valve(string line)
   {
      var m = ValveRegex().Match(line);
      if (m.Success)
      {
         this.Name = m.Groups["name"].Value;
         this.FlowRate = int.Parse(m.Groups["flowRate"].Value, System.Globalization.CultureInfo.CurrentCulture);
         this.AdjacentValves = m.Groups["valves"].Value.Split(", ").ToHashSet();
      }
      else
      {
         throw new ArgumentException($"Line {line} improperly formatted.", nameof(line));
      }
   }

   bool IEquatable<Valve>.Equals(Valve? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.Name.Equals(other.Name, StringComparison.OrdinalIgnoreCase);
   }

   [GeneratedRegex("Valve (?<name>..) has flow rate=(?<flowRate>\\d+); tunnels? leads? to valves? (?<valves>.+)")]
   private static partial Regex ValveRegex();
}

internal sealed record SearchState : IEquatable<SearchState>
{
   public string CurrentValve { get; init; }
   public HashSet<string> OpenValves { get; init; }
   public long TotalPressure { get; init; }

   public SearchState(string current, HashSet<string> valves, long pressure)
   {
      this.CurrentValve = current;
      this.OpenValves = valves;
      this.TotalPressure = pressure;
   }

   bool IEquatable<SearchState>.Equals(SearchState? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.CurrentValve.Equals(other.CurrentValve, StringComparison.OrdinalIgnoreCase)
         && Enumerable.SequenceEqual(this.OpenValves, other.OpenValves);
   }
}
