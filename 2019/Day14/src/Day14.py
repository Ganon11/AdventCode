import argparse
import math
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

def satisfy_need(reactions, element, amount_needed, collection=None): # pylint: disable=C0116,R0912
  if collection is None:
    collection = dict()

  # We still need more - find the reaction to make this element
  if element == 'ORE':
    # We can always just make ore.
    print(f'Need is ORE, I\'m makin\' it.')
    if element in collection:
      collection[element] += amount_needed
    else:
      collection[element] = amount_needed
    print(f'After adding ORE, collection is {collection}')
    return collection

  print(f'Satisfying need {element} {amount_needed} with collection {collection}')

  # We may already have some of this element in our collection - if so, we need less extra
  extra = amount_needed
  if element in collection:
    if collection[element] >= amount_needed:
      # We have what we need already
      collection[element] -= amount_needed
      return collection

    extra -= collection[element]

  print(f'After accounting for leftovers, I still need {extra} more {element}')

  needed_reaction = None
  for reaction in reactions:
    if reaction.product.name == element:
      needed_reaction = reaction
      break

  if needed_reaction is None:
    raise Exception(f'Cannot satisfy need for {amount_needed} {element}')

  # Use this reaction to satisfy the immediate need
  multiplier = math.ceil(extra / needed_reaction.product.amount)
  print(f'I will perform {needed_reaction} {multiplier} times to satisfy need.')
  for reactant in needed_reaction.reactants:
    collection = satisfy_need(reactions, reactant.name, reactant.amount * multiplier, collection)
    # if reactant.name in collection:
    #   collection[reactant.name] += multiplier * reactant.amount
    # else:
    #   collection[reactant.name] = multiplier * reactant.amount
    #needs[reactant.name] = multiplier * reactant.amount

  print(f'After performing reaction, my collection is {collection}')
  # Pick another need to satisfy
  # for need in needs:
  #   #collection[need] -= needs[need]
  #   collection = satisfy_need(reactions, need, needs[need], collection)

  return collection

def find_ore_from_fuel(reactions): # pylint: disable=C0116
  # DFS?
  collection = satisfy_need(reactions, 'FUEL', 1)
  #print(collection)

  #collection = satisfy_need(reactions, 'A', 20)

  if 'ORE' not in collection:
    return -1

  return collection['ORE']

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
