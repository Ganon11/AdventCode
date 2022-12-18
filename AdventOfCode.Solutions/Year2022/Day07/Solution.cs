namespace AdventOfCode.Solutions.Year2022.Day07;

internal sealed class Solution : SolutionBase<Solution.ElfDirectory?>
{
   public Solution() : base(07, 2022, "No Space Left On Device") { }

   public override string SolvePartOne()
   {
      if (this.ParsedInput == null)
      {
         return "ERROR";
      }

      return GetAllDirectories(this.ParsedInput).Where(d => d.GetSize() < 100000)
         .Sum(d => d.GetSize())
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      if (this.ParsedInput == null)
      {
         return "ERROR";
      }

      const int TOTAL_DISK_SPACE = 70000000;
      const int TARGET_FREE_SPACE = 30000000;
      var currentFreeSpace = TOTAL_DISK_SPACE - this.ParsedInput.GetSize();
      var amountToDelete = TARGET_FREE_SPACE - currentFreeSpace;

      ElfDirectory? toDelete = null;
      foreach (var dir in GetAllDirectories(this.ParsedInput))
      {
         var currentDirSize = dir.GetSize();
         if (currentDirSize >= amountToDelete)
         {
            if (toDelete == null || currentDirSize < toDelete.GetSize())
            {
               toDelete = dir;
            }
         }
      }

      if (toDelete == null)
      {
         return "ERROR";
      }

      return toDelete.GetSize().ToString(System.Globalization.CultureInfo.CurrentCulture);
   }


   public sealed record ElfFile
   {
      public string Name { get; init; }
      public int Size { get; init; }

      public ElfFile(string name, int size)
      {
         this.Name = name;
         this.Size = size;
      }

      public override string ToString() => $"{this.Name} (file, size={this.Size})";
   }

   public sealed class ElfDirectory
   {
      public string Name { get; init; }
      public List<ElfFile> Files { get; private set; }
      public Dictionary<string, ElfDirectory> Subdirectories { get; private set; }
      public ElfDirectory? Parent { get; init; }

      public ElfDirectory(string name, ElfDirectory? parent)
      {
         this.Name = name;
         this.Files = new List<ElfFile>();
         this.Subdirectories = new Dictionary<string, ElfDirectory>(StringComparer.CurrentCultureIgnoreCase);
         this.Parent = parent;
      }

      private int? size;
      public int GetSize()
      {
         if (!this.size.HasValue)
         {
            this.size = this.Files.Sum(f => f.Size) + this.Subdirectories.Sum(d => d.Value.GetSize());
         }

         return this.size.Value;
      }
   }

   public override ElfDirectory? ParseInput(string input)
   {
      ElfDirectory? current = null, root = null;

      foreach (var line in input.SplitByNewline(shouldTrim: true))
      {
         var tokens = line.Split(' ');
         // Command parsing
         if (tokens[0] == "$")
         {
            // Change directory
            if (tokens[1] == "cd")
            {
               // Special case for root directory
               if (tokens[2] == "/")
               {
                  current = new ElfDirectory("/", parent: null);
                  root = current;
               }
               else
               {
                  if (current == null)
                  {
                     Console.Error.WriteLine($"Tried to cd without a current directory");
                  }
                  // Special case for ..
                  else if (tokens[2] == "..")
                  {
                     current = current.Parent;
                  }
                  else if (current.Subdirectories.TryGetValue(tokens[2], out var newDir))
                  {
                     current = newDir;
                  }
                  else
                  {
                     Console.Error.WriteLine($"Tried to CD into directory {tokens[2]} from {current.Name}");
                  }
               }
            }
            else if (tokens[1] == "ls")
            {
               // Do nothing - output parsing will follow
            }
         }
         // Output parsing
         else
         {
            if (current == null)
            {
               Console.Error.WriteLine($"Tried to parse cd output without a current directory");
            }
            // Directory name
            else if (tokens[0].Equals("dir", StringComparison.OrdinalIgnoreCase))
            {
               current.Subdirectories.Add(tokens[1], new ElfDirectory(tokens[1], current));
            }
            else
            {
               current.Files.Add(new ElfFile(tokens[1], int.Parse(tokens[0], System.Globalization.CultureInfo.CurrentCulture)));
            }
         }
      }

      return root;
   }

   private static List<ElfDirectory> GetAllDirectories(ElfDirectory dir)
   {
      var allDirectories = new List<ElfDirectory>
      {
         dir
      };
      foreach (var subdir in dir.Subdirectories.Select(kvp => kvp.Value))
      {
         allDirectories.AddRange(GetAllDirectories(subdir));
      }

      return allDirectories;
   }
}
