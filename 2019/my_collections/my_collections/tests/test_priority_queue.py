import pytest

import my_collections

def test_empty_priority_queue():
  q = my_collections.PriorityQueue()
  assert q.empty()

def test_non_empty_priority_queue():
  q = my_collections.PriorityQueue()
  q.put("Item 7", 7)
  assert not q.empty()

def test_priority_queue_order():
  q = my_collections.PriorityQueue()
  q.put("Item 2", 2)
  q.put("Item 1", 1)
  q.put("Item 3", 3)
  assert q.get() == "Item 1"
  assert q.get() == "Item 2"
  assert q.get() == "Item 3"

def test_priority_queue_get_pops_element():
  q = my_collections.PriorityQueue()
  assert q.empty()
  q.put("Item 1", 1)
  assert not q.empty()
  assert q.get() == "Item 1"
  assert q.empty()

def test_priority_queue_peek_does_not_pop_element():
  q = my_collections.PriorityQueue()
  assert q.empty()
  q.put("Item 1", 1)
  assert not q.empty()
  assert q.peek() == "Item 1"
  assert not q.empty()

def test_priority_queue_does_not_allow_non_integer_priority():
  q = my_collections.PriorityQueue()
  with pytest.raises(AssertionError):
    q.put("Item 1", "Priority")
