#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>

class SevenPillarsGame {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText, instructionText, buttonText;
    sf::RectangleShape background, pillarBase, glowEffect;
    sf::CircleShape breathCircle, pillarGlow;

    enum GameState {
        WELCOME,
        MAIN_MENU,
        EMOTIONAL,
        PURPOSE,
        FINANCIAL,
        PHYSICAL,
        MENTAL,
        ENVIRONMENTAL,
        SPIRITUAL,
        COMPLETION
    };

    GameState currentState;
    std::vector<bool> pillarsActivated;
    std::vector<sf::Color> pillarColors;
    std::vector<std::string> pillarNames;

    // Animation variables
    float breathSize;
    float breathDirection;
    float glowAlpha;
    float glowDirection;
    sf::Clock animationClock;

    // Interaction variables
    int selectedOption;
    bool optionSelected;
    float treeGrowth;
    bool soundPlaying;

    // Questions and content
    std::vector<std::string> emotionalOptions = {
        "I feel energized and want to share positivity",
        "I feel calm and want to reflect quietly",
        "I feel challenged and want to grow stronger",
        "I feel grateful and want to appreciate life"
    };

    std::vector<std::string> purposeQuotes = {
        "\"Your purpose is to make a difference in the world\" - Unknown",
        "\"Success is not final, failure is not fatal\" - Churchill",
        "\"The best way to find yourself is to lose yourself in service\" - Gandhi",
        "\"Life is what happens when you're busy making plans\" - Lennon"
    };

    std::vector<std::string> financialOptions = {
        "Save money for future goals",
        "Invest in personal development",
        "Share resources with others in need"
    };

    std::vector<std::string> mentalOptions = {
        "\"I can learn from this challenge and grow stronger\"",
        "\"This difficult moment will pass, and I will adapt\"",
        "\"I have the inner wisdom to navigate this situation\"",
        "\"Every experience teaches me something valuable\""
    };

public:
    SevenPillarsGame() : window(sf::VideoMode(1200, 800), "The 7 Pillars of Self"),
        currentState(WELCOME),
        pillarsActivated(7, false),
        breathSize(50.0f), breathDirection(1.0f),
        glowAlpha(100.0f), glowDirection(1.0f),
        selectedOption(-1), optionSelected(false),
        treeGrowth(0.0f), soundPlaying(false) {

        // Initialize pillar colors
        pillarColors = {
            sf::Color(255, 100, 100), // Emotional - Red
            sf::Color(255, 200, 100), // Purpose - Orange
            sf::Color(100, 255, 100), // Financial - Green
            sf::Color(100, 200, 255), // Physical - Blue
            sf::Color(200, 100, 255), // Mental - Purple
            sf::Color(150, 255, 150), // Environmental - Light Green
            sf::Color(255, 255, 150)  // Spiritual - Yellow
        };

        pillarNames = {
            "Emotional", "Purpose", "Financial", "Physical",
            "Mental", "Environmental", "Spiritual"
        };

        // Setup basic shapes and text
        setupGraphics();
    }

    void setupGraphics() {
        // Create a simple font (you would load a real font file)
        // For this example, we'll use the default font

        // Setup background
        background.setSize(sf::Vector2f(1200, 800));
        background.setFillColor(sf::Color(20, 30, 50));

        // Setup title text
        titleText.setFont(font);
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setString("The 7 Pillars of Self");
        titleText.setPosition(300, 100);

        // Setup instruction text
        instructionText.setFont(font);
        instructionText.setCharacterSize(24);
        instructionText.setFillColor(sf::Color::White);

        // Setup button text
        buttonText.setFont(font);
        buttonText.setCharacterSize(20);
        buttonText.setFillColor(sf::Color::White);

        // Setup pillar base
        pillarBase.setSize(sf::Vector2f(100, 150));
        pillarBase.setFillColor(sf::Color(100, 100, 100));

        // Setup glow effect
        glowEffect.setSize(sf::Vector2f(120, 170));
        glowEffect.setFillColor(sf::Color(255, 255, 255, 50));

        // Setup breath circle
        breathCircle.setRadius(50);
        breathCircle.setFillColor(sf::Color(100, 150, 255, 100));
        breathCircle.setPosition(550, 350);

        // Setup pillar glow
        pillarGlow.setRadius(60);
        pillarGlow.setFillColor(sf::Color(255, 255, 255, 100));
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        switch (currentState) {
        case WELCOME:
            if (key == sf::Keyboard::Enter) {
                currentState = MAIN_MENU;
            }
            break;

        case MAIN_MENU:
            if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num7) {
                int pillarIndex = key - sf::Keyboard::Num1;
                currentState = static_cast<GameState>(EMOTIONAL + pillarIndex);
                selectedOption = -1;
                optionSelected = false;
            }
            break;

        case EMOTIONAL:
        case PURPOSE:
        case FINANCIAL:
        case MENTAL:
            if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num4) {
                selectedOption = key - sf::Keyboard::Num1;
                optionSelected = true;
            }
            if (key == sf::Keyboard::Enter && optionSelected) {
                completePillar();
            }
            break;

