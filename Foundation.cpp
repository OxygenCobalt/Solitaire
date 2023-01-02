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
 * the the functionality of the "Foundation" game element, in which completing it
 * is the objective of Solitaire.
 */

#include "Foundation.h"

using namespace sf;
using namespace std;

Foundation::Foundation( const CardSuit SUIT ) {
    mSuit = SUIT;

    // Place foundations within the top right of the window
    mBounds = IntRect( 
        16 + (Card::WIDTH + 16) * (3 + mSuit), 
        16, Card::WIDTH, Card::HEIGHT 
    );
}

bool Foundation::isComplete() const {
    if( mCards.size() > 0 ) { 
        // We already establish that any cards added must follow
        // an ascending ordering, so if the rank of the top of
        // the foundation is a king, we must be complete
        return mCards.back().getRank() == CardRank::KING;
    }

    return false;
}

bool Foundation::click( const Vector2i MOUSE_POS, Selection*& pSelection ) {
    if( mBounds.contains( MOUSE_POS ) && mCards.size() > 0 ) {
        // Can select this foundation, move the top card into a selection
        list<Card> cards;
        cards.splice( 
            cards.begin(), 
            mCards, --mCards.end(), mCards.end() 
        );

        pSelection = new Selection( 
            mGetId(),
            Vector2i( mBounds.left, mBounds.top ), 
            MOUSE_POS,
            cards
        );
    }

    return false;
}

void Foundation::release( Selection*& pSelection ) {
    if( !pSelection->isConsumed() ) {
        // Not consumed, return to foundation
        pSelection->consume( mCards, mCards.end() );
    }

    delete pSelection;
    pSelection = nullptr;
}

void Foundation::drop( Selection& selection ) {
    if( selection.getId() == mGetId() ) {
        // This foundation created the selection, avoid a
        // superfluous move
        return;
    }

    if( !selection.intersects( mBounds ) ) {
        // Not in bounds
        return;
    }

    if( selection.hasMultipleCards() ) {
        // Foundations can only be built one card at a time
        return;
    }

    // Cards added to the foundation must be of the same suit
    const Card CARD = selection.getTop();
    if( CARD.getSuit() != mSuit ) {
        return;
    }

    // Cards must be added to the foundation in ascending order (King -> Ace)
    const bool IS_STARTING = mCards.size() == 0 
        && CARD.getRank() == CardRank::ACE;
    const bool IS_CONTINUING = mCards.size() > 0 
        && CARD.getRank() == mCards.back().getRank() + 1;
    if( !IS_STARTING && !IS_CONTINUING ) {
        return;
    }

    selection.consume( mCards, mCards.end() );
}

void Foundation::draw( RenderTarget& target, RenderStates states ) const {
    VertexArray vertices( Quads, 4 );

    Card top;
    if( mCards.size() > 0 ) {
        // We have cards, show the uppermost card
        top = mCards.back();
    } else {
        // No cards, show an empty placeholder
        top = Card( mSuit );
    }

    top.intoVertices( vertices, Vector2i( mBounds.left, mBounds.top ) );
    target.draw( vertices, states );
}

unsigned int Foundation::mGetId() const {
    return (mSuit + 1) << AMOUNT;
}
