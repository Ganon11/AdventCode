#pragma once

#include <map>
#include <memory>

struct MarbleListNode;

typedef std::shared_ptr<MarbleListNode> MarbleListNodePtr;

struct MarbleListNode
{
  MarbleListNode(const unsigned int points);

  unsigned int m_points;
  MarbleListNodePtr next;
  MarbleListNodePtr previous;
};

class MarbleList
{
public:
  MarbleList(const unsigned int players, const unsigned int max_marble);

  unsigned int play_game();

private:
  MarbleListNodePtr add_marble(MarbleListNodePtr& left, const unsigned int marble);
  unsigned int remove_marble(MarbleListNodePtr& marble);

  unsigned int m_players;
  unsigned int m_max_marble;
  MarbleListNodePtr m_current;
  std::map<unsigned int, unsigned int> m_player_scores;
};
