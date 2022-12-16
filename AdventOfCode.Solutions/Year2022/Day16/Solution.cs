namespace AdventOfCode.Solutions.Year2022.Day16;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Dictionary<string, Valve>>
{
   public Solution() : base(16, 2022, "Proboscidea Volcanium", true) { }

   //public override Dictionary<string, Valve> ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
   //   .Select(line => new Valve(line))
   //   .ToDictionary(valve => valve.Name);
   public override Dictionary<string, Valve> ParseInput(string input)
   {
      return input.SplitByNewline(shouldTrim: true)
         .Select(line => new Valve(line))
         .ToDictionary(valve => valve.Name);
   }

   public override string SolvePartOne()
   {
      var startState = new SearchState("AA", Enumerable.Empty<Valve>(), pressure: 0, time: 0);
      var frontier = new Queue<SearchState>();
      frontier.Enqueue(startState);
      var reached = new HashSet<SearchState>
      {
         startState
      };

      while (frontier.Any())
      {
         var currentState = frontier.Dequeue();
         var newTime = currentState.Time + 1;
         if (newTime > 30)
         {
            continue;
         }

         var currentValve = this.ParsedInput[currentState.CurrentValve];
         var newPressure = currentState.TotalPressure + currentState.OpenValves.Sum(valve => valve.FlowRate);

         var adjacentStates = new HashSet<SearchState>();

         // See if we can open this valve
         if (!currentState.OpenValves.Contains(currentValve) && this.ParsedInput[currentState.CurrentValve].FlowRate > 0)
         {
            var openValves = new HashSet<Valve>();
            openValves.UnionWith(currentState.OpenValves);
            openValves.Add(currentValve);
            adjacentStates.Add(new SearchState(currentState.CurrentValve, openValves, newPressure, newTime));
         }

         // Which valves can we move to?
         foreach (var neighbor in currentValve.AdjacentValves)
         {
            if (currentState.CameFrom != null && currentState.CameFrom.Equals(neighbor, StringComparison.OrdinalIgnoreCase))
            {
               continue;
            }

            adjacentStates.Add(new SearchState(neighbor, currentState.OpenValves, newPressure, newTime, currentState.CurrentValve));
         }

         // Filter out states
         foreach (var newState in adjacentStates)
         {
            if (reached.TryGetValue(newState, out var alreadyVisitedState))
            {
               if (newState.TotalPressure <= alreadyVisitedState.TotalPressure)
               {
                  reached.Remove(alreadyVisitedState);
                  reached.Add(newState);
               }
            }
            else
            {
               frontier.Enqueue(newState);
               reached.Add(newState);
            }
         }
      }

      return "";
   }

   public override string SolvePartTwo()
   {
      return "";
   }
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
   public HashSet<Valve> OpenValves { get; init; }
   public int TotalPressure { get; init; }
   public int Time { get; init; }
   public string? CameFrom { get; init; }

   public SearchState(string current, IEnumerable<Valve> openValves, int pressure, int time, string? cameFrom = null)
   {
      this.CurrentValve = current;
      this.TotalPressure = pressure;
      this.OpenValves = new HashSet<Valve>();
      this.OpenValves.UnionWith(openValves);
      this.Time = time;
      this.CameFrom = cameFrom;
   }

   bool IEquatable<SearchState>.Equals(SearchState? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.CurrentValve.Equals(other.CurrentValve, StringComparison.OrdinalIgnoreCase)
         && Enumerable.SequenceEqual(this.OpenValves, other.OpenValves)
         && this.Time == other.Time;
   }
}
