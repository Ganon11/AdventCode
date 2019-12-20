import argparse
from collections import defaultdict
import re

class ChemicalAgent: # pylint: disable=C0115
  CHEMICAL_PATTERN = re.compile('(\\d+) (\\w+)')

  def __init__(self, name, amount):
    self.name = name
    self.amount = amount

  @classmethod
  def from_text(cls, string): # pylint: disable=C0116
    match = ChemicalAgent.CHEMICAL_PATTERN.match(string)
    if match is None:
      return None
    return cls(match.group(2), int(match.group(1)))

  def is_in_collection(self, collection): # pylint: disable=C0116
    if self.name not in collection or collection[self.name] < self.amount:
      return False
    return True

  def __str__(self):
    return f'{self.amount} {self.name}'

  def __repr__(self):
    return str(self)

class Reaction: # pylint: disable=C0115
  REACTION_PATTERN = re.compile('(?P<reactants>.+) => (?P<products>.+)')

  def __init__(self, reactants, product):
    self.reactants = reactants
    self.product = product

  @classmethod
  def from_text(cls, line): # pylint: disable=C0116
    match = Reaction.REACTION_PATTERN.match(line)
    if match is None:
      return None

    reactants = list(map(ChemicalAgent.from_text, match.group(1).split(', ')))
    product = ChemicalAgent.from_text(match.group(2))

    return cls(reactants, product)

  def can_occur(self, collection): # pylint: disable=C0116
    for reactant in self.reactants:
      if not reactant.is_in_collection(collection):
        return False
    return True

  def can_be_reversed(self, collection): # pylint: disable=C0116
    if not self.product.is_in_collection(collection):
      return False
    return True

  def __str__(self):
    return f'{", ".join(map(str, self.reactants))} => {str(self.product)}'

  def __repr__(self):
    return str(self)

def get_reactions(filename): # pylint: disable=C0116
  lines = None
  with open(filename, 'r') as file:
    lines = file.readlines()

  return [Reaction.from_text(line.rstrip()) for line in lines]

def create(target, reactions, collection=None): # pylint: disable=C0116
  if collection is None:
    collection = defaultdict(lambda: 0)

  #print(f'Want to make {target} with {collection}')
  # Just make the ore
  if target == 'ORE':
    collection['ORE'] += 1
    return (1, collection)

  # Find the reaction that makes target
  needed_reaction = None
  for reaction in reactions:
    if reaction.product.name == target:
      needed_reaction = reaction
      break

  if needed_reaction is None:
    raise Exception(f'No reaction found to make {target}')

  ore_required = 0
  # Ensure there are enough materials to feed the reaction
  for reactant in needed_reaction.reactants:
    while collection[reactant.name] < reactant.amount:
      (ore, collection) = create(reactant.name, reactions, collection)
      ore_required += ore
    collection[reactant.name] -= reactant.amount

  collection[target] += needed_reaction.product.amount

  return (ore_required, collection)

def find_ore_from_fuel(reactions): # pylint: disable=C0116
  # DFS?
  (ore_required, _) = create('FUEL', reactions)
  return ore_required

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  reactions = get_reactions(args.filename)
  if args.part == 1:
    ore_needed = find_ore_from_fuel(reactions)
    print(f'{ore_needed} ORE needed for 1 FUEL')

if __name__ == "__main__":
  main()
