import sys

def main():
  filename = 'Sample.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]

if __name__ == "__main__":
  main()
