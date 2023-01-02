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
 * the interface of the Selection game element, which enables drag and drop
 * support.
 */

#ifndef SELECTION_H
#define SELECTION_H

#include <SFML/Graphics.hpp>

#include <list>

#include "Card.h"

/**
 * @brief A draggable selection of one or more cards
 */
class Selection final : public sf::Drawable {
public:
    /**
     * @brief Create a selection.
     * @param ID The unique ID for this selection that can be used to
     * avoid selections created by a game element from being dropped on
     * the same game element
     * @param ELEMENT_POS The position of the element that created the
     * selection
     * @param MOUSE_POS The position of the mouse when the selection was
     * created
     * @param CARDS The cards to select
     */
    Selection(
        const unsigned int ID, 
        const sf::Vector2i ELEMENT_POS, 
        const sf::Vector2i MOUSE_POS, 
        const std::list<Card> CARDS
    );

    /**
     * @brief Get the ID of this selection. This can be used to identify
     * the game element that was originally selected.
     * @return A unique ID
     */
    unsigned int getId() const;

    /**
     * @brief Check if this selection intersects with a bounded rect
     * @param BOUNDS The bounds to check
     */
    bool intersects( const sf::IntRect BOUNDS ) const;

    /**
     * @brief Update the mouse position of the selection
     * @param MOUSE_POS The position of the mouse on the window
     */
    void setMousePosition( const sf::Vector2i MOUSE_POS );

    /**
     * @brief Check if this selection is consumed
     * @return true if the selection is consumed (has no cards), false
     * otherwise
     */
    bool isConsumed() const;

    /**
     * @brief Check if this selection has multiple cards
     * @return true if the selection has multiple cards, false otherwise
     */
    bool hasMultipleCards() const;

    /**
     * @brief Get the visual "top" of the selection
     * @return The front value within the internal list of cards 
     */
    Card getTop() const;

    /**
     * @brief Consume the selection into a destination list of cards
     * @param dst The destination list of cards
     * @param dstPos The iterator position at which to append the cards
     */
    void consume( std::list<Card>& dst, std::list<Card>::const_iterator dstPos );

    void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;

private:
    unsigned int mId;
    sf::Vector2i mMousePos;
    sf::Vector2i mOffset;
    std::list<Card> mCards;

    /**
     * @brief Get the "true" position of the selection, taking into account
     * the point at which the selection was originally clicked on
     * @return A position value representing the true selection position
     */
    sf::Vector2i mGetTruePos() const;
};

#endif