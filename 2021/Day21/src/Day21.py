import argparse
from functools import cache

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

@cache
def get_universe_constants(position, dice_result, total):
  resultant_position = position + dice_result
  if resultant_position > 10:
    resultant_position -= 10
  resultant_total = total + resultant_position
  product = 1
  if dice_result == 4 or dice_result == 8:
    product = 3
  elif dice_result == 5 or dice_result == 7:
    product = 6
  elif dice_result == 6:
    product = 7

  return (resultant_position, resultant_total, product)

@cache
def play_game_in_universe(player, position_0, position_1, total_0, total_1):
  '''We Dr Strange now'''
  if total_0 >= 21:
    return [1, 0]

  if total_1 >= 21:
    return [0, 1]

  wins = [0, 0]
  nextplayer = (player + 1) % 2

  if player == 0:
    for dice_result in range(3, 10):
      (r_position, r_total, product) = get_universe_constants(position_0, dice_result, total_0)
      temp_wins = play_game_in_universe(nextplayer, r_position, position_1, r_total, total_1)
      wins[0] += temp_wins[0] * product
      wins[1] += temp_wins[1] * product

##################### PLAYER 2 #####################
  elif player == 1:
    for dice_result in range(3, 10):
      (r_position, r_total, product) = get_universe_constants(position_1, dice_result, total_1)
      temp_wins = play_game_in_universe(nextplayer, position_0, r_position, total_0, r_total)
      wins[0] += temp_wins[0] * product
      wins[1] += temp_wins[1] * product

  return wins

def play_game(starting_positions, goal_score=1000):
  player = 0
  positions = [starting_positions[0], starting_positions[1]]
  totals = [0, 0]

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
  if args.part == 1:
    result = play_game(starting_positions)
    print(f'Game result: {result}')
  elif args.part == 2:
    wins = play_game_in_universe(0, starting_positions[0], starting_positions[1], 0, 0)
    print(wins)

if __name__ == "__main__":
  main()
