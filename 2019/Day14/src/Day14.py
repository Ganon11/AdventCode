import argparse
from collections import defaultdict
import math
import re

import my_collections

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
  reactions = dict() # Key is product name, value is Reaction
  lines = None
  with open(filename, 'r') as file:
    lines = file.readlines()

  for line in lines:
    reaction = Reaction.from_text(line.rstrip())
    reactions[reaction.product.name] = reaction

  return reactions

def make_fuel_queue_based(amount, recipebook): # pylint: disable=C0116
  supply = defaultdict(int)
  orders = my_collections.Queue()
  orders.put(ChemicalAgent('FUEL', amount))
  ore_needed = 0

  while not orders.empty():
    order = orders.get()
    # If making ore, just make it. We have "infinite" supply.
    if order.name == 'ORE':
      ore_needed += order.amount
    # If we have enough in supply to fulfill the order, just use it.
    elif order.amount <= supply[order.name]:
      supply[order.name] -= order.amount
    # We have to make more to fulfill the order
    else:
      amount_needed = order.amount - supply[order.name]
      reaction = recipebook[order.name]
      batches = math.ceil(amount_needed / reaction.product.amount)
      for reactant in reaction.reactants:
        orders.put(ChemicalAgent(reactant.name, reactant.amount * batches))
      leftover = batches * reaction.product.amount - amount_needed
      supply[order.name] = leftover
  return ore_needed

def find_fuel_from_ore(recipebook): # pylint: disable=C0116
  upper_bound = None
  cost_for_one_fuel = make_fuel_queue_based(1, recipebook)
  ore_capacity = 1_000_000_000_000
  lower_bound = ore_capacity // cost_for_one_fuel
  while lower_bound + 1 != upper_bound:
    if upper_bound is None:
      guess = lower_bound * 2
    else:
      guess = (upper_bound + lower_bound) // 2

    ore_required = make_fuel_queue_based(guess, recipebook)
    if ore_required > ore_capacity:
      upper_bound = guess
    else:
      lower_bound = guess

  return lower_bound

def main(): # pylint: disable=C0116
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  recipebook = get_reactions(args.filename)
  if args.part == 1:
    ore_needed = make_fuel_queue_based(1, recipebook)
    print(f'{ore_needed} ORE needed for 1 FUEL')
  elif args.part == 2:
    fuel_made = find_fuel_from_ore(recipebook)
    print(f'{fuel_made} FUEL can be made from 1000000000000 ORE')

if __name__ == "__main__":
  main()
