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
 * the interface of the Klondike Solitaire game.
 */

#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include <SFML/Graphics.hpp>

#include "Deck.h"
#include "Column.h"
#include "Foundation.h"
#include "ISelectable.h"
#include "Selection.h"
#include "SolitaireInfo.h"

/**
 * @brief A game of (Klondike) Solitaire
 */
class Solitaire final : public sf::Drawable {
public:
    /**
     * @brief Creates a new solitaire game with a randomized deal 
     */
    Solitaire();
    Solitaire( const Solitaire& OTHER );
    Solitaire& operator=( const Solitaire& OTHER );
    ~Solitaire();

    /**
     * @brief Get information about the game
     * @return A collection of several data points about the current
     * game
     */
    SolitaireInfo getInfo() const;
    
    /**
     * @brief Forward a mouse click event into the game
     * @param MOUSE_POS The position in the window that the mouse clicked
     * @param pSelection A selection that may be populated if the click
     * event will pick up some cards
     */
    void click( const sf::Vector2i MOUSE_POS, Selection*& pSelection );

    /**
     * @brief Release a selection of cards into the game
     * @param pSelection A non-null selection of cards.
     */
    void release( Selection*& pSelection );

    void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;

private:
    Deck* mPDeck;
    Foundation** mPPFoundations; 
    Column** mPPColumns;
    ISelectable* mPSelected;
    SolitaireInfo mInfo;

    /**
     * @brief Clears and de-allocates this game
     */
    void mClear();

    /**
     * @brief Copy the state of another game into this one
     * @param OTHER Another solitaire game
     */
    void mCopyFrom( const Solitaire& OTHER );

    /**
     * @brief Attempt to forward a click event to any game elements
     * @param MOUSE_POS The position in the window that the mouse clicked
     * @param pSelection A selection that may be populated if the click
     * event will pick up some cards
     * @return true if a *move* was done, false otherwise. A move may
     * be done without pSelection being populated
     */
    bool mClick( const sf::Vector2i MOUSE_POS, Selection*& pSelection );

    /**
     * @brief Attempt to drop a selection into any game elements
     * @param selection The selection to drop
     * @return true if the selection was consumed, false otherwise
     */
    bool mDrop( Selection& selection );

    /**
     * @brief Check if all foundations are complete in this game
     * (i.e the win condition)
     * @return true if the foundations are complete, false otherwise
     */
    bool mAreAllFoundationsComplete();
};

#endif