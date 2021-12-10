import argparse
from collections import deque

CHARACTER_MAP = {
  '(': ')',
  '[': ']',
  '{': '}',
  '<': '>'
}

SYNTAX_ERROR_SCORE_MAP = {
  ')': 3,
  ']': 57,
  '}': 1197,
  '>': 25137
}

AUTOCOMPLETE_SCORE_MAP = {
  ')': 1,
  ']': 2,
  '}': 3,
  '>': 4
}

def calculate_autocomplete_score(autocomplete):
  '''Calculates the score of an autocomplete string'''
  score = 0
  for c in autocomplete:
    score *= 5
    score += AUTOCOMPLETE_SCORE_MAP[c]
  return score

def process_chunks(filename):
  '''
  Processes the chunks in the file to determine the syntax error score and autocomplete scores.
  '''
  lines = []
  with open(filename, 'r') as fh:
    lines = fh.readlines()

  syntax_error_score = 0
  autocomplete_scores = []
  for line in lines:
    stack = deque()
    corrupt = False
    for c in line.strip():
      if c in CHARACTER_MAP:
        stack.append(c)
        continue

      if c == CHARACTER_MAP[stack[-1]]:
        stack.pop()
        continue

      corrupt = True
      syntax_error_score += SYNTAX_ERROR_SCORE_MAP[c]
      break

    if not corrupt and len(stack):
      autocomplete = ''
      while len(stack):
        autocomplete += CHARACTER_MAP[stack.pop()]
      autocomplete_scores.append(calculate_autocomplete_score(autocomplete))


  return (syntax_error_score, autocomplete_scores)

def main():
  '''Feeling chunky.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  (syntax_error_score, autocomplete_scores) = process_chunks(args.filename)
  if args.part == 1:
    print(f'Syntax error score: {syntax_error_score}')
  else:
    autocomplete_scores.sort()
    winning_score = autocomplete_scores[len(autocomplete_scores) // 2]
    print(f'Winning autocomplete score: {winning_score}')

if __name__ == "__main__":
  main()
