import collections
import heapq

class Queue:
  """A simple FIFO queue."""

  def __init__(self, iterable=list()):
    self.elements = collections.deque(iterable)

  def empty(self):
    """Returns True if the queue is empty, and False otherwise"""
    return len(self.elements) == 0

  def put(self, x):
    """Appends an item to the Queue"""
    self.elements.append(x)

  def get(self):
    """Pops and returns an item from the Queue"""
    return self.elements.popleft()

  def peek(self):
    """Returns the first element of the Queue without popping."""
    return self.elements[0]

class PriorityQueue:
  """A priority queue."""

  def __init__(self, iterable=list()):
    self.elements = []

  def empty(self):
    """Returns True if the queue is empty, and False otherwise"""
    return len(self.elements) == 0

  def put(self, item, priority):
    """
    Adds an item to the PriorityQueue at the given Priority.

    Priority should be an integer.
    """
    assert isinstance(priority, int)
    heapq.heappush(self.elements, (priority, item))

  def get(self):
    """Pops and returns the item in the queue with the lowest Priority value."""
    return heapq.heappop(self.elements)[1]

  def peek(self):
    """Returns the item with the lowest Priority value without popping."""
    return self.elements[0][1]
