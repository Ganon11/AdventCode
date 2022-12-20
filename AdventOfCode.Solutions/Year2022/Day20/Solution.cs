namespace AdventOfCode.Solutions.Year2022.Day20;
internal sealed class Solution : SolutionBase<long[]>
{
   public Solution() : base(20, 2022, "Grove Positioning System", false) { }

   public override long[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line => long.Parse(line, System.Globalization.CultureInfo.CurrentCulture))
      .ToArray();

   private static void Mix(List<MyLinkedListNode> allNodes)
   {
      foreach (var node in allNodes)
      {
         long effectiveVal = node.Data % (allNodes.Count - 1);
         if (effectiveVal == 0)
         {
            continue;
         }

         // Remove from current spot
         node.Previous.Next = node.Next;
         node.Next.Previous = node.Previous;

         // Move through the list
         var newPrev = node.Previous;
         if (node.Data < 0)
         {
            for (var index = 0; index > effectiveVal; --index)
            {
               newPrev = newPrev.Previous;
            }
         }
         else if (node.Data > 0)
         {
            for (var index = 0; index < effectiveVal; ++index)
            {
               newPrev = newPrev.Next;
            }
         }

         // Insert at new spot
         node.Next = newPrev.Next;
         node.Previous = newPrev;
         node.Next.Previous = node;
         node.Previous.Next = node;
      }
   }

   private static long CalculateSum(MyLinkedListNode zeroNode)
   {
      var current = zeroNode;
      long sum = 0;
      for (var count = 0; count <= 3000; ++count)
      {
         if (count % 1000 == 0)
         {
            sum += current.Data;
         }

         current = current.Next;
      }

      return sum;
   }

   public override string SolvePartOne()
   {
      var (allNodes, zeroNode) = MyLinkedListNode.Create(this.ParsedInput);
      Mix(allNodes);
      return CalculateSum(zeroNode).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   private const long ENCRYPTION_KEY = 811589153;

   public override string SolvePartTwo()
   {
      var (allNodes, zeroNode) = MyLinkedListNode.Create(this.ParsedInput);

      foreach (var node in allNodes)
      {
         node.Data *= ENCRYPTION_KEY;
      }

      for (var index = 0; index < 10; ++index)
      {
         Mix(allNodes);
      }

      return CalculateSum(zeroNode).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal class MyLinkedListNode
{
   public long Data { get; set; }
   public MyLinkedListNode? Previous { get; set; }
   public MyLinkedListNode? Next { get; set; }

   public MyLinkedListNode(long data)
   {
      this.Data = data;
      this.Previous = null;
      this.Next = null;
   }

   public static (List<MyLinkedListNode>, MyLinkedListNode) Create(long[] data)
   {
      var allNodes = new List<MyLinkedListNode>();
      MyLinkedListNode zeroNode = null;
      foreach (var number in data)
      {
         var newNode = new MyLinkedListNode(number);
         allNodes.Add(newNode);
         if (number == 0)
         {
            zeroNode = newNode;
         }
      }

      if (zeroNode == null)
      {
         throw new ArgumentException($"Where's the 0!?", nameof(data));
      }

      allNodes.First().Previous = allNodes.Last();
      for (var i = 1; i < allNodes.Count; ++i)
      {
         allNodes.ElementAt(i).Previous = allNodes.ElementAt(i - 1);
      }
      for (var i = 0; i < allNodes.Count - 1; ++i)
      {
         allNodes.ElementAt(i).Next = allNodes.ElementAt(i + 1);
      }
      allNodes.Last().Next = allNodes.First();

      return (allNodes, zeroNode);
   }
}
