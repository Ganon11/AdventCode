import hashlib

def main():
  secret_key = 'yzbqklnj'
  i = 1
  while True:
    hashee = (secret_key + str(i)).encode('utf-8')
    h = hashlib.md5(hashee).hexdigest()
    if h[0:5] == '00000':
      print("Found one: i = %d, hex = %s" % (i, h))
      break
    i = i + 1

if __name__ == "__main__":
  main()