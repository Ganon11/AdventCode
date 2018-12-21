import hashlib
import sys

def check_for_prefix(secret_key, prefix, hint=1):
  prefix_len = len(prefix)
  i = hint
  while True:
    hashee = (secret_key + str(i)).encode('utf-8')
    h = hashlib.md5(hashee).hexdigest()
    if h[0:prefix_len] == prefix:
      print("Found i = %d, hex = %s" % (i, h))
      break
    i = i + 1

  return i

def main():
  secret_key = 'yzbqklnj'
  if len(sys.argv) > 1:
    secret_key = sys.argv[1]

  hint = check_for_prefix(secret_key, '00000')
  check_for_prefix(secret_key, '000000', hint)

if __name__ == "__main__":
  main()