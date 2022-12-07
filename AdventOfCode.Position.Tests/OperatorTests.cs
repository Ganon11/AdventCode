namespace PositionTests;

[TestClass]
public class OperatorTests
{
   [TestMethod]
   public void Test_Add()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(3, 4, 5);

      var result1 = p1 + p2;
      var result2 = p2 + p1;
      Assert.AreEqual(result1, result2);
      Assert.AreEqual(4, result1.X);
      Assert.AreEqual(6, result1.Y);
      Assert.AreEqual(8, result1.Z);
   }
}
