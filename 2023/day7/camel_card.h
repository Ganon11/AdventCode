#pragma once

#include <map>
#include <string>
#include <vector>

enum CamelCardLabel
{
  NONE  = 0,
  TWO   = 2,
  THREE = 3,
  FOUR  = 4,
  FIVE  = 5,
  SIX   = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE  = 9,
  TEN   = 10,
  JACK  = 11,
  QUEEN = 12,
  KING  = 13,
  ACE   = 14
};

enum HandType
{
  HIGH_CARD = 0,
  ONE_PAIR = 1,
  TWO_PAIR = 2,
  THREE_OF_A_KIND = 3,
  FULL_HOUSE = 4,
  FOUR_OF_A_KIND = 5,
  FIVE_OF_A_KIND = 6
};

class CamelCardHand
{
public:
  CamelCardHand() = delete;
  CamelCardHand(const std::string& line, const bool jokers = false);

  unsigned short bid() const;
  std::string hand() const;
  std::string type() const;

  bool operator<(const CamelCardHand& other) const;

private:
  std::vector<CamelCardLabel> m_hand;
  std::map<CamelCardLabel, unsigned short> m_hand_counts;
  HandType m_hand_type;
  unsigned short m_bid;
  bool m_jokers;
};
