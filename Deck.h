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
 * the the interface of the "Deck" game element, which is where new Cards are drawn
 * from in Solitaire.
 */

#ifndef DECK_H
#define DECK_H

#include <SFML/Graphics.hpp>

#include <list>

#include "Card.h"
#include "ISelectable.h"
#include "Selection.h"

/**
 * @brief The Deck solitaire game element. This is where cards
 * are drawn from
 */
class Deck final : public sf::Drawable, public ISelectable {
public:

    /**
     * @brief Create a new shuffled deck
     */
    Deck();

    /**
     * @brief Pull a card from the deck so that other game elements can
     * be seeded with cards
     * @return A card from the top of the "hidden" portion of the deck
     */
    Card pull();

    bool click( const sf::Vector2i MOUSE_POS, Selection*& pSelection ) override;
    void release( Selection*& pSelection ) override;

    void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;

private:
    std::list<Card> mHiddenCards;
    std::list<Card> mRevealedCards;
    sf::IntRect mHiddenBounds;
    sf::IntRect mRevealedBounds;
};

#endif