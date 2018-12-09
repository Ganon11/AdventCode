#include "pch.h"
#include "MarbleList.h"
#include <algorithm>

MarbleListNode::MarbleListNode(const unsigned int points) : m_points{ points }
{ }

MarbleList::MarbleList(const unsigned int players, const unsigned int max_marble) : m_players{ players }, m_max_marble{ max_marble }
{ }

unsigned int MarbleList::play_game()
{
   m_player_scores.clear();
   m_current = std::make_shared<MarbleListNode>(0);
   m_current->next = m_current;
   m_current->previous = m_current;

   for (unsigned int marble = 1; marble <= m_max_marble; ++marble)
   {
      unsigned int current_player = marble % m_players;
      if (marble % 23 == 0)
      {
         // Scoring turn
         m_player_scores[current_player] += marble;

         MarbleListNodePtr marble_to_remove{ m_current };
         for (unsigned int iteration = 0; iteration < 7; ++iteration)
         {
            marble_to_remove = marble_to_remove->previous;
         }

         m_current = marble_to_remove->next;
         m_player_scores[current_player] += remove_marble(marble_to_remove);
      }
      else
      {
         // Placing turn
         m_current = add_marble(m_current->next, marble);
      }
   }

   auto winning_player = std::max_element(m_player_scores.begin(), m_player_scores.end(), [](const std::pair<unsigned int, unsigned int> p1, const std::pair<unsigned int, unsigned int> p2){ return p1.second < p2.second; });
   return winning_player->second;
}

MarbleListNodePtr MarbleList::add_marble(MarbleListNodePtr& left, const unsigned int marble)
{
   MarbleListNodePtr right{ left->next };
   MarbleListNodePtr new_marble{ std::make_shared<MarbleListNode>(marble) };
   new_marble->previous = left;
   new_marble->next = right;
   left->next = new_marble;
   right->previous = new_marble;
   return new_marble;
}

unsigned int MarbleList::remove_marble(MarbleListNodePtr& marble)
{
   MarbleListNodePtr left{ marble->previous }, right{ marble->next };
   left->next = right;
   right->previous = left;
   marble->next = nullptr;
   marble->previous = nullptr;

   return marble->m_points;
}
