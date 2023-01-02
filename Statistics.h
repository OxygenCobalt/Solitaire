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
 * the interface of the Statistics data structure, which represents a list of
 * statistics calculated from the Solitaire Game history.
 */

#ifndef STATISTICS_H
#define STATISTICS_H

/**
 * @brief A set of game statistics derived from the game history
 */
struct Statistics {
    /**
     * @brief The total amount of games that have been played
     */
    unsigned int totalGames;

    /**
     * @brief The total amount of games that were won
     */
    unsigned int wins;

    /**
     * @brief If the player is currently on a winning streak.
     * If false, a streak is still occurring, it is just a losing
     * streak instead
     */
    bool isOnWinningStreak;

    /**
     * @brief The current length of the streak outlined by
     * isOnWinningStreak
     */
    unsigned int currentStreakLength;

    /**
     * @brief The longest winning streak achieved by the player,
     * can be in-progress and thus equal to currentStreakLength
     */
    unsigned int longestWinningStreak;

    /**
     * @brief The longest losing streak achieved by the player,
     * can be in-progress and thus equal to currentStreakLength
     */
    unsigned int longestLosingStreak;

    /**
     * @brief The shortest time it took to win a game, in seconds
     */
    long shortestWinDuration;

    /**
     * @brief The smallest amount of moves it took to win a game
     */
    unsigned int smallestWinMoves;
};

#endif