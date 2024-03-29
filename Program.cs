#pragma warning disable CA1852

using AdventOfCode.Services;
using AdventOfCode.Solutions;

var year = ConfigurationService.GetYear();
var days = ConfigurationService.GetDays();

if (args.Length > 0 && int.TryParse(args.First(), out var day))
{
   days = new[] { day };
}

foreach (var solution in SolutionCollector.FetchSolutions(year, days))
{
   Console.WriteLine(solution.ToString());
}

#pragma warning restore CA1852
