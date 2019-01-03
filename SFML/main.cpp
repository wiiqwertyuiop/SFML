#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

int main() {

	const int fontSize = 39;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow Window;
	Window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Type");

	sf::Font font;
	if (!font.loadFromFile("LucidaSansRegular.ttf")) return 1;

	sf::String sentence;
	sf::Text text(sentence, font, fontSize);
	text.setFillColor(sf::Color::Blue);

	while (Window.isOpen()) {

		sf::Event Event;

		while (Window.pollEvent(Event)) {

			switch (Event.type)
			{
			case sf::Event::Closed:
				Window.close();
				break;
			case sf::Event::KeyPressed:

				if (Event.key.code == sf::Keyboard::Escape) Window.close();
				else if (Event.key.code == sf::Keyboard::Enter) sentence += "\n";
				break;

			case sf::Event::TextEntered:

				if (Event.text.unicode >= 32 && Event.text.unicode <= 126) {
					
					sentence += (char)Event.text.unicode;
					text.setString(sentence);
					
					if (text.findCharacterPos(sentence.getSize()).x + (fontSize / 2) + 1 >= screenDimensions.x) sentence += "\n";

				} else if(Event.text.unicode == 8 && sentence.getSize() > 0) {
					
					sentence.erase(sentence.getSize()-1, 1);
					text.setString(sentence);
				
				}

				std::cout << (char)sentence[0];

				break;
			}
		}

		Window.draw(text);
		Window.display();
		Window.clear();
	}

	return 0;
}