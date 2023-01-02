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
 * main draw loop and other user-facing code.
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

#include <cstdlib>
#include <ctime>

#include "History.h"
#include "Selection.h"
#include "Solitaire.h"
#include "Statistics.h"

/**
 * @brief Format a given duration into a (HH:)MM:SS string
 * @param DURATION The duration to format, in seconds
 * @return A formatted string
 */
string formatDuration( const long DURATION ) {
    // Calculate the trunacted hours/minutes/seconds values
    const unsigned long SECONDS = DURATION % 60 % 60;
    const unsigned long MINUTES = DURATION / 60 % 60;
    const unsigned long HOURS = DURATION / 60 / 60;

    string dst;

    // Don't write an hour value if we do not have
    // a non-zero value
    if( HOURS > 0 ) {
        if( HOURS < 10 ) {
            // Append a zero for single-digit hours
            dst += '0';
        }

        dst += to_string( HOURS );
        dst += ':';
    }

    // Always write minutes and seconds

    if( MINUTES < 10 ) {
        // Append a zero for single-digit minutes
        dst += '0';
    }

    dst += to_string( MINUTES );
    dst += ':';

    if( SECONDS < 10 ) {
        // Append a zero for single-digit seconds
        dst += '0';
    }

    dst += to_string( SECONDS );

    return dst;
}

/**
 * @brief Save the given statistics to the statistics file.
 * @param STATISTICS The statistics to write to the file
 * @return true if the file was successfully written, false otherwise
 */
bool saveStatistics( const Statistics STATISTICS ) {
    // This is placed within the main file for two reasons:
    // 1. The main file shows most of the user-facing information, so
    // it should also be responsible for the user-facing statistics
    // file
    // 2. This routine requires duration formatting, which is also within
    // the main file and is hard to integrate elsewhere

    // Open up the statistics file for writing
    ofstream statisticsOut;
    statisticsOut.open( "statistics.txt" );
    if( statisticsOut.fail() ) {
        return false;
    }

    if( STATISTICS.totalGames == 0 ) {
        // Most statistics calculations break without any games,
        // so write a placeholder to the statistics file in that
        // case
        statisticsOut << "Statistics about your past"
            << " solitaire games will be here"
            << " after you play some games!"
            << endl;

        return true;
    }

    statisticsOut << "YOUR SOLITAIRE STATISTICS:" << endl;

    // Write the total amount of games
    statisticsOut << "Total Games: " 
        << to_string( STATISTICS.totalGames ) << endl;

    // Write the win count and a win percentage
    statisticsOut << "Wins: " 
        << STATISTICS.wins
        << fixed << setprecision( 2 ) 
        << " (" 
        << (double) STATISTICS.wins / STATISTICS.totalGames * 100 
        << "%)"
        << endl;

    // Write out the current streak, noting whether the streak
    // is winning/losing
    statisticsOut << "Current Streak: "
        << STATISTICS.currentStreakLength;
    if( STATISTICS.isOnWinningStreak ) {
        statisticsOut << " (Winning)" << endl;
    } else {
        statisticsOut << " (Losing)" << endl;
    }

    // Write the longest winning/losing streaks
    statisticsOut << "Longest Winning Streak: "
        << STATISTICS.longestWinningStreak
        << endl;

    statisticsOut << "Longest Losing Streak: "
        << STATISTICS.longestLosingStreak
        << endl;

    // Only write smallest winning moves/shortest winning time
    // if any games have actually been won. Otherwise, they
    // should be written as N/A
    if( STATISTICS.wins > 0 ) {
        statisticsOut << "Shortest Winning Time: "
            << formatDuration( STATISTICS.shortestWinDuration )  
            << endl;   

        statisticsOut << "Smallest Winning Moves: "
            << STATISTICS.smallestWinMoves
            << endl;
    } else {
        statisticsOut << "Smallest Winning Moves: N/A" << endl;
        statisticsOut << "Smallest Losing Moves: N/A" << endl;
    }

    statisticsOut.close();

    return true;
}

