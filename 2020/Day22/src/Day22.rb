#!/usr/bin/ruby

require 'optparse'
require 'set'

class Player
  attr_reader :id
  attr_reader :deck

  def initialize(lines)
    player_name = lines[0]
    data = player_name.match(/Player (?<id>\d+):/)
    if data
      @id = data[:id].to_i
    else
      puts "Bad parse"
    end

    @deck = []
    lines.drop(1).each do |line|
      @deck.push(line.to_i)
    end
  end

  def draw
    @deck.shift
  end

  def deck_size
    @deck.length
  end

  def lost?
    return @deck.empty?
  end

  def add_cards(card1, card2)
    @deck.push(card1)
    @deck.push(card2)
  end

  def keep_cards(n)
    @deck = @deck[0..n-1]
  end

  def calculate_score
    size = @deck.length
    sum = 0
    @deck.each_with_index do |card, i|
      sum += (card * (size - i))
    end

    return sum
  end

  def to_s
    str = "Player #{@id}\n"
    @deck.each do |card|
      str += "#{card}\n"
    end

    return str
  end

  def serialize
    "(#{@id}:#{@deck.join(",")})"
  end
end

def play_round(player1, player2)
  card1 = player1.draw
  card2 = player2.draw

  if card1 > card2
    player1.add_cards(card1, card2)
  else
    player2.add_cards(card2, card1)
  end
end

def play_game(player1, player2)
  play_round(player1, player2) while (not player1.lost? and not player2.lost?)

  if player1.lost?
    puts player2
    return player2.calculate_score
  else
    puts player1
    return player1.calculate_score
  end
end

$NEW_GAME_NUMBER = 1

def play_recursive_round(player1, player2, round, game)
  #puts "-- Round #{round} (Game #{game}) --"
  #puts "Player 1's deck: #{player1.deck.join(", ")}"
  #puts "Player 2's deck: #{player2.deck.join(", ")}"
  card1 = player1.draw
  card2 = player2.draw

  #puts "Player 1 plays: #{card1}"
  #puts "Player 2 plays: #{card2}"

  if (player1.deck_size >= card1) and (player2.deck_size >= card2)
    #puts "Playing a sub-game to determine the winner..."
    cloned_player1 = player1.clone
    cloned_player1.keep_cards(card1)
    cloned_player2 = player2.clone
    cloned_player2.keep_cards(card2)
    winner = play_recursive_game(cloned_player1, cloned_player2, $NEW_GAME_NUMBER)
    #puts "...anyway, back to game #{game}."
    if winner == 1
      #puts "Player 1 wins round #{round} of game #{game}!"
      player1.add_cards(card1, card2)
    else
      #puts "Player 2 wins round #{round} of game #{game}!"
      player2.add_cards(card2, card1)
    end
  else
    if card1 > card2
      #puts "Player 1 wins round #{round} of game #{game}!"
      player1.add_cards(card1, card2)
    else
      #puts "Player 2 wins round #{round} of game #{game}!"
      player2.add_cards(card2, card1)
    end
  end
end

def play_recursive_game(player1, player2, game=1)
  #puts "=== Game #{game} ==="
  $NEW_GAME_NUMBER += 1
  seen = Set[]
  current_state = "#{player1.serialize},#{player2.serialize}"
  round = 1
  while true
    if seen.include?(current_state)
      #puts "The winner of game #{game} is player 1!"
      return 1
    elsif player1.lost?
      #puts "The winner of game #{game} is player 2!"
      return 2
    elsif player2.lost?
      #puts "The winner of game #{game} is player 1!"
      return 1
    end

    seen.add(current_state)
    play_recursive_round(player1, player2, round, game)
    current_state = "#{player1.serialize},#{player2.serialize}"
    round += 1
  end
end

options = {
  :part => 1,
  :filename => "../input/sample.txt"
}

OptionParser.new do |opts|
  opts.banner = "Usage: Day22.rb [options]"
  opts.on("-p PART", "--part=PART", Integer, "Part 1 or 2")
  opts.on("-f FILENAME", "--filename=FILENAME", String, "Which filename to use?")
end.parse!(into: options)

first, second = File.read(options[:filename]).split("\n\n").map{ |data| Player::new(data.split("\n"))}

if options[:part] == 1
  puts play_game(first, second)
elsif options[:part] == 2
  if play_recursive_game(first, second) == 1
    puts first.calculate_score
  else
    puts second.calculate_score
  end
end
