import argparse

def get_starting_positions(filename):
  '''Gets player 1 and 2's starting positions from the file.'''
  lines = None
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  position_1 = int(lines[0].split()[-1])
  position_2 = int(lines[1].split()[-1])

  return (position_1, position_2)

def deterministic_dice():
  '''Rolls a deterministic d100.'''
  deterministic_dice.TOTAL_ROLLS += 3

  if deterministic_dice.NEXT_RESULT == 98:
    deterministic_dice.NEXT_RESULT = 1
    return 297

  if deterministic_dice.NEXT_RESULT == 99:
    deterministic_dice.NEXT_RESULT = 2
    return 200

  if deterministic_dice.NEXT_RESULT == 100:
    deterministic_dice.NEXT_RESULT = 3
    return 103


  total = (deterministic_dice.NEXT_RESULT + 1) * 3
  deterministic_dice.NEXT_RESULT += 3
  return total

deterministic_dice.NEXT_RESULT = 1
deterministic_dice.TOTAL_ROLLS = 0

def play_game(starting_positions, goal_score=1000):
  player = 0
  positions = [starting_positions[0], starting_positions[1]]
  totals = [0, 0]

  #print(f'Player 1 starting position: {positions[0]}')
  #print(f'Player 1 starting position: {positions[1]}')

  while totals[0] < goal_score and totals[1] < goal_score:
    spaces_to_move = deterministic_dice()
    positions[player] += spaces_to_move
    while positions[player] > 10:
      positions[player] -= 10
    totals[player] += positions[player]
    #print(f'Player {player + 1} rolls {spaces_to_move} and moves to space {positions[player]} for a total score of {totals[player]}')
    player = (player + 1) % 2

  return totals[player] * deterministic_dice.TOTAL_ROLLS

def main():
  '''Let's play a game, shall we?'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  starting_positions = get_starting_positions(args.filename)
  result = play_game(starting_positions)
  print(f'Game result: {result}')

if __name__ == "__main__":
  main()
