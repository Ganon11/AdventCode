import sys

def increment_string(value):
  reversed_value = ''.join(reversed(value))
  reversed_len = len(reversed_value)
  for index in range(reversed_len - 1):
    ch = reversed_value[index]
    next_char = ''
    if ch == 'z':
      next_char = 'a'
    else:
      next_char = chr(ord(reversed_value[index]) + 1)

    if index == 0:
      reversed_value = next_char + reversed_value[index + 1:]
    elif index == (reversed_len - 1):
      reversed_value = reversed_value[0:index] + next_char
    else:
      reversed_value = reversed_value[0:index] + next_char + reversed_value[index + 1:]

    if ch != 'z':
      break

  return ''.join(reversed(reversed_value))

def password_contains_straight(password):
  for index in range(len(password) - 2):
    a = ord(password[index])
    b = ord(password[index + 1])
    c = ord(password[index + 2])
    if (a + 1) == b and (a + 2) == c:
      return True

  return False

def password_contains_i_o_l(password):
  if ('i' in password) or ('o' in password) or ('l' in password):
    return True;
  return False

def password_contains_two_pairs(password):
  last_index = -1
  for index in range(len(password) - 1):
    if password[index] == password[index + 1]:
      if last_index == -1:
        last_index = index
      elif (index - 1) == last_index:
        next
      else:
        return True

  return False

def password_is_ok(password):
  if not password_contains_straight(password):
    return False

  if password_contains_i_o_l(password):
    return False

  if not password_contains_two_pairs(password):
    return False

  return True

def main(old_password):
  new_password = increment_string(old_password)

  while not password_is_ok(new_password):
    new_password = increment_string(new_password)

  print('Santa\'s next password is "%s"' % new_password)

if __name__ == "__main__":
  old_password = 'abcdefgh'
  if len(sys.argv) > 1:
    old_password = sys.argv[1]

  #print(password_contains_two_pairs(old_password))
  main(old_password)