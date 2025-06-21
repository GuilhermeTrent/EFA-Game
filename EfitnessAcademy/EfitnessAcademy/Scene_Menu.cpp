#include "Scene_Menu.h"
//#include "Scene_Frogger.h"
#include "MusicPlayer.h"
#include <memory>

void Scene_Menu::onEnd()
{
	_game->window().close();
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}



void Scene_Menu::init()
{
	//MusicPlayer::getInstance().play("menuTheme");
	//MusicPlayer::getInstance().setVolume(5);

	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Enter, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "7   Pillars   of   Self";
	m_menuStrings.push_back("Press  Enter  to  Begin");

	m_levelPaths.push_back("../level1.txt");

	m_menuText.setFont(Assets::getInstance().getFont("Arcade"));

	const size_t CHAR_SIZE{ 84 };
	m_menuText.setCharacterSize(CHAR_SIZE);

}

void Scene_Menu::update(sf::Time dt)
{
	_entityManager.update();
}


void Scene_Menu::sRender()
{
	static const sf::Color backgroundColor(84, 146, 163);

	_game->window().clear(sf::Color(backgroundColor));

	sf::View view = _game->window().getView();
	view.setCenter(_game->window().getSize().x / 2.f, _game->window().getSize().y / 2.f);
	_game->window().setView(view);

	//static const sf::Color selectedColor(255, 255, 255);
	static const sf::Color normalColor(0, 0, 0);


	//sf::Text footer("Press Enter to begin    QUIT: ESC",
	//	Assets::getInstance().getFont("Arial"), 20);
	//footer.setFillColor(normalColor);
	//footer.setPosition(32, 700);


	m_menuText.setFillColor(normalColor);
	m_menuText.setString(m_title);
	m_menuText.setPosition(475, 10);
	_game->window().draw(m_menuText);

	for (size_t i{ 0 }; i < m_menuStrings.size(); ++i)
	{
		//m_menuText.setFillColor((i == m_menuIndex ? selectedColor : normalColor));
		m_menuText.setPosition(32, 32 + (i + 1) * 96);
		m_menuText.setString(m_menuStrings.at(i));
		_game->window().draw(m_menuText);
	}

	//_game->window().draw(footer);

}


void Scene_Menu::sDoAction(const Command& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			m_menuIndex = (m_menuIndex + m_menuStrings.size() - 1) % m_menuStrings.size();
		}
		else if (action.name() == "DOWN")
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
		}
		else if (action.name() == "PLAY")
		{
			_game->changeScene("PLAY", std::make_shared<Scene_Game>(_game, m_levelPaths[m_menuIndex]));
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}

}
