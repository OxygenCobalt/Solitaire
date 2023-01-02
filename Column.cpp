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
 * the the functionality of the "Column" game element, in which cards are re-arranged
 * in order to complete the game.
 */

#include "Column.h"

using namespace sf;
using namespace std;

#include <cmath>

Column::Column( const unsigned int NUMBER, Deck& deck ) {
    mNumber = NUMBER;

    // Initialize the column with cards from the deck such
    // that each column has one more card than the previous
    while( mHiddenCards.size() <= (unsigned int) mNumber ) {
        mHiddenCards.push_back( deck.pull() );
    }

    // Reveal only the first card initially 
    mReveal();
}

bool Column::click( const Vector2i MOUSE_POS, Selection*& pSelection ) {
    IntRect selectionBounds;
    list<Card>::const_iterator selectionIter = mShownCards.begin();

    // What cards we select depends on where in the shown cards the mouse
    // clicked.
    unsigned int i;
    for( i = mHiddenCards.size(); i < mGetTotalCards(); ++i ) {
        const IntRect BOUNDS = mGetBoundsAt( i );
        if( BOUNDS.contains( MOUSE_POS ) ) {
            // Found a card that is within the mouse bounds, select this
            // card
            selectionBounds = BOUNDS;
            break;
        }

        ++selectionIter;
    }

    if( i < mGetTotalCards() ) {
        // Select the card that was clicked on *and* any following cards
        list<Card> cards;
        cards.splice( 
            cards.begin(), 
            mShownCards, selectionIter, mShownCards.end() 
        );

        pSelection = new Selection( 
            mGetId(),
            Vector2i( selectionBounds.left, selectionBounds.top ), 
            MOUSE_POS,
            cards
        );
    }

    // If the loop was terminated by the inequality, we didn't find a card 

    return false;
}

void Column::release( Selection*& pSelection ) {
    if( pSelection->isConsumed() ) {
        // Consumed, reveal a new card if needed
        mReveal();
    } else {
        // Not consumed, return it to this instance
        pSelection->consume( mShownCards, mShownCards.end() );
    }

    delete pSelection;
    pSelection = nullptr;
}

void Column::drop( Selection& selection ) {
    if( selection.getId() == mGetId() ) {
        // This column created the selection, avoid a
        // superfluous move
        return;
    }

    if( !selection.intersects( mGetTotalBounds() ) ) {
        // Not in bounds
        return;
    }

    const Card TOP = selection.getTop();

    // Cards added to a column must be in descending order (King to Ace) 
    // and alternating in color (Black to Red and vice versa)
    if( mShownCards.size() > 0 ) {
        const Card BOTTOM = mShownCards.back();
        const bool IS_ALTERNATING = TOP.getColor() != BOTTOM.getColor();
        const bool IS_DESCENDING = TOP.getRank() == BOTTOM.getRank() - 1;
        if( !IS_ALTERNATING || !IS_DESCENDING ) {
            return;
        }
    } else if( TOP.getRank() != CardRank::KING ) {
        return;
    }

    selection.consume( mShownCards, mShownCards.end() );
}

void Column::draw( RenderTarget& target, RenderStates states ) const {
    VertexArray vertices( Quads, 4 );

    // Use a placeholder when drawing all hidden cards
    unsigned int i;
    const Card HIDDEN;
    for( i = 0; i < mHiddenCards.size(); ++i) {
        const IntRect BOUNDS = mGetBoundsAt( i );
        HIDDEN.intoVertices( vertices, Vector2i( BOUNDS.left, BOUNDS.top ) );
        target.draw( vertices, states );
    }

    // Draw the remaining shown cards afterwards
    for( Card card : mShownCards ) { 
        const IntRect BOUNDS = mGetBoundsAt( i );
        card.intoVertices( vertices, Vector2i( BOUNDS.left, BOUNDS.top ) );
        target.draw( vertices, states );
        ++i;
    }
}

void Column::mReveal() {
    if( mHiddenCards.size() == 0 || mShownCards.size() > 0 ) {
        // No hidden cards/multiple shown cards, we shouldn't
        // reveal anything
        return;
    }

    mShownCards.push_back( mHiddenCards.back() );
    mHiddenCards.pop_back();
}

unsigned int Column::mGetId() const {
    return (mNumber + 1) << AMOUNT;
}

unsigned int Column::mGetTotalCards() const {
    return mHiddenCards.size() + mShownCards.size();
}

IntRect Column::mGetTotalBounds() const {
    return IntRect(
        16 + mNumber * (Card::WIDTH + 16),
        32 + Card::HEIGHT,
        Card::WIDTH,
        // Total card amount is unsigned, so we want to make sure
        // we don't underflow with empty columns and create absurdly
        // tall bounds
        Card::HEIGHT + 24 * min(mGetTotalCards() - 1, mGetTotalCards())
    );
}

IntRect Column::mGetBoundsAt( const unsigned int IDX ) const {
    IntRect bounds(
        16 + mNumber * (Card::WIDTH + 16),
        32 + (Card::HEIGHT + 24 * IDX),
        Card::WIDTH,
        Card::HEIGHT
    );

    // Cards that are not at the bottom of a column are only
    // 24 pixels tall instead of the typical card height,
    // so this must be reflected in the bounds as well
    const bool IS_MULTIPLE = mGetTotalCards() > 1;
    const bool IS_BEHIND = IDX < mGetTotalCards() - 1 ;
    if( IS_MULTIPLE && IS_BEHIND ) {
        bounds.height = 24;
    }

    return bounds;
}
