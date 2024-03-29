namespace AdventOfCode.PositionTests;

[TestClass]
public class ComparisonTests
{
   [TestMethod]
   public void Test_ComparisonsWithSelf()
   {
      Position.Position p = new();

      Assert.AreEqual(p, p);
      Assert.AreEqual(0, p.CompareTo(p));
      Assert.IsTrue(p.Equals(p));
#pragma warning disable CS1718 // Comparison made to same variable
      Assert.IsTrue(p == p);
      Assert.IsTrue(p <= p);
      Assert.IsTrue(p >= p);

      Assert.IsFalse(p != p);
      Assert.IsFalse(p < p);
      Assert.IsFalse(p > p);
#pragma warning restore CS1718 // Comparison made to same variable
   }

   [TestMethod]
   public void Test_Comparisons()
   {
      Position.Position p1 = new();
      Position.Position p2 = new(1, 2, 3);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }

   [TestMethod]
   public void Test_XComparisons()
   {
      Position.Position p1 = new(0, 0, 0);
      Position.Position p2 = new(1, 0, 0);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }

   [TestMethod]
   public void Test_YComparisons()
   {
      Position.Position p1 = new(0, 0, 0);
      Position.Position p2 = new(0, 1, 0);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }

   [TestMethod]
   public void Test_ZComparisons()
   {
      Position.Position p1 = new(0, 0, 0);
      Position.Position p2 = new(0, 0, 1);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }

   [TestMethod]
   public void Test_Z_Trumps_X_In_Comparisons()
   {
      Position.Position p1 = new(0, 0, 0);
      Position.Position p2 = new(-1, 0, 1);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }

   [TestMethod]
   public void Test_Y_Trumps_X_In_Comparisons()
   {
      Position.Position p1 = new(0, 0, 0);
      Position.Position p2 = new(0, -1, 1);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }

   [TestMethod]
   public void Test_Z_Trumps_Y_In_Comparisons()
   {
      Position.Position p1 = new(0, 0, 0);
      Position.Position p2 = new(-1, 1, 0);

      Assert.AreNotEqual(p1, p2);
      Assert.AreEqual(1, p1.CompareTo(p2));
      Assert.AreEqual(-1, p2.CompareTo(p1));

      Assert.IsFalse(p1 == p2);
      Assert.IsFalse(p1 <= p2);
      Assert.IsTrue(p1 >= p2);

      Assert.IsTrue(p1 != p2);
      Assert.IsFalse(p1 < p2);
      Assert.IsTrue(p1 > p2);
   }
}
