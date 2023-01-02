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
 * functionality of the card data structure.
 */

#include "Card.h"

#include "CardSuit.h"
#include "CardRank.h"

using namespace sf;

Card::Card() : Card::Card( true ) {
    // Aliased to hidden card
}

Card::Card( const CardSuit SUIT, const CardRank RANK ) {
    mSuit = SUIT;
    mRank = RANK;
}

Card::Card( const CardSuit SUIT ) {
    mSuit = SUIT;

    // Overload the rank so that the calculated texture will be
    // at the empty placeholder portion of the spritesheet.
    // This is extremeley stupid, but I did this before I learned
    // that composition would be more suitable for this task
    mRank = (CardRank)(CardRank::KING + 1);
}

Card::Card( const bool HIDDEN ) {
    if( HIDDEN ) {
        mSuit = CardSuit::CLUBS;
    } else {
        mSuit = CardSuit::SPADES;
    }

    // Overload the rank so that the calculated texture will be 
    // at the face down/empty placeholder portion of the spritesheet.
    // This is extremeley stupid, but I did this before I learned
    // that composition would be more suitable for this task
    mRank = (CardRank)(CardRank::KING + 2);
}

CardRank Card::getRank() const {
    return mRank;
}

CardSuit Card::getSuit() const {
    return mSuit;
}

bool Card::getColor() const {
    // Suits are already organized by color where black suits are 
    // first. So, if the suit is higher than spades, we can confirm
    // that the card is red.
    return mSuit > CardSuit::SPADES;
}

void Card::intoVertices( VertexArray& vertices, const Vector2i POS ) const {
    // Instead of sprites or shapes, we use the lower-level vertex array
    // SFML API instead. This is because loading an individual texture
    // and sprite for each card is incredibly in-efficient, and so it
    // would be better to keep a single spritesheet texture in memory
    // and then crop it to a particular type of card when drawing them.
    // This is a job best suited for VertexArray compared to Sprite or
    // Shape.
    vertices[0].position = Vector2f( POS.x, POS.y );
    vertices[1].position = Vector2f( POS.x + WIDTH, POS.y );
    vertices[2].position = Vector2f( POS.x + WIDTH, POS.y + HEIGHT );
    vertices[3].position = Vector2f( POS.x, POS.y + HEIGHT );

    const Vector2f TEX_POS( mRank * WIDTH, mSuit * HEIGHT );
    vertices[0].texCoords = Vector2f( TEX_POS.x, TEX_POS.y );
    vertices[1].texCoords = Vector2f( TEX_POS.x + WIDTH, TEX_POS.y );
    vertices[2].texCoords = Vector2f( TEX_POS.x + WIDTH, TEX_POS.y + HEIGHT );
    vertices[3].texCoords = Vector2f( TEX_POS.x, TEX_POS.y + HEIGHT );
}
