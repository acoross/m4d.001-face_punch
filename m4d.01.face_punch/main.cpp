#include <SFML/Graphics.hpp>

int main(int argc, char** argv[])
{
	sf::RenderWindow window(sf::VideoMode(640, 480),
		"Rendering the rectangle.");

	sf::Texture texture;
	if (!texture.loadFromFile(".\\knight.png"))
	{
		// Handle an error.
		window.close();
	}
	
	const float scale = 0.08f;
	const auto sizeU = texture.getSize();
	const sf::Vector2f size(sizeU.x * scale, sizeU.y * scale);

	sf::Sprite knight(texture);
	knight.setScale(scale, scale);
	knight.setOrigin(size);

	sf::Vector2f increment(0.4f, 0.4f);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// Close window button clicked.
				window.close();
			}
		}

		const auto position = knight.getPosition();
		const auto windowSize = window.getSize();
		
		if ((position.x + size.x / 2 > windowSize.x && increment.x > 0) || 
			(position.x - size.x / 2 < 0 && increment.x < 0))
		{
			increment.x = -increment.x;
		}

		if ((position.y + size.y / 2 > windowSize.y && increment.y > 0) ||
			(position.y - size.y / 2 < 0 && increment.y < 0))
		{
			increment.y = -increment.y;
		}

		knight.setPosition(position + increment);

		window.clear(sf::Color::Black);
		window.draw(knight);
		window.display();
	}

	return 0;
}