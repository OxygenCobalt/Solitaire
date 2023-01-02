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
 * the functionality of the Selection game element, which enables drag and drop
 * support.
 */

#include "Selection.h"

using namespace sf;
using namespace std;

Selection::Selection(
    const unsigned int ID, 
    const Vector2i ELEMENT_POS, 
    const Vector2i MOUSE_POS, 
    const list<Card> CARDS
) {
    mId = ID;
    mMousePos = MOUSE_POS;
    mOffset = ELEMENT_POS - MOUSE_POS;
    mCards = CARDS;
}

unsigned int Selection::getId() const {
    return mId;
}

bool Selection::intersects( const IntRect BOUNDS ) const {
    const Vector2i TRUE_POS = mGetTruePos();

    const IntRect THIS_BOUNDS(
        TRUE_POS.x,
        TRUE_POS.y,
        Card::WIDTH,
        Card::HEIGHT + (24 * (mCards.size() - 1))
    );

    return THIS_BOUNDS.intersects( BOUNDS );
}

void Selection::setMousePosition( const Vector2i MOUSE_POS ) {
    mMousePos = MOUSE_POS;
}

bool Selection::isConsumed() const {
    return mCards.size() == 0;
}

bool Selection::hasMultipleCards() const {
    return mCards.size() > 1;
}

Card Selection::getTop() const {
    return mCards.front();
}

void Selection::consume( list<Card>& dst, list<Card>::const_iterator dstPos ) {
    dst.splice( dstPos, mCards, mCards.begin(), mCards.end() );
}

void Selection::draw( RenderTarget& target, RenderStates states ) const {
    VertexArray vertices( Quads, 4 );

    int i = 0;
    const Vector2i TRUE_POS = mGetTruePos();
    for( Card card : mCards ) {
        card.intoVertices( 
            vertices,
            Vector2i(
                TRUE_POS.x,
                TRUE_POS.y + 24 * i
            )
        );

        target.draw( vertices, states );
        ++i;
    }
}

Vector2i Selection::mGetTruePos() const {
    // If we were just to draw the selection where the mouse is currently at,
    // it would cause the selection to jump to the top left corner unexpectedly,
    // which does not produce a good experience. So, we instead store the offset
    // within the original game element where the mouse originally selected it,
    // and then translate any new mouse positions by the calculated offset
    return mMousePos + mOffset;
}
