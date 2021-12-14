import argparse
from collections import defaultdict

def get_template_and_rules(filename):
  '''
  Gets the beginning template string and the polymerization rules from the file.
  '''
  template = None
  rules = dict()
  with open(filename, 'r') as fh:
    # First line is the template
    template = fh.readline().strip()

    # Second line is blank
    fh.readline()

    # All remaining lines are pair insertion rules
    line = fh.readline().strip()
    while line:
      (pair, new) = line.split(' -> ')
      rules[pair] = new
      line = fh.readline().strip()

  return (template, rules)

def convert_string_to_pairs(string):
  '''Converts a polymer string into a collection of pairs.'''
  pairs = defaultdict(int)
  for index, _ in enumerate(string):
    if index + 1 == len(string):
      break
    pairs[string[index : index + 2]] += 1

  return pairs

def expand_polymer(pairs, rules):
  '''Expands a polymer string following the given rules.'''
  new_pairs = defaultdict(int)
  for pair, count in pairs.items():
    new_pairs[pair[0] + rules[pair]] += count
    new_pairs[rules[pair] + pair[1]] += count
  return new_pairs

def score_polymer(pairs, first, last):
  '''Calculates the score of a polymer.'''
  # If I sum the letters in all pairs, all letters will be counted twice, except for the first and
  # the last letter.
  # For example, in the string ABCD, the B appears in the pairs AB and BC. The C appears in the
  # pairs BC and CD. However, A only appears in the pair AB, and D only appears in the pair CD.
  # So we'll calculate the letter frequency of the pairs, add 1 for the first and last character,
  # then divide everything by 2 to get the final counts.
  counts = defaultdict(int)
  for pair, count in pairs.items():
    counts[pair[0]] += count
    counts[pair[1]] += count

  counts[first] += 1
  counts[last] += 1

  sorted_counts = sorted(n // 2 for n in counts.values())
  return sorted_counts[-1] - sorted_counts[0]

def main():
  '''The fish have turned into polymers!'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-s', '--steps', default=10, type=int)
  args = parser.parse_args()

  (template, rules) = get_template_and_rules(args.filename)
  pairs = convert_string_to_pairs(template)
  for _ in range(args.steps):
    pairs = expand_polymer(pairs, rules)

  score = score_polymer(pairs, template[0], template[-1])
  print(f'Score of polymer after {args.steps} steps: {score}')

if __name__ == "__main__":
  main()
