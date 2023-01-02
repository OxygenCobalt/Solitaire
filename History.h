/* CSCI 200: Final Project
 *
 * Author: Alexander Capehart
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 * - https://en.sfml-dev.org/forums/index.php?topic=10351.0 - Disable window resizing
 * - https://cplusplus.com/reference/stack/stack/ - For stack usage
 * - https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1VertexArray.php - For vertex array usage
 * - https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Vector2.php - For vector usage
 * - https://cplusplus.com/reference/list/list/ - For list usage
 * - https://stackoverflow.com/questions/34314892/high-cpu-usage-of-simple-program - To reduce CPU usage
 * - https://www.sfml-dev.org/tutorials/2.5/graphics-draw.php - For vertex array and renderstates usage
 * - https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Rect.php - For rect usage
 * - https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Drawable.php - For drawable implementation
 * - https://cplusplus.com/doc/tutorial/other_data_types/ for enum creation/usage
 * - Learned about for-each loops from Professor Paone
 * - Confirmed proper project formatting from Professor Paone
 *
 * This program implements a game of klondike solitaire. This file implements the
 * the the interface of the game history data structure.
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <fstream>
#include <list>

#include <cmath>

#include "SolitaireInfo.h"
#include "Statistics.h"

/**
 * @brief A log of past solitaire games
 */
class History final {
public:
    /**
     * @brief Create an uninitialized history instance
     */
    History();

    /**
     * @brief Load the history from the expected "history.dat"
     * file. Invalid entries are ignored
     * @return true if the file was loaded successfully, false
     * otherwise
     */
    bool load();

    /**
     * @brief Add new game information to the history
     * @param INFO The information to add
     */
    void add( const SolitaireInfo INFO );

    /**
     * @brief Calculate statistics based on the history
     * @return A set of calculated statistics
     */
    Statistics getStatistics() const;

    /**
     * @brief Save the history to the expected "history.dat"
     * file
     * @return true if the file was saved successfully, false
     * otherwise
     */
    bool save() const;

private:
    std::list<SolitaireInfo> mInfos;

    /**
     * @brief Attempt to read a solitaire info entry from the log
     * @param logIn The input  stream to the history file
     * @param info The destination information data
     * @return true if it was successfully read, false otherwise
     */
    bool mLoadInfo( std::ifstream& historyIn, SolitaireInfo& info ) const;
};


#endif