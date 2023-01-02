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
 * interface of the card data structure.
 */

#ifndef CARD_H
#define CARD_H

#include <SFML/Graphics.hpp>

#include "CardRank.h"
#include "CardSuit.h"

/**
 * @brief A playing card. Can be a data object to use in other
 * game elements, or a decorative element. Cards are not game
 * elements on their own, and must be loaded into a vertex array
 * with an externally provided position in order to be drawn
 */
class Card final {
public:
	/**
	 * @brief The width of a card when displayed, in pixels
	 */
	static const unsigned int WIDTH = 64;

	/**
	 * @brief The height of a card when displayed, in pixels
	 */
	static const unsigned int HEIGHT = 96;

	/**
	 * @brief Construct a "face down" card with no suit or rank,
	 * equivelent to Card( true ). Solely for decorative purposes
	 */
	Card();

	/**
	 * @brief Create a card with no suit or rank. Solely for
	 * decorative purposes
	 * @param FACE_DOWN Whether this card is face down.
	 * If false, the card will be displayed as a placeholder
	 */
	Card( const bool HIDDEN );

	/**
	 * @brief Create a card with a suit, but no rank. The card
	 * will be displayed as a placeholder, but with the iconography
	 * of the given suit. Solely for decorative purposes
	 * @param SUIT The suit desired
	 */
	Card( const CardSuit SUIT );

	/**
	 * @brief Create a card with a suit and rank.
	 * @param SUIT The suit desired
	 * @param RANK The rank desired
	 */
	Card( const CardSuit SUIT, const CardRank RANK );

	/**
	 * @brief Get the suit of this card 
	 * @return The suit of the card. If the card does not have
	 * a suit, the behavior is undefined
	 */
	CardSuit getSuit() const;

	/**
	 * @brief Get the rank of this card
	 * @return The rank of the card. If the card does not have
	 * a rank, the behavior is undefined
	 */
	CardRank getRank() const;

	/**
	 * @brief Get the color of this card
	 * @return true if the card is red, false if the card is black.
	 * If the card does not have a suit, the behavior is undefined
	 */
	bool getColor() const;

	/**
	 * @brief Modify the given vertices to have the bounds and texture
	 * of the card at the given position
	 * @param vertices The vertices to modify
	 * @param POS The position the card should be at
	 */
	void intoVertices( sf::VertexArray& vertices, const sf::Vector2i POS ) const;

private:
	CardSuit mSuit;
	CardRank mRank;
};

#endif