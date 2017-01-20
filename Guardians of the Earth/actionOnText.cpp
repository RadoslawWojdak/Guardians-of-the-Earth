#include "actionOnText.h"

sf::String keyToStr(sf::Keyboard::Key key)
{
	if (key >= 0 && key <= 25)	//A...Z
		return char(key + 65);
	else if (key >= 26 && key <= 35)
	{
		std::string key_str = "Num";
		key_str += (char)(key - 26 + 48);
		return key_str;
	}
	else if (key >= 75 && key <= 84)
	{
		std::string key_str = "Numpad";
		key_str += (char)(key - 75 + 48);
		return key_str;
	}
	else if (key >= 85 && key <= 99)
	{
		std::string key_str = "F";
		if (key - 84 + 48 >= 58)
		{
			key_str += '1';
			key_str += (char)(key - 84 + 48 - 10);
		}
		else
			key_str += (char)(key - 84 + 48);
		return key_str;
	}
	
	switch (key)
	{
	case sf::Keyboard::Escape: return "Escape";
	case sf::Keyboard::LControl: return "LControl";
	case sf::Keyboard::LShift: return "LShift";
	case sf::Keyboard::LAlt: return "LAlt";
	case sf::Keyboard::LSystem: return "LSystem";
	case sf::Keyboard::RControl: return "RControl";
	case sf::Keyboard::RShift: return "RShift";
	case sf::Keyboard::RAlt: return "RAlt";
	case sf::Keyboard::RSystem: return "RSystem";
	case sf::Keyboard::Menu: return "Menu";
	case sf::Keyboard::LBracket: return "[";
	case sf::Keyboard::RBracket: return "]";
	case sf::Keyboard::SemiColon: return ";";
	case sf::Keyboard::Comma: return ",";
	case sf::Keyboard::Period: return ".";
	case sf::Keyboard::Quote: return "'";
	case sf::Keyboard::Slash: return "/";
	case sf::Keyboard::BackSlash: return "\\";
	case sf::Keyboard::Tilde: return "~";
	case sf::Keyboard::Equal: return "=";
	case sf::Keyboard::Dash: return "-";
	case sf::Keyboard::Space: return "Space";
	case sf::Keyboard::Return: return "Return";
	case sf::Keyboard::BackSpace: return "BackSpace";
	case sf::Keyboard::Tab: return "Tab";
	case sf::Keyboard::PageUp: return "PageUp";
	case sf::Keyboard::PageDown: return "PageDown";
	case sf::Keyboard::End: return "End";
	case sf::Keyboard::Home: return "Home";
	case sf::Keyboard::Insert: return "Insert";
	case sf::Keyboard::Delete: return "Delete";
	case sf::Keyboard::Add: return "+";
	case sf::Keyboard::Subtract: return "-";
	case sf::Keyboard::Multiply: return "*";
	case sf::Keyboard::Divide: return "/";
	case sf::Keyboard::Left: return "Left";
	case sf::Keyboard::Right: return "Right";
	case sf::Keyboard::Up: return "Up";
	case sf::Keyboard::Down: return "Down";
	case sf::Keyboard::Pause: return "Pause";
	case sf::Keyboard::KeyCount: return "KeyCount";
	}

	return "Unknown";
}