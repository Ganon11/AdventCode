import argparse
import itertools
import re

class Ingredient:
  _INGREDIENT_PATTERN = re.compile('(\w+): capacity (-?\d+), durability (-?\d+), flavor (-?\d+), texture (-?\d+), calories (-?\d+)', re.IGNORECASE)
  def __init__(self, line):
    match = Ingredient._INGREDIENT_PATTERN.match(line)
    self.name = match.group(1)
    self.capacity = int(match.group(2))
    self.durability = int(match.group(3))
    self.flavor = int(match.group(4))
    self.texture = int(match.group(5))
    self.calories = int(match.group(6))

class Cookie:
  def __init__(self):
    self.ingredients = list()

  def add_ingredient(self, ingredient):
    if len(self.ingredients) == 100:
      return False

    self.ingredients.append(ingredient)
    return True

  def get_score(self):
    total_capacity = 0
    total_durability = 0
    total_flavor = 0
    total_texture = 0

    for ingredient in self.ingredients:
      total_capacity = total_capacity + ingredient.capacity
      total_durability = total_durability + ingredient.durability
      total_flavor = total_flavor + ingredient.flavor
      total_texture = total_texture + ingredient.texture

    if total_capacity < 0:
      total_capacity = 0
    if total_durability < 0:
      total_durability = 0
    if total_flavor < 0:
      total_flavor = 0
    if total_texture < 0:
      total_texture = 0

    return total_capacity * total_durability * total_flavor * total_texture

  def get_calories(self):
    total_calories = 0

    for ingredient in self.ingredients:
      total_calories = total_calories + ingredient.calories

    if total_calories < 0:
      total_calories = 0

    return total_calories

def get_ingredients(filename):
  ingredients = list()
  with open(filename, 'r') as fp:
    for line in fp.readlines():
      ingredients.append(Ingredient(line))

  return ingredients

def part_1(filename):
  ingredients = get_ingredients(filename)

  best_score = 0
  for recipe_candidate in itertools.combinations_with_replacement(ingredients, 100):
    cookie = Cookie()
    for ingredient in recipe_candidate:
      cookie.add_ingredient(ingredient)

    score = cookie.get_score()
    if score > best_score:
      best_score = score

  print("Best cookie score is: %d" % best_score)

def part_2(filename):
  ingredients = get_ingredients(filename)

  best_score = 0
  for recipe_candidate in itertools.combinations_with_replacement(ingredients, 100):
    cookie = Cookie()
    for ingredient in recipe_candidate:
      cookie.add_ingredient(ingredient)

    score = cookie.get_score()

    if 500 == cookie.get_calories() and score > best_score:
      best_score = score

  print("Best cookie score is: %d" % best_score)

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('filename')
  args = parser.parse_args()

  part_1(args.filename)
  part_2(args.filename)

if __name__ == "__main__":
  main()
