#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

class Rocket {
public:
    std::pair<unsigned, unsigned> size{ 12,23 };
    std::pair<unsigned, unsigned> place;
    Clock clock;

    Rocket() :place((rand() * 1200) / RAND_MAX, (rand() * 600) / RAND_MAX) {}

    bool Booom()
    {
        if (this->size.first >= 100 && this->size.second >= 100)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void fall()
    {
        size.first *= 1.2;
        size.second *= 1.2;
    }
};

int main()
{
    srand(time(0));
    RenderWindow window(sf::VideoMode(1300, 700), "fallout");
    window.setPosition(Vector2i(250, 100));
    Image background_image;
    background_image.loadFromFile("Images/nebo.jpg");
    Texture t_nebo;
    t_nebo.loadFromImage(background_image);
    Sprite nebo(t_nebo);

    window.setMouseCursorVisible(false);
    Texture t_cur;
    t_cur.loadFromFile("Images/aim2.png");
    Sprite cur(t_cur);
    cur.setOrigin(1024, 1024);
    cur.setScale(0.05, 0.05);
    cur.setColor(Color(255, 0, 0));

    vector<Rocket> rockets(1);

    Music music;
    music.openFromFile("Music/prob.mp3");
    music.setLoop(true);
    music.play();

    Font font;
    font.loadFromFile("Fonts/for_game.ttf");
    Text score;
    score.setFont(font);
    score.setFillColor(Color(100, 100, 100));
    score.setPosition(15, 5);
    score.setScale(1.5, 1.5);
    Text start_game;
    start_game.setFont(font);
    start_game.setFillColor(Color(50, 50, 50));
    start_game.setPosition(550, 250);
    start_game.setScale(3, 3);
    start_game.setString("START\nGAME");
    char start = '0';

    int num_rockets = 50;

    Clock global_time;

    while (window.isOpen())
    {
        window.clear();

        //cur.setPosition(Mouse::getPosition().x - 424, Mouse::getPosition().y - 335);
        cur.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (start == '0')
        {
            window.draw(nebo);
            Text name;
            name.setFont(font);
            name.setFillColor(Color(5, 5, 5));
            name.setPosition(420, 25);
            name.setScale(6, 6);
            name.setString("FallOut");
            window.draw(name);
            window.draw(start_game);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                pair<unsigned, unsigned> mouse_place(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
                if (mouse_place.first >= start_game.getPosition().x && mouse_place.first <= 750
                    && mouse_place.second >= start_game.getPosition().y && mouse_place.second <= 400)
                {
                    start = '1';
                    global_time.restart();
                }
            }
        }
        else if ((num_rockets != 0 || rockets.size() != 0) && start == '1')
        {
            window.draw(nebo);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                pair<unsigned, unsigned> mouse_place(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
                for (int i = 0; i < rockets.size(); ++i)
                {
                    if (rockets[i].place.first <= mouse_place.first && rockets[i].place.second <= mouse_place.second
                        && (rockets[i].place.first + rockets[i].size.first) >= mouse_place.first && (rockets[i].place.second + rockets[i].size.second) >= mouse_place.second)
                    {
                        rockets.erase(rockets.begin() + i);
                        num_rockets--;
                        break;
                    }
                }
            }

            Texture t_rocket;
            t_rocket.loadFromFile("Images/missile00.png");
            Sprite rocket(t_rocket);
            rocket.setTextureRect(IntRect(10, 3, 12, 23));
            for (int i = 0; i < rockets.size(); ++i)
            {
                rocket.setScale(rockets[i].size.first / 12.0, rockets[i].size.second / 23.0);
                rocket.setPosition(rockets[i].place.first, rockets[i].place.second);
                window.draw(rocket);
                if (rockets[i].Booom())
                {
                    start = '-';
                    break;
                }
                if (rockets[i].clock.getElapsedTime() >= milliseconds(300))
                {
                    rockets[i].fall();
                    rockets[i].clock.restart();
                }
            }
            if (num_rockets - rockets.size() > 0 && global_time.getElapsedTime() >= milliseconds(700))
            {
                rockets.push_back(Rocket());
                global_time.restart();
            }

            string for_score = "Rockets last: " + to_string(num_rockets);
            score.setString(for_score);
            window.draw(score);
        }
        window.draw(cur);
        window.display();
    }

    return 0;
}
