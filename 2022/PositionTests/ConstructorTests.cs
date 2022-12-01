namespace PositionTests;

 [TestClass]
 public class ConstructorTests
 {
    [TestMethod]
    public void Test_Default_Constructor()
    {
       Position.Position p = new();
       Assert.AreEqual(0, p.X);
       Assert.AreEqual(0, p.Y);
       Assert.AreEqual(0, p.Z);
    }

    [TestMethod]
    public void Test_Constructor_One_Argument()
    {
       Position.Position p = new(1);
       Assert.AreEqual(1, p.X);
       Assert.AreEqual(0, p.Y);
       Assert.AreEqual(0, p.Z);
    }

    [TestMethod]
    public void Test_Constructor_Two_Arguments()
    {
       Position.Position p = new(1, 2);
       Assert.AreEqual(1, p.X);
       Assert.AreEqual(2, p.Y);
       Assert.AreEqual(0, p.Z);
    }

    [TestMethod]
    public void Test_Constructor_Three_Arguments()
    {
       Position.Position p = new(1, 2, 3);
       Assert.AreEqual(1, p.X);
       Assert.AreEqual(2, p.Y);
       Assert.AreEqual(3, p.Z);
    }
 }
