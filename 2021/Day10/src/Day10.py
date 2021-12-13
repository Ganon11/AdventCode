import argparse
from collections import deque

def calculate_autocomplete_score(autocomplete):
  '''Calculates the score of an autocomplete string'''
  score = 0
  for c in autocomplete:
    score *= 5
    score += calculate_autocomplete_score.AUTOCOMPLETE_SCORE_MAP[c]
  return score

calculate_autocomplete_score.AUTOCOMPLETE_SCORE_MAP = {
  ')': 1,
  ']': 2,
  '}': 3,
  '>': 4
}

def process_chunks(filename, debug):
  '''
  Processes the chunks in the file to determine the syntax error score and autocomplete scores.
  '''
  lines = []
  with open(filename, 'r') as fh:
    lines = list(map(lambda line: line.strip(), fh.readlines()))

  syntax_error_score = 0
  autocomplete_scores = []
  for line in lines:
    stack = deque()
    corrupt = False
    for c in line:
      if c in process_chunks.CHARACTER_MAP:
        stack.append(c)
        continue

      if c == process_chunks.CHARACTER_MAP[stack[-1]]:
        stack.pop()
        continue

      corrupt = True
      syntax_error_score += process_chunks.SYNTAX_ERROR_SCORE_MAP[c]
      if debug:
        expected = process_chunks.CHARACTER_MAP[stack[-1]]
        print(f'{line} - Expected {expected}, but found {c} instead.')
      break

    if not corrupt and len(stack):
      autocomplete = ''
      while len(stack):
        autocomplete += process_chunks.CHARACTER_MAP[stack.pop()]
      autocomplete_scores.append(calculate_autocomplete_score(autocomplete))
      if debug:
        print(f'{line} - Complete by adding {autocomplete}')

  return (syntax_error_score, autocomplete_scores)

process_chunks.CHARACTER_MAP = {
  '(': ')',
  '[': ']',
  '{': '}',
  '<': '>'
}

process_chunks.SYNTAX_ERROR_SCORE_MAP = {
  ')': 3,
  ']': 57,
  '}': 1197,
  '>': 25137
}

def main():
  '''Feeling chunky.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-d', '--debug', dest='debug', action='store_true')
  parser.set_defaults(debug=False)
  args = parser.parse_args()

  (syntax_error_score, autocomplete_scores) = process_chunks(args.filename, args.debug)
  if args.part == 1:
    print(f'Syntax error score: {syntax_error_score}')
  else:
    autocomplete_scores.sort()
    winning_score = autocomplete_scores[len(autocomplete_scores) // 2]
    print(f'Winning autocomplete score: {winning_score}')

if __name__ == "__main__":
  main()
