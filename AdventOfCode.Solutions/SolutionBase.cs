#pragma warning disable IDE0065 // Misplaced using directive
global using System;
global using System.Collections.Generic;
global using System.Linq;
global using System.Text;
global using AdventOfCode.Solutions.Utils;
#pragma warning restore IDE0065 // Misplaced using directive

namespace AdventOfCode.Solutions;

using System.Diagnostics;
using System.Net;
using AdventOfCode.Services;

public abstract class SolutionBase<T> : ISolution
{
   public int Day { get; }
   public int Year { get; }
   public string Title { get; }
   public bool Debug { get; set; }
   public string Input { get; init; }
   public string DebugInput { get; init; }
   public T ParsedInput { get; init; }
   public T ParsedDebugInput { get; init; }

   public SolutionResult Part1 => this.Solve(1);
   public SolutionResult Part2 => this.Solve(2);

   private protected SolutionBase(int day, int year, string title, bool useDebugInput = false)
   {
      this.Day = day;
      this.Year = year;
      this.Title = title;
      this.Debug = useDebugInput;

      this.Input = this.LoadInput(this.Debug);
      this.DebugInput = this.LoadInput(true);

      this.ParsedInput = this.ParseInput(this.Input);
      this.ParsedDebugInput = this.ParseInput(this.DebugInput);
   }

   public IEnumerable<SolutionResult> SolveAll()
   {
      yield return this.Solve(this.SolvePartOne);
      yield return this.Solve(this.SolvePartTwo);
   }

   public SolutionResult Solve(int part = 1)
   {
      if (part == 1)
      {
         return this.Solve(this.SolvePartOne);
      }

      if (part == 2)
      {
         return this.Solve(this.SolvePartTwo);
      }

      throw new InvalidOperationException("Invalid part param supplied.");
   }

   private SolutionResult Solve(Func<string> solverFunction)
   {
      if (this.Debug)
      {
         if (string.IsNullOrEmpty(this.DebugInput))
         {
#pragma warning disable CA2201 // Exception type System.Exception is not sufficiently specific
            throw new Exception("DebugInput is null or empty");
#pragma warning restore CA2201
         }
      }
      else if (string.IsNullOrEmpty(this.Input))
      {
#pragma warning disable CA2201 // Exception type System.Exception is not sufficiently specific
         throw new Exception("Input is null or empty");
#pragma warning restore CA2201
      }

      try
      {
         var then = DateTime.Now;
         var result = solverFunction();
         var now = DateTime.Now;
         return string.IsNullOrEmpty(result)
             ? SolutionResult.Empty
             : new SolutionResult { Answer = result, Time = now - then };
      }
      catch (Exception)
      {
         if (Debugger.IsAttached)
         {
            Debugger.Break();
            return SolutionResult.Empty;
         }
         else
         {
            throw;
         }
      }
   }

   private string LoadInput(bool debug = false)
   {
      var inputFilepath =
          $"./AdventOfCode.Solutions/Year{this.Year}/Day{this.Day:D2}/{(debug ? "debug" : "input")}";

      if (File.Exists(inputFilepath) && new FileInfo(inputFilepath).Length > 0)
      {
         return File.ReadAllText(inputFilepath);
      }

      if (debug) return "";

      try
      {
         var input = AdventOfCodeService.FetchInput(this.Year, this.Day).Result;
         File.WriteAllText(inputFilepath, input);
         return input;
      }
      catch (HttpRequestException e)
      {
         var code = e.StatusCode;
         var colour = Console.ForegroundColor;
         Console.ForegroundColor = ConsoleColor.DarkRed;
         if (code == HttpStatusCode.BadRequest)
         {
            Console.WriteLine($"Day {this.Day}: Received 400 when attempting to retrieve puzzle input. Your session cookie is probably not recognized.");

         }
         else if (code == HttpStatusCode.NotFound)
         {
            Console.WriteLine($"Day {this.Day}: Received 404 when attempting to retrieve puzzle input. The puzzle is probably not available yet.");
         }
         else
         {
            Console.ForegroundColor = colour;
            Console.WriteLine(e.ToString());
         }
         Console.ForegroundColor = colour;
      }
      catch (InvalidOperationException)
      {
         var colour = Console.ForegroundColor;
         Console.ForegroundColor = ConsoleColor.DarkYellow;
         Console.WriteLine($"Day {this.Day}: Cannot fetch puzzle input before given date (Eastern Standard Time).");
         Console.ForegroundColor = colour;
      }

      return "";
   }

   public override string ToString() =>
       $"--- Day {this.Day}: {this.Title} --- {(this.Debug ? "!! Debug mode active, using DebugInput !!" : "")}\n"
       + $"{ResultToString(1, this.Part1)}\n"
       + $"{ResultToString(2, this.Part2)}\n";

   private static string ResultToString(int part, SolutionResult result) =>
       $"  - Part{part} => " + (string.IsNullOrEmpty(result.Answer)
           ? "Unsolved"
           : $"{result.Answer} ({result.Time.TotalMilliseconds}ms)");

   public virtual string SolvePartOne() => throw new NotImplementedException();
   public virtual string SolvePartTwo() => throw new NotImplementedException();
   public abstract T ParseInput(string input);
}

public abstract class SolutionBase : SolutionBase<string>
{
   private protected SolutionBase(int day, int year, string title, bool useDebugInput = false) : base(day, year, title, useDebugInput) { }

   public override string ParseInput(string input) => input;
}

public interface ISolution
{
   public string SolvePartOne();
   public string SolvePartTwo();

}

public struct SolutionResult
{
   public string Answer { get; set; }
   public TimeSpan Time { get; set; }

   public static SolutionResult Empty => new SolutionResult();
}
