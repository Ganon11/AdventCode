import collections
import heapq

class Queue:
  """A simple FIFO queue."""

  def __init__(self, iterable=None):
    if iterable is None:
      self._elements = collections.deque()
    else:
      self._elements = collections.deque(iterable.copy())

  def empty(self):
    """Returns True if the queue is empty, and False otherwise"""
    return len(self._elements) == 0

  def put(self, x):
    """Appends an item to the Queue"""
    self._elements.append(x)

  def get(self):
    """Pops and returns an item from the Queue"""
    return self._elements.popleft()

  def peek(self):
    """Returns the first element of the Queue without popping."""
    return self._elements[0]

class PriorityQueue:
  """A priority queue."""

  def __init__(self, iterable=None):
    if iterable is None:
      self._elements = []
    else:
      self._elements = iterable.copy()
      heapq.heapify(self._elements)

  def empty(self):
    """Returns True if the queue is empty, and False otherwise"""
    return not self._elements

  def put(self, item, priority):
    """
    Adds an item to the PriorityQueue at the given Priority.

    Priority should be an integer.
    """
    heapq.heappush(self._elements, (priority, item))

  def get(self):
    """Pops and returns the item in the queue with the lowest Priority value."""
    return heapq.heappop(self._elements)[1]

  def peek(self):
    """Returns the item with the lowest Priority value without popping."""
    return self._elements[0][1]
