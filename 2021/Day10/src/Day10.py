import argparse

class Chunk:
  '''A chunk can contain other chunks.'''

  _OPEN_TO_CLOSE = {
    '(': ')',
    '[': ']',
    '{': '}',
    '<': '>'
  }

  def __init__(self, line, index=0):
    self.illegal_character = None
    self.corrupt = False
    self.complete = False
    self.end_index = -1
    self.autocomplete = ''

    self.start = line[index]
    if self.start not in Chunk._OPEN_TO_CLOSE:
      self.corrupt = True
      self.illegal_character = self.start
      return

    print(f'Starting new chunk at {index} with {self.start}')

    self.chunks = []
    line_length = len(line)
    index += 1
    while index < line_length:
      current = line[index]
      if current in Chunk._OPEN_TO_CLOSE:
        new_chunk = Chunk(line, index)
        self.chunks.append(new_chunk)
        if new_chunk.corrupt:
          self.corrupt = True
          self.illegal_character = new_chunk.illegal_character
          break
        index = new_chunk.end_index + 1
        if not new_chunk.complete:
          self.autocomplete += new_chunk.autocomplete
          break
      elif current != Chunk._OPEN_TO_CLOSE[self.start]:
        self.corrupt = True
        self.illegal_character = current
        break
      else:
        self.complete = True
        break

    self.end_index = index
    if not self.complete:
      print(f'Chunk is incomplete: our own start is {self.start}')
      self.autocomplete += Chunk._OPEN_TO_CLOSE[self.start]

  def get_corruption_score(self):
    '''Gets the illegal point value of this chunk'''
    if self.illegal_character is None:
      return 0
    if self.illegal_character == ')':
      return 3
    if self.illegal_character == ']':
      return 57
    if self.illegal_character == '}':
      return 1197
    if self.illegal_character == '>':
      return 25137
    return 0

  @staticmethod
  def _get_autocomplete_score(character):
    '''Gets the score for a closing character'''
    if character == ')':
      return 1
    if character == ']':
      return 2
    if character == '}':
      return 3
    if character == '>':
      return 4
    return 0

  def get_autocomplete_score(self):
    '''Gets the score of this chunk's autocomplete string'''
    score = 0
    print(f"Chunk's autocomplete: {self.autocomplete}")
    for c in self.autocomplete:
      score *= 5
      score += Chunk._get_autocomplete_score(c)
    return score

  def get_chunk_str(self, indentation=''):
    '''Gets a formatted string representing the chunk and its subchunks.'''
    chunk_str = None
    if self.corrupt:
      chunk_str = f'{indentation}Corrupt chunk begins with {self.start}\n'
    else:
      chunk_str = f'{indentation}Chunk begins with {self.start}\n'
    for chunk in self.chunks:
      chunk_str += chunk.get_chunk_str(indentation + '\t')
    if self.corrupt:
      chunk_str += f'{indentation}Corrupt chunk ends with {self.illegal_character}\n'
    elif self.complete:
      chunk_str += f'{indentation}Chunk ends with {Chunk._OPEN_TO_CLOSE[self.start]}\n'
    return chunk_str

def get_chunks(filename):
  '''Gets a collection of chunks for each line of the file.'''
  chunks = []
  with open(filename, 'r') as fh:
    for line in fh.readlines():
      line = line.strip()
      index = 0
      row = []
      while index < len(line):
        new_chunk = Chunk(line, index)
        row.append(new_chunk)
        index = new_chunk.end_index
      chunks.append(row)

  return chunks

def main():
  '''Feeling chunky.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  chunk_rows = get_chunks(args.filename)
  print(chunk_rows)
  if args.part == 1:
    score = 0
    for chunk_row in chunk_rows:
      for chunk in chunk_row:
        if chunk.corrupt:
          score += chunk.get_corruption_score()

    print(f'Chunk score: {score}')
  else:
    scores = []
    for chunk_row in chunk_rows:
      for chunk in chunk_row:
        if not chunk.complete:
          scores.append(chunk.get_autocomplete_score())
    print(scores)

if __name__ == "__main__":
  main()
