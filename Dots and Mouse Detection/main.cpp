#include <SFML/Graphics.hpp>
#include "utilities.hpp"
#include <iostream>
#include <cmath>

int
    numOfDots = 3000,
    positionsX[3000], // One for each dot
    positionsY[3000], // One for each dot
    winX = 1600,
    winY = 900,
    padding = 10;
    
sf::Color colors[3000]; // One for each dot

/**
 * Generates random positions and colors
 */
void generateDotData() {
    for (int i = 0; i < numOfDots; ++i) {
        positionsX[i] = randInt(padding, winX - padding);
        positionsY[i] = randInt(padding, winY - padding);
        colors[i] = sf::Color(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
}

/**
 * Handles mouse and dot collisions and draws the dots.
 * @param window Window to draw on
 */
void drawDots(sf::RenderWindow* window) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
    int
        r = 10,
        bufferZone = 2 * r + 5, // Diameter of dot + 5 padding
        differenceX,
        differenceY,
        *posX,
        *posY;
    double
        xOffset = 1,
        yOffset = 1,
        angle;
    
    for (int i = 0; i < numOfDots; ++i) {
        // Position randomally anywhere within 5px of the edge of the window
        differenceX = mousePosition.x - positionsX[i];
        differenceY = mousePosition.y - positionsY[i];
        posX = &positionsX[i];
        posY = &positionsY[i];
        
        // If mouse is in buffer zone of dot
        if (abs(differenceX) <= bufferZone && abs(differenceY) <= bufferZone) {
            if (differenceX != 0 && differenceY != 0) {
                // Angle mouse is approaching dot
                angle = atan(differenceY/differenceX);
                xOffset = cos(angle);
                yOffset = sin(angle);
            }

            // Mouse is to right of dot
            if (differenceX > 0) {
                // Move dot to the left to maintain buffer zone
                *posX -= xOffset * (bufferZone - differenceX);
            }
            // Mouse is to left of dot
            else if (differenceX < 0) {
                // Move dot to the right to maintain buffer zone
                *posX += xOffset * (bufferZone + differenceX);
            }

            // Mouse is above dot
            if (differenceY > 0) {
                // Move dot up to maintain buffer zone
                *posY -= yOffset * (bufferZone - differenceY);
            }
            // Mouse is below dot
            else if (differenceY < 0) {
                // Move dot down to maintain buffer zone
                *posY += yOffset * (bufferZone + differenceY);
            }
        }

        // Circle with radius of r
        sf::CircleShape circle(r);
        circle.setOrigin(r, r); // Sets origin to center of circle
        circle.setFillColor(colors[i]); // Random color
        circle.setPosition(*posX, *posY); // Random position

        window->draw(circle);
    }
}

/**
 * Handles all rendering processes
 * @param window Window to render to
 */
void renderingThread(sf::RenderWindow* window) {
    while (window->isOpen()) {
        // Clear before drawing
        window->clear(sf::Color::Black);
        
        // Draw
        drawDots(window);

        window->display();
    }
}

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(winX, winY), "Dots and Mouse Detection");

    // deactivate its OpenGL context
    window.setActive(false);
    
    // Prerendering
    generateDotData();

    // launch the rendering thread
    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}