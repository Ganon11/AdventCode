import os
import re

def has_three_vowels(word):
  vowels = 'aeiou'
  count = 0
  for v in vowels:
    count = count + word.count(v)

  #print("Word: %s, count: %d" % (word, count))
  return count > 2

double_letter_pattern = re.compile(r'.*([a-z])\1.*', re.IGNORECASE)
def has_double_letter(word):
  m = double_letter_pattern.match(word)
  #print("Word: %s, Match: %s" % (word, m))
  if m:
    return True
  else:
    return False

naughty_pattern = re.compile(r'.*(?:ab|cd|pq|xy).*', re.IGNORECASE)
def has_naughty_substr(word):
  m = naughty_pattern.match(word)
  #print("Word: %s, Match: %s" % (word, m))
  if m:
    return True
  else:
    return False

def determine_nice_1(words):
  naughty = 0
  nice = 0
  for w in words:
    word = w.rstrip()
    check1 = has_three_vowels(word)
    check2 = has_double_letter(word)
    check3 = has_naughty_substr(word)
    #if check1 or check2 or not check3:
      #print("Word: %s, Has Three Vowels: %s, Has Double Letter %s, Has Naughty Substring %s" % (word, check1, check2, check3))

    if check1 and check2 and not check3:
      nice = nice + 1
    else:
      naughty = naughty + 1

  print("Part 1: Naughty: %d, Nice: %d", (naughty, nice))

double_pair_pattern = re.compile(r'.*([a-z]{2}).*\1.*', re.IGNORECASE)
def has_double_pair(word):
  m = double_pair_pattern.match(word)
  #if m:
  #  print("Word: %s, Match: %s" % (word, m.group(1)))
  if m:
    return True
  else:
    return False

mirror_pattern = re.compile(r'.*([a-z])[a-z]\1.*', re.IGNORECASE)
def has_mirror_letter(word):
  m = mirror_pattern.match(word)
  #print("Word: %s, Match: %s" % (word, m))
  if m:
    return True
  else:
    return False

def determine_nice_2(words):
  naughty = 0
  nice = 0
  for w in words:
    word = w.rstrip()
    check1 = has_double_pair(word)
    check2 = has_mirror_letter(word)
    if check1 and check2:
      nice = nice + 1
    else:
      naughty = naughty + 1

  print("Part 2: Naughty: %d, Nice: %d", (naughty, nice))

def main():
  f = open('Day5Input.txt', 'r')
  words = f.readlines()
  determine_nice_1(words)
  determine_nice_2(words)

if __name__ == "__main__":
  main()