        case PHYSICAL:
            // Breathing exercise - any key to complete
            if (key == sf::Keyboard::Space) {
                completePillar();
            }
            break;

        case ENVIRONMENTAL:
            if (key == sf::Keyboard::Space) {
                treeGrowth = std::min(treeGrowth + 0.1f, 1.0f);
                if (treeGrowth >= 1.0f) {
                    completePillar();
                }
            }
            break;

        case SPIRITUAL:
            if (key == sf::Keyboard::Space) {
                completePillar();
            }
            break;

        case COMPLETION:
            if (key == sf::Keyboard::Enter) {
                // Reset game or return to menu
                resetGame();
            }
            break;
        }

        // Always allow ESC to return to main menu
        if (key == sf::Keyboard::Escape && currentState != WELCOME) {
            currentState = MAIN_MENU;
        }
    }

    void handleMouseClick(int x, int y) {
        // Handle mouse clicks for specific interactions
        if (currentState == ENVIRONMENTAL) {
            // Click to plant tree
            if (x >= 500 && x <= 700 && y >= 400 && y <= 600) {
                treeGrowth = std::min(treeGrowth + 0.2f, 1.0f);
                if (treeGrowth >= 1.0f) {
                    completePillar();
                }
            }
        }
    }

    void update() {
        float deltaTime = animationClock.restart().asSeconds();

        // Update animations
        updateBreathAnimation(deltaTime);
        updateGlowAnimation(deltaTime);

        // Check if all pillars are completed
        bool allCompleted = true;
        for (bool activated : pillarsActivated) {
            if (!activated) {
                allCompleted = false;
                break;
            }
        }

        if (allCompleted && currentState != COMPLETION) {
            currentState = COMPLETION;
        }
    }

    void updateBreathAnimation(float deltaTime) {
        breathSize += breathDirection * 30.0f * deltaTime;
        if (breathSize > 80.0f) {
            breathSize = 80.0f;
            breathDirection = -1.0f;
        }
        else if (breathSize < 30.0f) {
            breathSize = 30.0f;
            breathDirection = 1.0f;
        }
        breathCircle.setRadius(breathSize);
        breathCircle.setOrigin(breathSize, breathSize);
    }

    void updateGlowAnimation(float deltaTime) {
        glowAlpha += glowDirection * 50.0f * deltaTime;
        if (glowAlpha > 150.0f) {
            glowAlpha = 150.0f;
            glowDirection = -1.0f;
        }
        else if (glowAlpha < 50.0f) {
            glowAlpha = 50.0f;
            glowDirection = 1.0f;
        }
    }

    void completePillar() {
        int pillarIndex = currentState - EMOTIONAL;
        if (pillarIndex >= 0 && pillarIndex < 7) {
            pillarsActivated[pillarIndex] = true;
        }
        currentState = MAIN_MENU;
    }

    void resetGame() {
        std::fill(pillarsActivated.begin(), pillarsActivated.end(), false);
        currentState = MAIN_MENU;
        selectedOption = -1;
        optionSelected = false;
        treeGrowth = 0.0f;
    }

    void render() {
        window.clear();
        window.draw(background);

        switch (currentState) {
        case WELCOME:
            renderWelcomeScreen();
            break;
        case MAIN_MENU:
            renderMainMenu();
            break;
        case EMOTIONAL:
            renderEmotionalPillar();
            break;
        case PURPOSE:
            renderPurposePillar();
            break;
        case FINANCIAL:
            renderFinancialPillar();
            break;
        case PHYSICAL:
            renderPhysicalPillar();
            break;
        case MENTAL:
            renderMentalPillar();
            break;
        case ENVIRONMENTAL:
            renderEnvironmentalPillar();
            break;
        case SPIRITUAL:
            renderSpiritualPillar();
            break;
        case COMPLETION:
            renderCompletionScreen();
            break;
        }

        window.display();
    }

    void renderWelcomeScreen() {
        titleText.setString("The 7 Pillars of Self");
        titleText.setPosition(250, 200);
        window.draw(titleText);

        instructionText.setString("A Journey of Self-Discovery and Growth");
        instructionText.setPosition(350, 300);
        window.draw(instructionText);

        instructionText.setString("Based on Indigenous Wisdom and Emotional Fitness");
        instructionText.setPosition(280, 350);
        window.draw(instructionText);

        instructionText.setString("Press ENTER to begin your journey");
        instructionText.setPosition(400, 450);
        window.draw(instructionText);

        instructionText.setString("Emotional Fitness Academy");
        instructionText.setCharacterSize(18);
        instructionText.setPosition(500, 650);
        window.draw(instructionText);
        instructionText.setCharacterSize(24);
    }

    void renderMainMenu() {
        titleText.setString("Choose a Pillar to Explore");
        titleText.setPosition(300, 100);
        window.draw(titleText);

        // Draw pillars in a circle
        float centerX = 600;
        float centerY = 400;
        float radius = 200;

        for (int i = 0; i < 7; i++) {
            float angle = (i * 2 * M_PI) / 7 - M_PI / 2;
            float x = centerX + radius * cos(angle) - 50;
            float y = centerY + radius * sin(angle) - 75;

            // Draw pillar base
            pillarBase.setPosition(x, y);
            if (pillarsActivated[i]) {
                pillarBase.setFillColor(pillarColors[i]);
                // Draw glow effect
                sf::Color glowColor = pillarColors[i];
                glowColor.a = static_cast<sf::Uint8>(glowAlpha);
                pillarGlow.setFillColor(glowColor);
                pillarGlow.setPosition(x - 10, y - 10);
                window.draw(pillarGlow);
            }
            else {
                pillarBase.setFillColor(sf::Color(100, 100, 100));
            }
            window.draw(pillarBase);

            // Draw pillar name
            buttonText.setString(std::to_string(i + 1) + ". " + pillarNames[i]);
            buttonText.setPosition(x - 20, y + 160);
            window.draw(buttonText);
        }

        instructionText.setString("Press 1-7 to explore each pillar");
        instructionText.setPosition(450, 650);
        window.draw(instructionText);

        // Show progress
        int completed = 0;
        for (bool activated : pillarsActivated) {
            if (activated) completed++;
        }
        instructionText.setString("Progress: " + std::to_string(completed) + "/7 pillars activated");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderEmotionalPillar() {
        titleText.setString("Emotional Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("How do you feel today and what do you want to do with that emotion?");
        instructionText.setPosition(200, 200);
        window.draw(instructionText);

        for (int i = 0; i < emotionalOptions.size(); i++) {
            sf::Color textColor = (selectedOption == i) ? sf::Color::Yellow : sf::Color::White;
            buttonText.setFillColor(textColor);
            buttonText.setString(std::to_string(i + 1) + ". " + emotionalOptions[i]);
            buttonText.setPosition(150, 280 + i * 40);
            window.draw(buttonText);
        }
        buttonText.setFillColor(sf::Color::White);

        if (optionSelected) {
            instructionText.setString("Press ENTER to activate the Emotional Pillar");
            instructionText.setPosition(400, 600);
            window.draw(instructionText);
        }

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderPurposePillar() {
        titleText.setString("Purpose Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("Which quote inspires you the most?");
        instructionText.setPosition(400, 200);
        window.draw(instructionText);

        for (int i = 0; i < purposeQuotes.size(); i++) {
            sf::Color textColor = (selectedOption == i) ? sf::Color::Yellow : sf::Color::White;
            buttonText.setFillColor(textColor);
            buttonText.setString(std::to_string(i + 1) + ". " + purposeQuotes[i]);
            buttonText.setPosition(100, 280 + i * 50);
            window.draw(buttonText);
        }
        buttonText.setFillColor(sf::Color::White);

        if (optionSelected) {
            instructionText.setString("Press ENTER to activate the Purpose Pillar");
            instructionText.setPosition(400, 600);
            window.draw(instructionText);
        }

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderFinancialPillar() {
        titleText.setString("Financial Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("Choose a wise financial action:");
        instructionText.setPosition(400, 200);
        window.draw(instructionText);

        for (int i = 0; i < financialOptions.size(); i++) {
            sf::Color textColor = (selectedOption == i) ? sf::Color::Yellow : sf::Color::White;
            buttonText.setFillColor(textColor);
            buttonText.setString(std::to_string(i + 1) + ". " + financialOptions[i]);
            buttonText.setPosition(350, 280 + i * 40);
            window.draw(buttonText);
        }
        buttonText.setFillColor(sf::Color::White);

        if (optionSelected) {
            instructionText.setString("Press ENTER to activate the Financial Pillar");
            instructionText.setPosition(400, 600);
            window.draw(instructionText);
        }

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderPhysicalPillar() {
        titleText.setString("Physical Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("Follow the guided breathing exercise");
        instructionText.setPosition(400, 200);
        window.draw(instructionText);

        instructionText.setString("Watch the circle expand and contract");
        instructionText.setPosition(400, 250);
        window.draw(instructionText);

        instructionText.setString("Breathe in as it grows, breathe out as it shrinks");
        instructionText.setPosition(350, 300);
        window.draw(instructionText);

        // Draw animated breathing circle
        window.draw(breathCircle);

        instructionText.setString("Press SPACE when you feel centered");
        instructionText.setPosition(400, 600);
        window.draw(instructionText);

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderMentalPillar() {
        titleText.setString("Mental Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("Choose the most helpful thought:");
        instructionText.setPosition(400, 200);
        window.draw(instructionText);

        for (int i = 0; i < mentalOptions.size(); i++) {
            sf::Color textColor = (selectedOption == i) ? sf::Color::Yellow : sf::Color::White;
            buttonText.setFillColor(textColor);
            buttonText.setString(std::to_string(i + 1) + ". " + mentalOptions[i]);
            buttonText.setPosition(100, 280 + i * 50);
            window.draw(buttonText);
        }
        buttonText.setFillColor(sf::Color::White);

        if (optionSelected) {
            instructionText.setString("Press ENTER to activate the Mental Pillar");
            instructionText.setPosition(400, 600);
            window.draw(instructionText);
        }

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderEnvironmentalPillar() {
        titleText.setString("Environmental Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("Plant a tree to connect with Mother Earth");
        instructionText.setPosition(400, 200);
        window.draw(instructionText);

        // Draw growing tree
        sf::RectangleShape trunk(sf::Vector2f(20, 100 * treeGrowth));
        trunk.setFillColor(sf::Color(101, 67, 33));
        trunk.setPosition(590, 500 - 100 * treeGrowth);
        window.draw(trunk);

        if (treeGrowth > 0.3f) {
            sf::CircleShape leaves(30 * treeGrowth);
            leaves.setFillColor(sf::Color(34, 139, 34));
            leaves.setPosition(570, 420 - 80 * treeGrowth);
            window.draw(leaves);
        }

        instructionText.setString("Click on the tree area or press SPACE to help it grow");
        instructionText.setPosition(350, 550);
        window.draw(instructionText);

        if (treeGrowth >= 1.0f) {
            instructionText.setString("Beautiful! The Environmental Pillar is activated!");
            instructionText.setPosition(350, 600);
            window.draw(instructionText);
        }

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderSpiritualPillar() {
        titleText.setString("Spiritual Pillar");
        titleText.setPosition(450, 100);
        window.draw(titleText);

        instructionText.setString("Take a moment for quiet reflection");
        instructionText.setPosition(400, 200);
        window.draw(instructionText);

        instructionText.setString("Listen to the silence within");
        instructionText.setPosition(400, 250);
        window.draw(instructionText);

        // Draw meditation silhouette
        sf::CircleShape head(40);
        head.setFillColor(sf::Color(50, 50, 50));
        head.setPosition(560, 350);
        window.draw(head);

        sf::RectangleShape body(sf::Vector2f(80, 100));
        body.setFillColor(sf::Color(50, 50, 50));
        body.setPosition(560, 430);
        window.draw(body);

        // Draw aura
        sf::CircleShape aura(120);
        aura.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(glowAlpha / 3)));
        aura.setPosition(480, 320);
        window.draw(aura);

        instructionText.setString("Press SPACE when you feel a sense of calm");
        instructionText.setPosition(400, 600);
        window.draw(instructionText);

        instructionText.setString("Press ESC to return to main menu");
        instructionText.setPosition(450, 700);
        window.draw(instructionText);
    }

    void renderCompletionScreen() {
        titleText.setString("Congratulations!");
        titleText.setPosition(400, 150);
        window.draw(titleText);

        instructionText.setString("You have activated all 7 Pillars of Self!");
        instructionText.setPosition(350, 250);
        window.draw(instructionText);

        instructionText.setString("Your foundation is now strong and balanced.");
        instructionText.setPosition(350, 300);
        window.draw(instructionText);

        instructionText.setString("Remember: True transformation comes from within.");
        instructionText.setPosition(320, 350);
        window.draw(instructionText);

        instructionText.setString("Your innate wisdom is your greatest tool.");
        instructionText.setPosition(350, 400);
        window.draw(instructionText);

        instructionText.setString("Continue your journey with the Emotional Fitness Academy");
        instructionText.setPosition(250, 500);
        window.draw(instructionText);

        instructionText.setString("Visit: https://efitacademy.ca/");
        instructionText.setPosition(450, 550);
        window.draw(instructionText);

        instructionText.setString("Press ENTER to start a new journey");
        instructionText.setPosition(400, 650);
        window.draw(instructionText);
    }
};

int main() {
    SevenPillarsGame game;
    game.run();
    return 0;
}