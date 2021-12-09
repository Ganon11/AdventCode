import argparse

class BingoSquare: # pylint: disable=R0903
  '''Represents a square in a bingo board.'''
  def __init__(self, number):
    self.number = number
    self.picked = False

class BingoBoard:
  '''Represents a bingo board.'''
  def __init__(self, lines):
    self.board = [[BingoSquare(int(n)) for n in line.strip().split()] \
     for line in lines]

  def number_called(self, number):
    '''
    A number has been called - check for any matching squares and 'pick' them.
    '''
    for row in self.board:
      number_found = False

      for square in row:
        if square.number == number:
          square.picked = True
          number_found = True
          break

      if number_found:
        break

  def is_winning(self):
    '''Determines if this board is winning.'''
    for row in self.board:
      if all(s.picked for s in row):
        return True

    for c in range(5):
      all_picked = True
      for r in range(5):
        if not self.board[r][c].picked:
          all_picked = False
          break

      if all_picked:
        return True

    return False

  def score(self, number):
    '''
    Calculates the score of the board by summing the unpicked squares, then
    multiplying by the last-picked number.
    '''
    total = sum([sum([s.number for s in row if not s.picked]) for row in self.board])
    return total * number

def parse_input(filename):
  '''Parses the input file to get the numbers called and the boards.'''
  lines = []

  with open(filename, 'r') as fh:
    lines = fh.readlines()

  numbers = []
  boards = []
  # First line is the numbers called
  numbers = list(map(int, lines[0].strip().split(',')))
  # Then a blank line
  index = 2
  while index < len(lines):
    boards.append(BingoBoard(lines[index:index + 5]))
    index += 6

  return (numbers, boards)

def take_turn(number, boards, ignored):
  '''Given the number called, informs each board of the called number.'''
  winning_indices = list()
  for index, board in enumerate(boards):
    if index in ignored:
      continue

    board.number_called(number)
    if board.is_winning():
      winning_indices.append(index)

  return winning_indices

def play_bingo(numbers, boards, end_early):
  '''
  Let's play Bingo!

  Given a list of numbers to call and player boards, calls one number at a time
  until there is a winning board!
  '''
  winning_boards = list()
  ignored = dict()
  winning_number = None
  for number in numbers:
    winners = take_turn(number, boards, ignored)

    if winners:
      for winner in winners:
        ignored[winner] = True
        winning_boards.append(winner)
      winning_number = number
      if end_early or len(winning_boards) == len(boards):
        break

  if winning_boards:
    winner = winning_boards[-1]
    print(f'Score is {boards[winner].score(winning_number)}')

def main():
  '''Plays Bingo with a given input file.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  numbers, boards = parse_input(args.filename)
  end_early = True
  if args.part == 2:
    end_early = False
  play_bingo(numbers, boards, end_early)

if __name__ == "__main__":
  main()
