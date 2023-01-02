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
 * the Selectable game element interface, which allows game elements to handle
 * click events.
 */

#ifndef SELECTABLE_H
#define SELECTABLE_H

#include <SFML/Graphics.hpp>

#include "Selection.h"

/**
 * @brief An game element that can be selected
 */
class ISelectable {
public:
    virtual ~ISelectable() {}

    /**
     * @brief Called when the element might have been clicked on by the mouse
     * @param MOUSE_POS The position in the window that the mouse clicked
     * @param pSelection A selection that can be populated if the mouse has
     * picked up cards from the element. This should be allocated on the
     * free store
     * @return true if a *move* was performed, false otherwise. An element
     * can leave pSelection unpopulated and still return true to signify that
     * a move occurred
     */
    virtual bool click( const sf::Vector2i MOUSE_POS, Selection*& pSelection ) = 0;

    /**
     * @brief Called when a selection obtained from this element has been
     * released
     * @param pSelection The selection. This should be deleted and reset by
     * the element
     * @param RETURN Whether the selection is being returned to the element
     * or not
     */
    virtual void release( Selection*& pSelection ) = 0;
};

#endif