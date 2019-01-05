#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

/*

 - TODO -

Word wrap:

If we delete a line:
	loop to see if any characers are off screen

If a character is off screen loop back until you find a space or a new line character

	If you find a space insert a new line
	If you find a newline character end search and just break up the word to make it fit

*/

int main() {

	const int fontSize = 39;
	const int crusorBlinkTime = 500; // Crusor blink time in Milliseconds

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow Window;
	Window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Type");

	sf::Font font;
	if (!font.loadFromFile("lucon.ttf")) return 1;

	sf::String sentence;
	sf::Text text(sentence, font, fontSize);
	text.setFillColor(sf::Color::Blue);

	int cursorTracker = 0;

	// Constructor
	sf::RectangleShape cursor(sf::Vector2f(fontSize/2, 5.f));	// Set cursor width and thickness
	sf::Vector2f cursorPos(fontSize / 10, fontSize + 3);	// set cursor starting position	

	sf::Clock clock;

	while (Window.isOpen()) {

		sf::Event Event;

		while (Window.pollEvent(Event)) {

			switch (Event.type)
			{
			
			case sf::Event::Closed:
				Window.close();
				break;

			case sf::Event::KeyPressed:
				
				clock.restart(); // Don't cause cursor to blink if we are typing

				if (Event.key.code == sf::Keyboard::Escape) Window.close();
				else if (Event.key.code == sf::Keyboard::Enter) {
					
					// Position text cursor on new line
					cursorPos.x = fontSize / 10;
					cursorPos.y += fontSize;

					// Insert new line
					sentence.insert(cursorTracker, '\n');
					text.setString(sentence);
					cursorTracker++;

				}
				else if (Event.key.code == sf::Keyboard::BackSpace && sentence.getSize() > 0 && cursorTracker != 0) {

					// Delete character before cursor position
					int deletePos = cursorTracker - 1;

					if (text.findCharacterPos(deletePos).x + (fontSize / 2) >= screenDimensions.x) {
						// If we backspace and we end up off screen delete the new line character as well as the other character and loop us around
						sentence.erase(deletePos, 1);
						//text.setString(sentence);
						cursorTracker--;
						cursorPos.y -= fontSize;
						//break;
					}
					// If we are at the beginning of a new line or there is a new line charcter before us wrap around
					else if (cursorPos.x == fontSize / 10 || sentence[deletePos] == '\n') cursorPos.y -= fontSize;

					// Set the cursor's X position to the character's that will be erased
					cursorPos.x = text.findCharacterPos(deletePos).x;

					sentence.erase(deletePos, 1);
					text.setString(sentence);

					cursorTracker--;

					// If we are on an empty line, position the cursor at the begining of the line
					if (cursorTracker != 0 && sentence.getSize() > 0 && sentence[deletePos] == '\n') cursorPos.x = fontSize / 10;

				}
				else if (Event.key.code == sf::Keyboard::Left && cursorTracker > 0) {

				tryBackAgain:

					cursorTracker--;

					if (sentence[cursorTracker] == '\n') {

						cursorPos.y -= fontSize; // Move cursor up

						// If we are on blank line put cursor at the beginning of it
						if (sentence[cursorTracker - 1] == '\n') {
							cursorPos.x = fontSize / 10;
							break;
						}
					}

					// Set cursor at the position of the character
					cursorPos.x = text.findCharacterPos(cursorTracker).x + fontSize / 10;

					// If we go back and are off screen try the letter before it
					if (cursorPos.x + (fontSize / 2) >= screenDimensions.x) goto tryBackAgain; 

				}
				else if (Event.key.code == sf::Keyboard::Right && cursorTracker < sentence.getSize()) {

					// If character is a line break...
					if (sentence[cursorTracker] == '\n') {

						// Move cursor down one and put it at the beginning of the line
						cursorPos.y += fontSize;
						cursorPos.x = fontSize / 10;
						cursorTracker++;
						break;
					}

				tryForwardAgain:
					cursorTracker++;
					cursorPos.x = text.findCharacterPos(cursorTracker).x + fontSize / 10;

					// If we move forward and are off screen, skip this character and move on to the next one
					if (cursorPos.x + (fontSize / 2) >= screenDimensions.x) {
						cursorPos.y += fontSize;
						goto tryForwardAgain;
					}

				}
				break;

			case sf::Event::TextEntered:

				if (Event.text.unicode >= 32 && Event.text.unicode <= 126) {
					
					// Add new character to sentence
					sentence.insert(cursorTracker, (char)Event.text.unicode);
					text.setString(sentence);

					// Increase crusor pos by width of the character
					cursorPos.x += text.findCharacterPos(sentence.getSize()).x - text.findCharacterPos(sentence.getSize()-1).x;

					// If the cursor is off screen start a new line
					if (cursorPos.x + (fontSize / 2) >= screenDimensions.x) { 
						sentence += "\n";
						cursorTracker++;
						text.setString(sentence);
					}

					cursorTracker++;

				} 
				break;

			}

		}

		// Update cursor
		if (cursorPos.x + (fontSize / 2) >= screenDimensions.x) {

			// Correct position if we are off screen
			cursorPos.x = fontSize / 10;
			cursorPos.y += fontSize;
		}

		cursor.setPosition(cursorPos); // Update position

		sf::Int32 time = clock.getElapsedTime().asMilliseconds();

		// Make crusor blink
		if (time < crusorBlinkTime) Window.draw(cursor);
		else if (time >= crusorBlinkTime+200) clock.restart();

		Window.draw(text);
		Window.display();
		Window.clear();
	}

	return 0;
}
