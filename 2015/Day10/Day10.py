def expand(num):
  print("Num: %s" % num)
  out = ""
  old = num[0]
  print("Old: %s" % old)
  count = 1

  while len(num) > 1:
    num = num[1:]
    print("\tnum: %s" % num)
    char = num[0]
    print("\tchar: %s" % char)
    print("\told: %s" % old)
    if old == char:
      count = count + 1
    else:
      out = "%s%s%s" % (out, str(count), old)
      count = 1
    old = char

  out = "%s%s%s" % (out, str(count), old)

  return out

def main():
  num = "1"
  for i in range(0, 2):
    num = expand(num)
    print("Result: %s" % num)

if __name__ == "__main__":
  main()