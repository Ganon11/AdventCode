namespace AdventOfCode.Solutions.Year2022.Day13;

using System.Diagnostics.CodeAnalysis;

internal sealed class Solution : SolutionBase<Tuple<IPacketData, IPacketData>[]>
{
   public Solution() : base(13, 2022, "Distress Signal", true) { }

   public override Tuple<IPacketData, IPacketData>[] ParseInput(string input)
   {
      var result = new List<Tuple<IPacketData, IPacketData>>();
      var paragraphs = input.SplitByParagraph(shouldTrim: true);
      foreach (var paragraph in paragraphs)
      {
         var tokens = paragraph.SplitByNewline(shouldTrim: true);
         result.Add(new(IPacketData.ParsePacket(tokens.First()), IPacketData.ParsePacket(tokens.Last())));
      }

      return result.ToArray();
   }

   public override string SolvePartOne()
   {
      var packets = this.ParsedInput.Copy();
      var sum = 0;
      for (var index = 0; index < this.ParsedInput.Length; ++index)
      {
         var pair = this.ParsedInput[index];
         //Console.WriteLine($"== Pair {index + 1} ==");
         if (pair.Item1.CompareTo(pair.Item2) <= 0)
         {
            sum += index + 1;
         }
      }

      return sum.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var packets = this.ParsedInput.Copy().ToList();
      var newPacket1 = IPacketData.ParsePacket("[[2]]");
      var newPacket2 = IPacketData.ParsePacket("[[6]]");
      packets.Add(new(newPacket1, newPacket2));

      var allPacketsInList = new List<IPacketData>();
      foreach (var pair in packets)
      {
         allPacketsInList.Add(pair.Item1);
         allPacketsInList.Add(pair.Item2);
      }

      allPacketsInList.Sort();
      var decoderKey = (allPacketsInList.IndexOf(newPacket1) + 1) * (allPacketsInList.IndexOf(newPacket2) + 1);
      return decoderKey.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal interface IPacketData : IComparable, IEquatable<IPacketData>
{
   public int CompareTo(IPacketData other);

   public static IPacketData ParsePacket(string packet)
   {
      PacketList? currentList = null;
      var listStack = new Stack<PacketList>();
      var currentNumber = string.Empty;
      int? value = null;
      for (var index = 0; index < packet.Length; ++index)
      {
         switch (packet[index])
         {
            case '[':
               if (currentList != null)
               {
                  listStack.Push(currentList);
               }
               currentList = new PacketList();
               break;
            case ']':
               if (!string.IsNullOrEmpty(currentNumber))
               {
                  value = int.Parse(currentNumber, System.Globalization.CultureInfo.CurrentCulture);
                  currentList.Value.Add(new PacketInteger { Value = value.Value });
                  currentNumber = string.Empty;
               }
               if (listStack.Any())
               {
                  var previousList = listStack.Pop();
                  previousList.Value.Add(currentList);
                  currentList = previousList;
               }
               break;
            case ',':
               // Skip this character
               if (!string.IsNullOrEmpty(currentNumber))
               {
                  value = int.Parse(currentNumber, System.Globalization.CultureInfo.CurrentCulture);
                  currentList.Value.Add(new PacketInteger { Value = value.Value });
                  currentNumber = string.Empty;
               }
               break;
            default:
               // Parse all digits
               currentNumber += packet[index];
               break;
         }
      }

      return currentList;
   }
}

internal sealed record PacketInteger : IPacketData
{
   public int Value { get; set; }

   public int CompareTo(IPacketData other)
   {
      //Console.WriteLine($"Compare {this} vs {other}");

      // Comparing two integers
      var otherInt = other as PacketInteger;
      if (otherInt != null)
      {
         return this.Value.CompareTo(otherInt.Value);
      }

      // Comparing integer to list
      var otherList = other as PacketList;
      if (otherList != null)
      {
         var thisAsList = new PacketList();
         thisAsList.Value.Add(this);
         return thisAsList.CompareTo(other);
      }

      throw new ArgumentException($"Input is neither Integer nor List", nameof(other));
   }

   public int CompareTo(object? obj)
   {
      if (obj is IPacketData otherPacket)
      {
         return this.CompareTo(otherPacket);
      }

      throw new ArgumentException($"Other object is not a packet", nameof(obj));
   }

   public bool Equals(IPacketData? other)
   {
      if (other is PacketInteger otherInt)
      {
         return this.Value.Equals(otherInt.Value);
      }

      return false;
   }

   public override string ToString() => this.Value.ToString(System.Globalization.CultureInfo.CurrentCulture);
}

internal sealed record PacketList : IPacketData
{
   public List<IPacketData> Value { get; set; } = new List<IPacketData>();

   public int CompareTo(IPacketData other)
   {
      //Console.WriteLine($"Compare {this} vs {other}");

      var otherList = other as PacketList;
      if (otherList == null)
      {
         var otherInt = other as PacketInteger;
         if (otherInt == null)
         {
            throw new ArgumentException($"Input is neither Integer nor List", nameof(other));
         }

         otherList = new PacketList();
         otherList.Value.Add(otherInt);
      }

      for (var index = 0; index < this.Value.Count; ++index)
      {
         if (index >= otherList.Value.Count)
         {
            return 1;
         }

         var compareVal = this.Value.ElementAt(index).CompareTo(otherList.Value.ElementAt(index));

         if (compareVal != 0)
         {
            return compareVal;
         }
      }

      if (Enumerable.SequenceEqual(this.Value, otherList.Value))
      {
         return 0;
      }

      return -1;
   }

   public int CompareTo(object? obj)
   {
      if (obj is IPacketData otherPacket)
      {
         return this.CompareTo(otherPacket);
      }

      throw new ArgumentException($"Other object is not a packet", nameof(obj));
   }

   public bool Equals(IPacketData? other)
   {
      if (other is PacketList otherList)
      {
         return Enumerable.SequenceEqual(this.Value, otherList.Value);
      }

      return false;
   }

   public override string ToString() => $"[{string.Join(',', this.Value)}]";
}
