namespace AdventOfCode.Solutions.Year2022.Day16;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Dictionary<string, Valve>>
{
   public Solution() : base(16, 2022, "Proboscidea Volcanium", false) { }

   public override Dictionary<string, Valve> ParseInput(string input)
   {
      var lines = input.SplitByNewline(shouldTrim: true).ToList();
      lines.Sort();

      var valves = new List<Valve>();
      for (var index = 0; index < lines.Count; ++index)
      {
         valves.Add(new Valve(lines[index], 1 << index));
      }

      return valves.ToDictionary(v => v.Name);
   }

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

   private static SearchState[] CalculateScores(Dictionary<string, Valve> valves, int timeLimit)
   {
      var maxScore = long.MinValue;
      var usefulValves = valves.Where(kvp => kvp.Value.FlowRate > 0)
         .Select(kvp => kvp.Key)
         .ToHashSet();

      var start = new SearchState("AA", 0, 0);

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

         foreach (var usefulValve in usefulValves)
         {
            if ((valves[usefulValve].Id & current.OpenValves) > 0)
            {
               continue;
            }

            var newCost = costSoFar[current] + CostToActivate(current.CurrentValve, usefulValve, valves);
            if (newCost > timeLimit)
            {
               continue;
            }

            long remainingTime = timeLimit - newCost;

            var newOpenValves = current.OpenValves | valves[usefulValve].Id;

            var newScore = current.TotalPressure + (valves[usefulValve].FlowRate * remainingTime);

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

      return cameFrom.Keys
         .Where(state => state.TotalPressure > 0)
         .OrderByDescending(state => state.TotalPressure)
         .ToArray();
   }

   public override string SolvePartOne()
   {
      var scores = CalculateScores(this.ParsedInput, 30);
      return scores.First().TotalPressure.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var scores = CalculateScores(this.ParsedInput, 26);
      var maxScore = long.MinValue;
      for (var index = 0; index < scores.Length; ++index)
      {
         for (var index2 = index + 1; index2 < scores.Length; ++index2)
         {
            if ((scores[index].OpenValves & scores[index2].OpenValves) > 0)
            {
               continue;
            }

            if (scores[index].TotalPressure + scores[index2].TotalPressure > maxScore)
            {
               maxScore = scores[index].TotalPressure + scores[index2].TotalPressure;
            }
         }
      }

      return maxScore.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal sealed class IntMaxCompare : IComparer<long>
{
   public int Compare(long x, long y) => y.CompareTo(x);
}

internal sealed partial class Valve : IEquatable<Valve>
{
   public long Id { get; init; }
   public string Name { get; init; }
   public int FlowRate { get; init; }
   public string[] AdjacentValves { get; init; }

   public Valve(string line, long id)
   {
      var m = ValveRegex().Match(line);
      if (m.Success)
      {
         this.Id = id;
         this.Name = m.Groups["name"].Value;
         this.FlowRate = int.Parse(m.Groups["flowRate"].Value, System.Globalization.CultureInfo.CurrentCulture);
         this.AdjacentValves = m.Groups["valves"].Value.Split(", ").ToArray();
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

   public override bool Equals(object? obj) => ((IEquatable<Valve>)this).Equals(obj as Valve);

   public override int GetHashCode() => this.Name.GetHashCode();
}

internal sealed record SearchState : IEquatable<SearchState>
{
   public string CurrentValve { get; init; }
   public long OpenValves { get; init; }
   public long TotalPressure { get; init; }

   public SearchState(string current, long valves, long pressure)
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

      return this.CurrentValve == other.CurrentValve && this.OpenValves == other.OpenValves;
   }
}
