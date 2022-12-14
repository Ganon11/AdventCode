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
   public int Day { get; init; }
   public int Year { get; init; }
   public string Title { get; init; }
   public bool Debug { get; init; }

   private string? input;
   public string Input
   {
      get
      {
         this.input ??= this.LoadInput(this.Debug);
         return this.input;
      }
   }

   private string? debugInput;
   public string DebugInput
   {
      get
      {
         this.debugInput ??= this.LoadInput(debug: true);
         return this.debugInput;
      }
   }

   private T? parsedInput;
   public T ParsedInput
   {
      get
      {
         this.parsedInput ??= this.ParseInput(this.Input);
         return this.parsedInput;
      }
   }

   private T? parsedDebugInput;
   public T ParsedDebugInput
   {
      get
      {
         this.parsedDebugInput ??= this.ParseInput(this.DebugInput);
         return this.parsedDebugInput;
      }
   }

   public SolutionResult Part1 => this.Solve(1);
   public SolutionResult Part2 => this.Solve(2);

   private protected SolutionBase(int day, int year, string title, bool useDebugInput = false)
   {
      this.Day = day;
      this.Year = year;
      this.Title = title;
      this.Debug = useDebugInput;
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
            //throw new InvalidOperationException("DebugInput is null or empty");
            return new SolutionResult { Answer = "DebugInput is null or empty" };
         }
      }
      else if (string.IsNullOrEmpty(this.Input))
      {
         //throw new InvalidOperationException("Input is null or empty");
         return new SolutionResult { Answer = "Input is null or empty" };
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

      if (debug)
      {
         return string.Empty;
      }

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

      return string.Empty;
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

   public static SolutionResult Empty => new();
}
