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
def play_game_in_universe(player, position_0, position_1, total_0, total_1):
  '''We Dr Strange now'''
  if total_0 >= 21:
    return [1, 0]

  if total_1 >= 21:
    return [0, 1]

  wins = [0, 0]
  nextplayer = (player + 1) % 2
  if player == 0:
    # Roll a 3 in 1 universe
    position_0 += 3
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0]
    wins[1] += temp_wins[1]

    # Roll a 4 in 3 universes
    position_0 += 1
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0] * 3
    wins[1] += temp_wins[1] * 3

    # Roll a 5 in 6 universes
    position_0 += 1
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0] * 6
    wins[1] += temp_wins[1] * 6

    # Roll a 6 in 7 universes
    position_0 += 1
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0] * 7
    wins[1] += temp_wins[1] * 7

    # Roll a 7 in 6 universes
    position_0 += 1
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0] * 6
    wins[1] += temp_wins[1] * 6

    # Roll an 8 in 3 universes
    position_0 += 1
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0] * 3
    wins[1] += temp_wins[1] * 3

    # Roll an 9 in 1 universe
    position_0 += 1
    if position_0 > 10:
      position_0 -= 10
    temp_total = total_0 + position_0
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, temp_total, total_1)
    wins[0] += temp_wins[0]
    wins[1] += temp_wins[1]

##################### PLAYER 2 #####################
  elif player == 1:
    # Roll a 3 in 1 universe
    position_1 += 3
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0]
    wins[1] += temp_wins[1]

    # Roll a 4 in 3 universes
    position_1 += 1
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0] * 3
    wins[1] += temp_wins[1] * 3

    # Roll a 5 in 6 universes
    position_1 += 1
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0] * 6
    wins[1] += temp_wins[1] * 6

    # Roll a 6 in 7 universes
    position_1 += 1
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0] * 7
    wins[1] += temp_wins[1] * 7

    # Roll a 7 in 6 universes
    position_1 += 1
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0] * 6
    wins[1] += temp_wins[1] * 6

    # Roll an 8 in 3 universes
    position_1 += 1
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0] * 3
    wins[1] += temp_wins[1] * 3

    # Roll an 9 in 1 universe
    position_1 += 1
    if position_1 > 10:
      position_1 -= 10
    temp_total = total_1 + position_1
    temp_wins = play_game_in_universe(nextplayer, position_0, position_1, total_0, temp_total)
    wins[0] += temp_wins[0]
    wins[1] += temp_wins[1]

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