int main() {
    srand( time( 0 ) );
    rand();

    // Load the game textures. To speed up loading, instead of many texture files
    // for each card, instead a spritesheet is loaded that contains all cards, with
    // specific cards being textured by restricting the texture coordinates to a
    // certain point in the spritesheet
    Texture textures;
    if( !textures.loadFromFile( "data/textures.png" ) ) {
        cerr << "Unable to open textures file" << endl; 
        return -1;
    }

    // Load the game font
    Font font;
    if( !font.loadFromFile( "data/arial.ttf" ) ) {
        cerr << "Unable to open font file" << endl;
        return -1;
    }

    // Generate a new solitaire game
    Solitaire* pSolitaire = new Solitaire;
    Selection* pSelection = nullptr;

    // Create a window
    RenderWindow window( VideoMode( 576, 576 ), "FP", Style::Titlebar | Style::Close );
    window.setFramerateLimit( 60 );

    // Instead of providing the texture to game elements, it is provided to
    // the window instead, and game elements simply operate under the assumption
    // that the card spritesheet is being applied
    RenderStates states;
    states.texture = &textures;

    // This text shows the current move counter, game time, and whether the user
    // has won or not
    Text infoText;
    infoText.setFont( font );
    infoText.setCharacterSize( 16 );
    infoText.setPosition( 16, 544 );

    // Create an event object to re-use for future events
    Event event;

    // While the window is open
    while( window.isOpen() ) {
        // Clear the previous frame with a dark green background
        window.clear( Color( 0, 64, 0 ) );

        // Draw the game elements
        window.draw( infoText );
        window.draw( *pSolitaire, states );
        if( pSelection != nullptr ) {
            // If available, draw selections last, as they should
            // not clip behind any other game elements
            window.draw( *pSelection, states );
        }

        // Display the new frame
        window.display();
        
        while( window.pollEvent( event ) ) {
            if( event.type == Event::Closed ) {
                // Window closed
                window.close();
            }

            if( event.type == Event::EventType::MouseButtonPressed 
                && event.mouseButton.button == Mouse::Button::Left ) {
                // Game element possibly clicked, if something is clicked,
                // pSelection will already be modified, so there is nothing
                // we need to do on our end
                pSolitaire->click( 
                    Vector2i( event.mouseButton.x, event.mouseButton.y ), pSelection );
            }

            if( event.type == Event::EventType::MouseMoved && pSelection != nullptr ) {
                // Mouse moved, update the selection position if we have one
                pSelection->setMousePosition( 
                    Vector2i( event.mouseMove.x, event.mouseMove.y ) );
            }

            if( event.type == Event::EventType::MouseButtonReleased && pSelection != nullptr) {
                // Mouse released, release the selection
                pSolitaire->release( pSelection );
            }
        }

        // Update the information text with the possibly changed
        // game state
        const SolitaireInfo INFO = pSolitaire->getInfo();

        string infoString;
        if( INFO.win ) {
            infoString += "You Win! | ";
        }

        infoString += "Moves: ";
        infoString += to_string( INFO.moves );

        infoString += " | Time: ";
        infoString += formatDuration( INFO.getDuration() );

        infoText.setString( infoString );
    }

    History* pHistory = new History;
    if( pHistory->load() ) {
        // Successfully loaded history, add this game to it
        pHistory->add( pSolitaire->getInfo() );

        // Calculate the new statistics that includes the current game,
        // and then try to write it out to it's file
        Statistics statistics = pHistory->getStatistics();
        if( !saveStatistics( statistics ) ) {
            cerr << "Unable to save statistics file" << endl;
        }

        // Save the new information back to the history file
        if( !pHistory->save() ) {
            cerr << "Unable to save history file" << endl;
        }
    } else {
        cerr << "Unable to open history file" << endl;
    }

    delete pHistory;
    delete pSolitaire;

    return 0;
}
