/**
 * \file SearchResult.h
 * \brief This file contains the A* search result class.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#ifndef SEARCH_RESULT_H__
#define SEARCH_RESULT_H__

#include <cstddef>
#include <chrono>
#include "memusage.h"
#include "boxedintypes.h"
#include "Node.h"
#include "EncodedPath.h"

namespace boxedin {

    /**
       \class SearchResult
       \brief A* search results, statistics, possibly a solution
     */
    class SearchResult
    {
    public:
        bool success;
        int num_moves;
        size_t openset_size;
        size_t closedset_size;
        std::chrono::steady_clock::time_point search_start_time;
        std::chrono::steady_clock::time_point search_stop_time;
        std::string solution; // if search succeeded
        MemUsage memusage;

        SearchResult()
            : success(false)
            , num_moves(-1)
            , openset_size(0)
            , closedset_size(0)
        {
            search_start_time = std::chrono::steady_clock::now();
        }

        void SetFailed(size_t openset_size, size_t closedset_size)
        {
            search_stop_time = std::chrono::steady_clock::now();
            success = false;
            this->openset_size = openset_size;
            this->closedset_size = closedset_size;

            GetMemUsage(memusage);
        }
        void SetSucceeded(const Node* node, size_t openset_size, size_t closedset_size)
        {
            search_stop_time = std::chrono::steady_clock::now();
            success = true;
            this->openset_size = openset_size;
            this->closedset_size = closedset_size;

            GetMemUsage(memusage);

            num_moves = 0;
            while (node)
            {
                num_moves += (int)node->path_.size();
                for (int i = 0; i < (int)node->path_.size(); i++)
                {
                    switch (node->path_.at(i))
                    {
                    case ENCODED_PATH_DIRECTION_UP:
                        solution.insert(0, 1, 'U');
                        break;
                    case ENCODED_PATH_DIRECTION_DOWN:
                        solution.insert(0, 1, 'D');
                        break;
                    case ENCODED_PATH_DIRECTION_LEFT:
                        solution.insert(0, 1, 'L');
                        break;
                    case ENCODED_PATH_DIRECTION_RIGHT:
                        solution.insert(0, 1, 'R');
                        break;
                    }
                }
                node = node->predecessor_;
            }
        }
    };

} // namespace

#endif
