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
 * the the functionality of the "Deck" game element, which is where new Cards are
 * drawn from in Solitaire.
 */

#include "Deck.h"

using namespace sf;
using namespace std;

#include "CardRank.h"
#include "CardSuit.h"

#include <cstdlib>

Deck::Deck() {
    const unsigned int DECK_SIZE = (CardSuit::DIAMONDS + 1) * (CardRank::KING + 1);

    // Initially create an ordered array for the deck. This allows the
    // shuffling  process to be much more efficient compared to immediately
    // using a linked list
    Card** initDeck = new Card*[DECK_SIZE];
    for( unsigned int i = 0; i < DECK_SIZE; ++i ) {
        initDeck[i] = new Card( 
            (CardSuit) (i % (CardSuit::DIAMONDS + 1)), 
            (CardRank) (i % (CardRank::KING + 1)) 
        );
    }

    // Shuffle the deck SIZE * 2 times to ensure a high level of randomness
    for( unsigned int i = 0; i < DECK_SIZE * 2; ++i ) {
        const int FROM = rand() % DECK_SIZE;
        const int TO = rand() % DECK_SIZE;
        Card* pCard = initDeck[FROM];
        initDeck[FROM] = initDeck[TO];
        initDeck[TO] = pCard;
    }

    // Convert the card array to a linked list to be used in this instance
    for( unsigned int i = 0; i < DECK_SIZE; ++i ) {
    	mHiddenCards.push_back( *initDeck[i] );
    	delete initDeck[i];
    }

    delete[] initDeck;

    // The deck consists of two elements, one for cards that have not been
    // drawn and cards that are drawn. These are placed placed in the top
    // left corner
    mHiddenBounds = IntRect( 16, 16, Card::WIDTH, Card::HEIGHT );
    mRevealedBounds = IntRect( 32 + Card::WIDTH, 16, Card::WIDTH, Card::HEIGHT );
}

Card Deck::pull() {
    // Simply take the top hidden card. We assume that this function is
    // called by other code and not triggered by any user input, thus
    // validation is not needed
    Card card = mHiddenCards.back();
    mHiddenCards.pop_back();
    return card;
}

bool Deck::click( const Vector2i MOUSE_POS, Selection*& pSelection ) {
    if( mHiddenBounds.contains( MOUSE_POS ) ) {
        // Clicked the hidden portion of the deck, draw a card
        if( mHiddenCards.size() > 0 ) {
            // Moving a card from the *back* of the hidden cards to the *front*
            // of the revealed cards allows the ordering of the whole deck to
            // be preserved
            mRevealedCards.push_front( mHiddenCards.back() );
            mHiddenCards.pop_back();
        } else {
            // Swap the two card lists to replenish the hidden cards
            mHiddenCards = mRevealedCards;
            mRevealedCards = list<Card>();
        }

        // Drawing is considered a move
        return true; 
    }

    if( mRevealedBounds.contains( MOUSE_POS ) && mRevealedCards.size() > 0 ) {
        // Clicked the revealed portion of the deck, select the top revealed card
        list<Card> cards;
        cards.splice( 
            cards.begin(), 
            mRevealedCards, mRevealedCards.begin(), ++mRevealedCards.begin() 
        );

        pSelection = new Selection( 
            0,
            Vector2i( mRevealedBounds.left, mRevealedBounds.top ), 
            MOUSE_POS,
            cards
        );
    }

    return false;
}

void Deck::release( Selection*& pSelection ) {
    if( !pSelection->isConsumed() ) {
        // Not consumed, return to deck
        pSelection->consume( mRevealedCards, mRevealedCards.begin() );
    }

    delete pSelection;
    pSelection = nullptr;    
}

void Deck::draw( RenderTarget& target, RenderStates states ) const {
    VertexArray vertices( Quads, 4 );

    // Always show a placeholder for the hidden portion of the deck
    Card hidden( mHiddenCards.size() > 0 );

    hidden.intoVertices( vertices, sf::Vector2i( mHiddenBounds.left, mHiddenBounds.top ) );
    target.draw( vertices, states );

    // Show the top (front) shown card if available
    Card top;
    if( mRevealedCards.size() > 0 ) {
        top = mRevealedCards.front();
    } else {
        top = Card( false );
    }

    top.intoVertices( vertices, Vector2i( mRevealedBounds.left, mRevealedBounds.top ) );
    target.draw( vertices, states );
}