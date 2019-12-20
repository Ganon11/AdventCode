import my_collections

def test_empty_queue():
  q = my_collections.Queue()
  assert q.empty()

def test_non_empty_queue():
  q = my_collections.Queue()
  q.put(7)
  assert not q.empty()

def test_queue_order():
  q = my_collections.Queue()
  q.put(1)
  q.put(2)
  q.put(3)
  assert q.get() == 1
  assert q.get() == 2
  assert q.get() == 3

def test_queue_get_pops_element():
  q = my_collections.Queue()
  assert q.empty()
  q.put(1)
  assert not q.empty()
  assert q.get() == 1
  assert q.empty()

def test_queue_peek_does_not_pop_element():
  q = my_collections.Queue()
  assert q.empty()
  q.put(1)
  assert not q.empty()
  assert q.peek() == 1
  assert not q.empty()
