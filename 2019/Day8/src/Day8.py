import argparse

def get_layers(values, height, width):
  layers = list()
  layer_length = height * width
  for row in range(0, height):
    layers.append(list())
    for col in range(0, width):
      layers[row].append(list())

  for index in range(0, len(values)):
    row = (index % layer_length) // width
    col = (index % layer_length) % width
    layers[row][col].append(values[index])

  return layers

def count_value_in_layer(layers, layer, value):
  total = 0
  for row in range(0, len(layers)):
    for col in range(0, len(layers[row])):
      if layers[row][col][layer] == value:
        total += 1
  return total

def layer_with_min_value_count(layers, value):
  min_count = len(layers) * len(layers[0])
  min_layer = 0
  for layer in range(0, len(layers[0][0])):
    count = count_value_in_layer(layers, layer, value)
    if count < min_count:
      min_count = count
      min_layer = layer

  return min_layer

def get_pixel_value(layers, row, col):
  for pixel in layers[row][col]:
    if pixel == 0:
      return 0
    if pixel == 1:
      return 1

def get_image(layers):
  image = list()
  for row in range(0, len(layers)):
    image.append(list())
    for col in range(0, len(layers[row])):
      image[row].append(get_pixel_value(layers, row, col))

  return image

def print_image(image):
  for row in range(0, len(image)):
    for col in range(0, len(image[row])):
      if image[row][col] == 1:
        print('*', end='')
      else:
        print(' ', end='')
    print('')

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  parser.add_argument('-he', '--height', type=int)
  parser.add_argument('-w', '--width', type=int)
  args = parser.parse_args()

  values = list()
  with open(args.filename, 'r') as file:
    values.extend([int(x) for x in file.readline()])

  layers = get_layers(values, args.height, args.width)
  if args.part == 1:
    checksum_layer = layer_with_min_value_count(layers, 0)
    print(count_value_in_layer(layers, checksum_layer, 1) * count_value_in_layer(layers, checksum_layer, 2))
  elif args.part == 2:
    print_image(get_image(layers))

if __name__ == "__main__":
  main()
