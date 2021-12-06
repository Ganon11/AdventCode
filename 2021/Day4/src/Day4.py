import argparse
from itertools import islice

class BingoSquare:
  def __init__(self, number):
    self.number = number
    self.picked = False

  def get_number(self):
    return self.number

  def get_picked(self):
    return self.picked

  def pick_number(self):
    self.picked = True

  def __str__(self):
    if self.picked:
      return '{0: >5}'.format('_' + str(self.number) + '_')
    else:
      return '{0: >5}'.format(self.number)

class BingoBoard:
  def __init__(self, lines):
    # board will be a 2D array, 5x5, of the numbers
    self.board = []
    # picked will be a 2D
    self.picked = []
    for line in lines:
      row = []
      for number in map(lambda token: int(token), line.strip().split()):
        row.append(BingoSquare(number))
      self.board.append(row)
    pass

  def number_called(self, number):
    for row in self.board:
      number_found = False

      for square in row:
        if square.get_number() == number:
          square.pick_number()
          number_found = True
          break

      if number_found:
        break

  def is_winning(self):
    # Check rows
    #print('Checking rows')
    for row in self.board:
      if all(s.get_picked() for s in row):
        return True

    # Check columns
    #print('Checking columns')
    for c in range(5):
      all_picked = True
      for r in range(5):
        if not self.board[r][c].get_picked():
          all_picked = False
          break

      if all_picked:
        return True

    # Check diagonal (0,0 -> 4,4)
    #print('Checking first diagonal')
    diagonal_picked = True
    for index in range(5):
      if not self.board[index][index].get_picked():
        diagonal_picked = False
        break

    if diagonal_picked:
      return True

    # Check diagonal (0,4 -> 4,0)
    #print('Checking second diagonal')
    diagonal_picked = True
    for addend in range(5):
      if not self.board[addend][4 - addend].get_picked():
        diagonal_picked = False
        break

    if diagonal_picked:
      return True

    return False

  def __str__(self):
    s = ''
    for row in self.board:
      s += ' '.join(map(str, row))
      s += '\n'
    return s

def parse_input(filename):
  lines = []

  with open(filename, 'r') as fh:
    lines = fh.readlines()

  numbers = []
  boards = []
  # First line is the numbers called
  numbers = list(map(lambda token: int(token), lines[0].strip().split(',')))
  # Then a blank line
  index = 2
  while index < len(lines):
    rows = lines[index:index + 5]
    b = BingoBoard(rows)
    boards.append(b)
    index += 6

  return (numbers, boards)

def take_turn(number, boards):
  index = 0
  for board in boards:
    board.number_called(number)
    if board.is_winning():
      return index
    index += 1
  return -1

def play_bingo(numbers, boards):
  # for board in boards:
  #   print(board)

  winning_board = -1
  for index in range(len(numbers)):
    print(f'{numbers[index]}!')
    winning_board = take_turn(numbers[index], boards)
    if winning_board != -1:
      break

  for board in boards:
    print(board)

  if winning_board != -1:
    print('Winner!')
    print(boards[winning_board])

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  numbers, boards = parse_input(args.filename)
  play_bingo(numbers, boards)

if __name__ == "__main__":
  main()
