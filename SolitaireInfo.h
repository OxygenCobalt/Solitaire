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
 * the interface of the Solitaire Information data structure, which acts as a
 * record of a past or current solitaire game. 
 */

#ifndef SOLITAIRE_INFO_H
#define SOLITAIRE_INFO_H

/**
 * @brief Information about a particular solitaire game
 */
struct SolitaireInfo {
    /**
     * @brief Whether the game was completed
     */
    bool win;

    /**
     * @brief How many moves were performed in the games
     */
    unsigned int moves;

    /**
     * @brief The start time of the game, as a unix epoch timestamp
     */
    long startTime;

    /**
     * @brief The end time of the game, as a unix epoch timestamp
     */
    long endTime;

    /**H
     * @brief Get the total duration of this game, assuming valid
     * start and end timestamps
     * @return The total time, in seconds
     */
    long getDuration() const;
};

#endif