import hashlib

def main():
  secret_key = 'ckczppom'
  i = 1
  while True:
    hashee = (secret_key + str(i)).encode('utf-8')
    m = hashlib.md5(hashee)
    h = m.hexdigest()
    check = h[0:6]
    if check == '000000':
      print("Found one: i = %d, hex = %s" % (i, h))
      break
    i = i + 1

if __name__ == "__main__":
  main()