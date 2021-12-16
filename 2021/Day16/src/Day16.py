import argparse

class Packet:
  '''Represents a Buoyancy Interchange Transmission System (BITS) Packet.'''

  def __init__(self, bits, index=0, skip_padding=False):
    initial_index = index
    (self.version, index) = Packet._fetch_value_from_bits(bits, index, 3)
    (self.type, index) = Packet._fetch_value_from_bits(bits, index, 3)
    self.value = None
    self.subpackets = []
    if self.type == 4:
      (self.value, index) = Packet._get_literal(bits, index)
    else:
      length_type = bits[index]
      index += 1
      if length_type == 0:
        # Next 15 bits are the bit length
        (bit_length, index) = Packet._fetch_value_from_bits(bits, index, 15)
        current_index = index
        while index - current_index < bit_length:
          new_packet = Packet(bits, index, skip_padding=True)
          self.subpackets.append(new_packet)
          index += new_packet.packet_size
      else:
        # Next 11 bits are the number of sub-packets
        (subpacket_count, index) = Packet._fetch_value_from_bits(bits, index, 11)
        for _ in range(subpacket_count):
          new_packet = Packet(bits, index, skip_padding=True)
          self.subpackets.append(new_packet)
          index += new_packet.packet_size

    # Get through padding.
    if not skip_padding:
      while (index - initial_index) % 4 != 0:
        index += 1

    self.packet_size = index - initial_index

  @staticmethod
  def _get_literal(bits, index):
    bits_in_value = []
    keep_parsing = True
    while keep_parsing:
      bits_in_value.extend(bits[index + 1 : index + 5])

      if bits[index] == 0:
        keep_parsing = False

      index += 5

    return (Packet._convert_bits_to_decimal(bits_in_value), index)

  @staticmethod
  def _convert_bits_to_decimal(bits):
    return int(''.join(map(str, bits)), 2)

  @staticmethod
  def _fetch_value_from_bits(bits, index, size):
    bits_to_convert = bits[index : index + size]
    decimal = Packet._convert_bits_to_decimal(bits_to_convert)
    return (decimal, index + size)

  def sum_version_numbers(self):
    '''Gets the sum of this packet's version number, and all subpackets.'''
    total = self.version
    for p in self.subpackets:
      total += p.sum_version_numbers()

    return total

  def _sum(self):
    value = 0
    for p in self.subpackets:
      value += p.get_value()

    return value

  def _product(self):
    value = 1
    for p in self.subpackets:
      value *= p.get_value()

    return value

  def _min(self):
    value = None
    for p in self.subpackets:
      p_value = p.get_value()
      if value is None or p_value < value:
        value = p_value

    return value

  def _max(self):
    value = None
    for p in self.subpackets:
      p_value = p.get_value()
      if value is None or p_value > value:
        value = p_value

    return value

  def _gt(self):
    if self.subpackets[0].get_value() > self.subpackets[1].get_value():
      return 1

    return 0

  def _lt(self):
    if self.subpackets[0].get_value() < self.subpackets[1].get_value():
      return 1

    return 0

  def _eq(self):
    if self.subpackets[0].get_value() == self.subpackets[1].get_value():
      return 1

    return 0

  def get_value(self):
    '''Calculates the value of this packet.'''
    value = -1
    if self.type == 0:
      value = self._sum()
    elif self.type == 1:
      value = self._product()
    elif self.type == 2:
      value = self._min()
    elif self.type == 3:
      value = self._max()
    elif self.type == 4:
      value = self.value
    elif self.type == 5:
      value = self._gt()
    elif self.type == 6:
      value = self._lt()
    elif self.type == 7:
      value = self._eq()

    return value

def convert_hex_to_bits(hex_string):
  '''Converts a hexadecimal string into a series of bits.'''
  bits = []
  hex_map = {
    '0': [0, 0, 0, 0],
    '1': [0, 0, 0, 1],
    '2': [0, 0, 1, 0],
    '3': [0, 0, 1, 1],
    '4': [0, 1, 0, 0],
    '5': [0, 1, 0, 1],
    '6': [0, 1, 1, 0],
    '7': [0, 1, 1, 1],
    '8': [1, 0, 0, 0],
    '9': [1, 0, 0, 1],
    'A': [1, 0, 1, 0],
    'B': [1, 0, 1, 1],
    'C': [1, 1, 0, 0],
    'D': [1, 1, 0, 1],
    'E': [1, 1, 1, 0],
    'F': [1, 1, 1, 1]
  }
  for character in hex_string:
    bits.extend(hex_map[character])
  return bits

def get_packet(filename):
  '''Gets the outermost packet from the file.'''
  with open(filename, 'r') as fh:
    hex_string = fh.read().strip()

  bits = convert_hex_to_bits(hex_string)
  return Packet(bits)

def main():
  '''The return of IntCode.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample1.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  packet = get_packet(args.filename)
  if args.part == 1:
    print(packet.sum_version_numbers())
  elif args.part == 2:
    print(packet.get_value())

if __name__ == "__main__":
  main()
