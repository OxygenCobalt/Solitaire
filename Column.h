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
 * the the interface of the "Column" game element, in which cards are re-arranged
 * in order to complete the game.
 */

#ifndef COLUMN_H
#define COLUMN_H

#include <SFML/Graphics.hpp>

#include <list>

#include "Card.h"
#include "Deck.h"
#include "IDroppable.h"
#include "Selection.h"

/**
 * @brief The Column solitaire game element. Cards must be re-arranged
 * between these in order to complete the game
 */
class Column final : public sf::Drawable, public IDroppable {
public:
    /**
     * @brief The amount of columns in a solitaire game
     */
    static const unsigned int AMOUNT = 7;

    /**
     * @brief Create a column populated from the given deck
     * @param NUMBER The number of the deck, in the range 
     * [0, AMOUNT]. This controls where the column is
     * placed and how many cards it will initially have
     * @param deck The deck to populate the column from
     */
    Column( const unsigned int NUMBER, Deck& deck );

    bool click( const sf::Vector2i MOUSE_POS, Selection*& pSelection ) override;
    void release( Selection*& pSelection ) override;
    
    void drop( Selection& selection ) override;

    void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;

private:
    std::list<Card> mHiddenCards;
    std::list<Card> mShownCards;
    int mNumber;

    /**
     * @brief Reveals the bottom hidden card if there are no shown cards in
     * this column.
     */
    void mReveal();

    /**
     * @brief Get the selection ID of this column
     * @return A unique selection ID
     */
    unsigned int mGetId() const;

    /**
     * @brief Get the total amount of cards in this column
     * @return The combined amount of hidden and shown cards
     */
    unsigned int mGetTotalCards() const;

    /**
     * @brief Get the bounds of the entire column
     * @return A rect representing the bounds of the column
     * as is displayed in the window
     */
    sf::IntRect mGetTotalBounds() const;

    /**
     * @brief Get the bounds of a particular card at IDX
     * @param IDX The index of the card to retrive the bounds
     * from, in the range [0, TOTAL_CARDS)
     * @return A rect representing the bounds of the desired
     * card as is displayed in the window
     */
    sf::IntRect mGetBoundsAt( const unsigned int IDX ) const;
};

#endif