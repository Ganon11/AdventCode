import argparse

class Packet:
  '''Represents a Buoyancy Interchange Transmission System (BITS) Packet.'''

  def __init__(self, bits, index=0):
    '''Unpacks a packet from a series of bits.'''
    initial_index = index
    (self.version, index) = Packet._get_value_from_bits(bits, index, 3)
    (self.type, index) = Packet._get_value_from_bits(bits, index, 3)
    if self.type == 4:
      (self.value, index) = Packet._get_literal(bits, index)
      self.subpackets = []
    else:
      (self.subpackets, index) = Packet._get_subpackets(bits, index)

    self.packet_size = index - initial_index

  @staticmethod
  def _get_literal(bits, index):
    '''Extracts a literal value from a series of bits.'''
    bits_in_value = []
    keep_parsing = True
    while keep_parsing:
      bits_in_value.extend(bits[index + 1 : index + 5])

      if bits[index] == 0:
        keep_parsing = False

      index += 5

    return (Packet._convert_bits_to_decimal(bits_in_value), index)

  @staticmethod
  def _get_subpackets(bits, index):
    '''Extracts a series of packets from a series of bits.'''
    length_type = bits[index]
    index += 1
    if length_type == 0:
      # Next 15 bits are the bit length
      return Packet._get_subpackets_by_bit_length(bits, index)

    return Packet._get_subpackets_by_count(bits, index)

  @staticmethod
  def _get_subpackets_by_bit_length(bits, index):
    '''Converts a specific number of bits into subpackets.'''
    subpackets = []
    (bit_length, index) = Packet._get_value_from_bits(bits, index, 15)
    current_index = index
    while index - current_index < bit_length:
      new_packet = Packet(bits, index)
      subpackets.append(new_packet)
      index += new_packet.packet_size

    return (subpackets, index)

  @staticmethod
  def _get_subpackets_by_count(bits, index):
    '''Converts bits into a specific number of subpackets.'''
    subpackets = []
    (subpacket_count, index) = Packet._get_value_from_bits(bits, index, 11)
    for _ in range(subpacket_count):
      new_packet = Packet(bits, index)
      subpackets.append(new_packet)
      index += new_packet.packet_size

    return (subpackets, index)

  @staticmethod
  def _convert_bits_to_decimal(bits):
    '''Converts bits into a single decimal value.'''
    return int(''.join(map(str, bits)), 2)

  @staticmethod
  def _get_value_from_bits(bits, index, size):
    '''Gets a numeric value from a given subsection of bits.'''
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
    '''Gets the sum of all subpackets.'''
    value = 0
    for p in self.subpackets:
      value += p.get_value()

    return value

  def _product(self):
    '''Gets the product of all subpackets.'''
    value = 1
    for p in self.subpackets:
      value *= p.get_value()

    return value

  def _min(self):
    '''Gets the minimum value of the subpackets.'''
    value = None
    for p in self.subpackets:
      p_value = p.get_value()
      if value is None or p_value < value:
        value = p_value

    return value

  def _max(self):
    '''Gets the maximum value of the subpackets.'''
    value = None
    for p in self.subpackets:
      p_value = p.get_value()
      if value is None or p_value > value:
        value = p_value

    return value

  def _literal(self):
    '''Gets the literal value.'''
    return self.value

  def _gt(self):
    '''
    Returns 1 if the first subpacket's value is greater than the second subpacket's value.

    Otherwise, returns 0.
    '''
    if self.subpackets[0].get_value() > self.subpackets[1].get_value():
      return 1

    return 0

  def _lt(self):
    '''
    Returns 1 if the first subpacket's value is less than the second subpacket's value.

    Otherwise, returns 0.
    '''
    if self.subpackets[0].get_value() < self.subpackets[1].get_value():
      return 1

    return 0

  def _eq(self):
    '''
    Returns 1 if the first subpacket's value is equal to the second subpacket's value.

    Otherwise, returns 0.
    '''
    if self.subpackets[0].get_value() == self.subpackets[1].get_value():
      return 1

    return 0

  _VALUE_MAP = {
    0: _sum,
    1: _product,
    2: _min,
    3: _max,
    4: _literal,
    5: _gt,
    6: _lt,
    7: _eq
  }

  def get_value(self):
    '''Calculates the value of this packet.'''
    return Packet._VALUE_MAP[self.type](self)

def convert_hex_to_bits(hex_string):
  '''Converts a hexadecimal string into a series of bits.'''
  bits = []
  for character in hex_string:
    bits.extend(convert_hex_to_bits.HEX_MAP[character])
  return bits

convert_hex_to_bits.HEX_MAP = {
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

def get_packet(filename):
  '''Gets the outermost packet from the file.'''
  with open(filename, 'r') as fh:
    hex_string = fh.read().strip()

  bits = convert_hex_to_bits(hex_string)
  return Packet(bits)

def main():
  '''The return of IntCode.'''
  parser = argparse.ArgumentParser()
  parser.add_argument('-f', '--filename', default='../input/sample.txt')
  parser.add_argument('-p', '--part', choices=[1, 2], default=1, type=int)
  args = parser.parse_args()

  packet = get_packet(args.filename)
  if args.part == 1:
    print(packet.sum_version_numbers())
  elif args.part == 2:
    print(packet.get_value())

if __name__ == "__main__":
  main()
