#include "camel_card.h"

#include <algorithm>
#include <string>
#include <vector>

#include "input_handler.h"

namespace
{
CamelCardLabel parse(const char c)
{
  switch (c)
  {
    case '2': return CamelCardLabel::TWO;
    case '3': return CamelCardLabel::THREE;
    case '4': return CamelCardLabel::FOUR;
    case '5': return CamelCardLabel::FIVE;
    case '6': return CamelCardLabel::SIX;
    case '7': return CamelCardLabel::SEVEN;
    case '8': return CamelCardLabel::EIGHT;
    case '9': return CamelCardLabel::NINE;
    case 'T': return CamelCardLabel::TEN;
    case 'J': return CamelCardLabel::JACK;
    case 'Q': return CamelCardLabel::QUEEN;
    case 'K': return CamelCardLabel::KING;
    case 'A': return CamelCardLabel::ACE;
    default: return CamelCardLabel::NONE;
  }
}

char encode(const CamelCardLabel c)
{
  switch (c)
  {
    case CamelCardLabel::TWO: return '2';
    case CamelCardLabel::THREE: return '3';
    case CamelCardLabel::FOUR: return '4';
    case CamelCardLabel::FIVE: return '5';
    case CamelCardLabel::SIX: return '6';
    case CamelCardLabel::SEVEN: return '7';
    case CamelCardLabel::EIGHT: return '8';
    case CamelCardLabel::NINE: return '9';
    case CamelCardLabel::TEN: return 'T';
    case CamelCardLabel::JACK: return 'J';
    case CamelCardLabel::QUEEN: return 'Q';
    case CamelCardLabel::KING: return 'K';
    case CamelCardLabel::ACE: return 'A';
    default: return '-';
  }
}

HandType determine_hand_type_no_jokers(const std::map<CamelCardLabel, unsigned short>& hand_counts)
{
  if (std::any_of(hand_counts.begin(), hand_counts.end(), [](const auto& p){ return p.second == 5; }))
  {
    return HandType::FIVE_OF_A_KIND;
  }

  if (std::any_of(hand_counts.begin(), hand_counts.end(), [](const auto& p){ return p.second == 4; }))
  {
    return HandType::FOUR_OF_A_KIND;
  }

  if (std::any_of(hand_counts.begin(), hand_counts.end(), [](const auto& p){ return p.second == 3; }) &&
    std::any_of(hand_counts.begin(), hand_counts.end(), [](const auto& p){ return p.second == 2; }))
  {
    return HandType::FULL_HOUSE;
  }

  if (std::any_of(hand_counts.begin(), hand_counts.end(), [](const auto& p){ return p.second == 3; }))
  {
    return HandType::THREE_OF_A_KIND;
  }

  bool pairFound = false;
  for (const auto& card : hand_counts)
  {
    if (card.second == 2)
    {
      if (pairFound)
      {
        return HandType::TWO_PAIR;
      }
      else
      {
        pairFound = true;
      }
    }
  }

  if (pairFound)
  {
    return HandType::ONE_PAIR;
  }

  return HandType::HIGH_CARD;
}

HandType determine_hand_type_with_jokers(const std::map<CamelCardLabel, unsigned short>& hand_counts, unsigned short joker_count = 0)
{
  if (joker_count == 5 || joker_count == 4)
  {
    return HandType::FIVE_OF_A_KIND;
  }

  if (std::any_of(hand_counts.begin(), hand_counts.end(), [&joker_count](const auto& p){ return p.first != CamelCardLabel::JACK && p.second + joker_count == 4; }))
  {
    return HandType::FOUR_OF_A_KIND;
  }

  if (std::any_of(hand_counts.begin(), hand_counts.end(), [&joker_count](const auto& p){ return p.first != CamelCardLabel::JACK && p.second + joker_count == 3; }) &&
    std::any_of(hand_counts.begin(), hand_counts.end(), [](const auto& p){ return p.first != CamelCardLabel::JACK && p.second == 2; }))
  {
    return HandType::FULL_HOUSE;
  }

  return HandType::HIGH_CARD;
}

HandType determine_hand_type(const std::map<CamelCardLabel, unsigned short>& hand_counts, const bool jokers)
{
  if (!jokers)
  {
    return determine_hand_type_no_jokers(hand_counts);
  }
  else
  {
    unsigned short joker_count{0};
    const auto& joker_iterator{ hand_counts.find(CamelCardLabel::JACK) };
    if (joker_iterator != hand_counts.end())
    {
      joker_count = joker_iterator->second;
    }

    return determine_hand_type_with_jokers(hand_counts, joker_count);
  }
}
}

CamelCardHand::CamelCardHand(const std::string& line, const bool jokers)
{
  std::vector<std::string> tokens = advent_of_code::tokenize(line, ' ');
  for (const char ch : tokens[0])
  {
    CamelCardLabel label{ ::parse(ch) };
    m_hand.push_back(label);
    if (!m_hand_counts.contains(label))
    {
      m_hand_counts[label] = 1;
    }
    else
    {
      m_hand_counts[label]++;
    }
  }

  m_bid = std::stoi(tokens[1]);
  m_hand_type = determine_hand_type(m_hand_counts, jokers);
}

unsigned short CamelCardHand::bid() const
{
  return m_bid;
}

std::string CamelCardHand::hand() const
{
  std::string hand;
  for (const CamelCardLabel label : m_hand)
  {
    hand += encode(label);
  }

  return hand;
}

std::string CamelCardHand::type() const
{
  switch (m_hand_type)
  {
    case HIGH_CARD: return "High Card";
    case ONE_PAIR: return "One Pair";
    case TWO_PAIR: return "Two Pair";
    case THREE_OF_A_KIND: return "Three of a Kind";
    case FULL_HOUSE: return "Full House";
    case FOUR_OF_A_KIND: return "Four of a Kind";
    case FIVE_OF_A_KIND: return "Five of a Kind";
    default: return "I don't know!";
  }
}

bool CamelCardHand::operator<(const CamelCardHand& other) const
{
  if (m_hand_type < other.m_hand_type)
  {
    return true;
  }

  if (m_hand_type == other.m_hand_type)
  {
    for (size_t index = 0; index < m_hand.size(); ++index)
    {
      if (m_hand[index] < other.m_hand[index])
      {
        return true;
      }
      else if (m_hand[index] > other.m_hand[index])
      {
        return false;
      }
    }
  }

  return false;
}

bool CamelCardHand::operator>(const CamelCardHand& other) const
{
  if (m_hand_type > other.m_hand_type)
  {
    return true;
  }

  if (m_hand_type == other.m_hand_type)
  {
    for (size_t index = 0; index < m_hand.size(); ++index)
    {
      if (m_hand[index] > other.m_hand[index])
      {
        return true;
      }
      else if (m_hand[index] < other.m_hand[index])
      {
        return false;
      }
    }
  }

  return false;
}

