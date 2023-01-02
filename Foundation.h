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
 * the the interface of the "Foundation" game element, in which completing it is the
 * objective of Solitaire.
 */

#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <SFML/Graphics.hpp>

#include <list>

#include "Card.h"
#include "CardSuit.h"
#include "IDroppable.h"
#include "Selection.h"

/**
 * @brief The Foundation solitaire game element. This is where
 * cards are placed in order to complete the game
 */
class Foundation final : public sf::Drawable, public IDroppable {
public:
    /**
     * @brief The amount of foundations in a solitaire game
     */
    static const unsigned int AMOUNT = CardSuit::DIAMONDS + 1;

    /**
     * @brief Create a foundation
     * @param SUIT The suit of the cards that should be placed in this foundation
     */
    Foundation( const CardSuit SUIT );

    /**
     * @brief Get if this foundation is complete
     * @return true if the foundation is complete (All cards from Ace to King
     * have been placed in the foundation), false otherwise
     */
    bool isComplete() const;

    bool click( const sf::Vector2i MOUSE_POS, Selection*& pSelection ) override;
    void release( Selection*& pSelection ) override;

    void drop( Selection& selection ) override;

    void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;

private:
    CardSuit mSuit;
    std::list<Card> mCards;
    sf::IntRect mBounds;

    /**
     * @brief Get the selection ID for this foundation
     * @return A unique selection ID
     */
    unsigned int mGetId() const;
};

#endif