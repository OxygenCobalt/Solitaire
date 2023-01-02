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
 * the the functionality of the game history data structure.
 */

#include "History.h"

#include <iostream>
using namespace std;

History::History() {
    // Nothing to do
}

bool History::load() {
    ifstream historyIn;
    historyIn.open( "data/history.dat" );
    if( historyIn.fail() ) {
        return false;
    }

    // History data begins with a number specifying the amount of
    // game information that exists in the file
    unsigned int length;
    historyIn >> length;
    if( historyIn.fail() ) {
        // A history without a length value is just an empty history
        return true;
    }

    // The rest of the file is game information
    SolitaireInfo info;
    for( unsigned int i = 0; i < length; ++i ) {
        if( mLoadInfo( historyIn, info ) ) {
            // This is valid information data
            mInfos.push_back( info );
        } else {
            // This is not valid information data, skip to the next
            // value and let the user know
            cerr << "Unable to read history entry #" << (i + 1) << endl;
            historyIn.clear();
            char badChar = 0;
            do { 
                historyIn.get( badChar ); 
            } while( badChar != '\n' && badChar != 0 );
        }
    }

    historyIn.close();

    return true;
}

void History::add( const SolitaireInfo INFO ) {
    // Only add games where the player made a move. This prevents
    // very short, uncompleted games from polluting the history
    if( INFO.moves > 0 ) {
        mInfos.push_back( INFO );
    }
}

Statistics History::getStatistics() const {
    // Most statistics values can be initialized with zeroes.
    // This is okay, as these values are nearly always overwritten
    // by the time we are done
    Statistics statistics {
        totalGames: (unsigned int) mInfos.size(),
        0
    };

    for( SolitaireInfo info : mInfos ) {
        if( info.win ) {
            if( statistics.wins == 0 ) {
                // This is the first win, so it msut currently have
                // the lowest winning time and lowest winning moves
                statistics.shortestWinDuration = info.getDuration();
                statistics.smallestWinMoves = info.moves;
            } else {
                // Otherwise, do the typical min checking
                if( info.getDuration() < statistics.shortestWinDuration ) {
                    statistics.shortestWinDuration = info.getDuration();
                }

                if( info.moves < statistics.smallestWinMoves ) {
                    statistics.smallestWinMoves = info.moves;
                }
            }

            if( statistics.isOnWinningStreak ) {
                // This win extends the current winning streak
                ++statistics.currentStreakLength;
            } else {
                // This win starts a winning streak. Keep the last
                // streak if it ends up being the longest losing streak
                if( statistics.currentStreakLength > statistics.longestLosingStreak ) {
                    statistics.longestLosingStreak = statistics.currentStreakLength;
                }

                statistics.isOnWinningStreak = true;
                statistics.currentStreakLength = 1;
            }

            ++statistics.wins;
        } else {
            if( !statistics.isOnWinningStreak ) {
                // This loss extends the current losing streak
                ++statistics.currentStreakLength;
            } else {
                // The loss starts a losing streak. Kepe the last streak
                // if it ends up being the longest winning streak
                if( statistics.currentStreakLength > statistics.longestWinningStreak ) {
                    statistics.longestWinningStreak = statistics.currentStreakLength;
                }

                statistics.isOnWinningStreak = false;
                statistics.currentStreakLength = 1;
            }
        }
    }

    // If the current streak we built up is actually the longest streak,
    // make sure we change the statistic to such
    const bool PENDING_WINNING_STREAK = statistics.isOnWinningStreak 
        && statistics.currentStreakLength > statistics.longestWinningStreak;
    const bool PENDING_LOSING_STREAK = !statistics.isOnWinningStreak 
        && statistics.currentStreakLength > statistics.longestLosingStreak;

    if( PENDING_WINNING_STREAK ) {
        statistics.longestWinningStreak = statistics.currentStreakLength;
    } else if( PENDING_LOSING_STREAK ) {
        statistics.longestLosingStreak = statistics.currentStreakLength;
    }

    return statistics;
}

bool History::save() const {
    // Open the history file
    ofstream historyOut;
    historyOut.open( "data/history.dat" );
    if( historyOut.fail() ) {
        return false;
    }

    // Only write to the file if we actually have information
    if( mInfos.size() > 0 ) {
        // Write the amount first
        historyOut << mInfos.size() << endl;

        // And then write all of the history entries in the same
        // order as how they are read
        for( SolitaireInfo info : mInfos ) {
            historyOut << info.win << ' ' 
                << info.moves << ' '
                << info.startTime << ' '
                << info.endTime << endl;
        }
    }

    historyOut.close();

    return true;
}

bool History::mLoadInfo( ifstream& stream, SolitaireInfo& info ) const {
    // History entries consist of a line containing four entries
    // representing the win/loss state, the moves, the start time,
    // and the end time, in that order
    stream >> info.win;
    if( stream.fail() ) {
        return false;
    }

    // The move count should always be non-zero
    stream >> info.moves;
    if( stream.fail() || info.moves < 1 ) {
        return false;
    }

    stream >> info.startTime;
    if( stream.fail() ) {
        return false;
    }

    // The game duration should be non-zero
    stream >> info.endTime;
    if( stream.fail() || info.getDuration() < 0 ) {
        return false;
    }

    return true;
}
