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
 * the functionality of the Klondike Solitaire game, which mostly involves
 * controlling each game element.
 */

#include "Solitaire.h"

using namespace sf;

#include <ctime>

#include "CardSuit.h"

Solitaire::Solitaire() {
    // Create (and shuffle) a deck
    mPDeck = new Deck();

    // Create empty founds
    mPPFoundations = new Foundation*[Foundation::AMOUNT];
    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        mPPFoundations[i] = new Foundation( (CardSuit) i );
    }

    // Create foundations and populate them with the deck
    mPPColumns = new Column*[Column::AMOUNT];
    for( unsigned int i = 0; i < Column::AMOUNT; ++i ) {
        mPPColumns[i] = new Column( i, *mPDeck );
    }

    mPSelected = nullptr;

    // Initialize game information 
    mInfo.win = false;
    mInfo.moves = 0;
    mInfo.startTime = time( 0 );

    // Currently an ongoing game, so no end time
    mInfo.endTime = -1;
}

Solitaire::Solitaire( const Solitaire& OTHER ) {
    mCopyFrom( OTHER );
}

Solitaire& Solitaire::operator=( const Solitaire& OTHER ) {
    if( this == &OTHER ) {
        return *this;
    }

    mClear();
    mCopyFrom( OTHER );

    return *this;    
}

Solitaire::~Solitaire() {
    mClear();
}

SolitaireInfo Solitaire::getInfo() const {
    SolitaireInfo info = mInfo;
    if( info.endTime < 0 ) {
        // No end time yet, replace it with the current time
        // to get an "in-progress" elapsed time
        info.endTime = time( 0 );
    }
    return info;
}

void Solitaire::click( const Vector2i MOUSE_POS, Selection*& pSelection ) {
    if( mInfo.win ) {
        // Already won, no more moves can be done
        return;
    }

    if( mClick( MOUSE_POS, pSelection ) ) {
        ++mInfo.moves;
    }
}

void Solitaire::release( Selection*& pSelection ) {
    if( mDrop( *pSelection ) ) {
        // Successfully dropping a selection is always a move
        ++mInfo.moves;
    }

    // Signal the previously selected element that the
    // selection was released. If the selection was not
    // dropped, it will be returned to the element.
    mPSelected->release( pSelection );
    mPSelected = nullptr;

    if( mAreAllFoundationsComplete() ) {
        // All foundations are win, the user has won!
        mInfo.win = true;
        mInfo.endTime = time( 0 );
    }
}

void Solitaire::draw( RenderTarget& target, RenderStates states ) const {
	target.draw( *mPDeck, states );

    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        target.draw( *mPPFoundations[i], states );
    }

    for( unsigned int i = 0; i < Column::AMOUNT; ++i ) {
        target.draw( *mPPColumns[i], states );
    }
}

void Solitaire::mClear() {
    delete mPDeck;

    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        delete mPPFoundations[i];
    }
    delete[] mPPFoundations;

    for( unsigned int i = 0; i < Column::AMOUNT; ++i ) {
        delete mPPColumns[i];
    }
    delete[] mPPColumns;
}

void Solitaire::mCopyFrom( const Solitaire& OTHER ) {
    mPDeck = new Deck( *OTHER.mPDeck );

    mPPFoundations = new Foundation*[Foundation::AMOUNT];
    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        mPPFoundations[i] = new Foundation( *OTHER.mPPFoundations[i] );
    }

    mPPColumns = new Column*[Column::AMOUNT];
    for( unsigned int i = 0; i < Column::AMOUNT; ++i ) {
        mPPColumns[i] = new Column( *OTHER.mPPColumns[i] );
    }

    mInfo = OTHER.mInfo;
}

bool Solitaire::mClick( const Vector2i MOUSE_POS, Selection*& pSelection ) {
    // We have to simultaniously check if a move was performed or if
    // a selection was created. If either occurs, we are done checking.

    bool moved = mPDeck->click( MOUSE_POS, pSelection );
    if( moved || pSelection != nullptr ) {
        mPSelected = mPDeck;
        return moved;
    }

    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        moved = mPPFoundations[i]->click( MOUSE_POS, pSelection );
        if( moved || pSelection != nullptr ) {
            mPSelected = mPPFoundations[i];
            return moved;
        }
    }

    for( unsigned int i = 0; i < Column::AMOUNT; ++i ) {
        moved = mPPColumns[i]->click( MOUSE_POS, pSelection );
        if( moved || pSelection != nullptr ) {
            mPSelected = mPPColumns[i];
            return moved;
        }
    }

    return moved;
}

bool Solitaire::mDrop( Selection& selection ) {
    // Unlike in click, we only need to check if the selection was
    // consumed.

    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        mPPFoundations[i]->drop( selection );
        if( selection.isConsumed() ) {
            return true;
        }
    }

    for( unsigned int i = 0; i < Column::AMOUNT; ++i ) {
        mPPColumns[i]->drop( selection );
        if( selection.isConsumed() ) {
            return true;
        }
    }

    return false;
}

bool Solitaire::mAreAllFoundationsComplete() {
    for( unsigned int i = 0; i < Foundation::AMOUNT; ++i ) {
        if( !mPPFoundations[i]->isComplete() ) {
            // Not win yet
            return false;
        }
    }

    return true;
}