// Copyright (C) 2019 Hannu Viitala
// 
// The source code in this file is released under the MIT license.
// Go to http://opensource.org/licenses/MIT for the full license details.5
// 
// Converted to C++ and extended by Filmote

// *** A TILEMAP DEMO FOR THE POKITTO ***

#include "Pokitto.h"
#include "Tilemap.hpp"
#include "Data.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

namespace Constants {
    
    const uint8_t tileWidth = 16;
    const uint8_t tileHeight = 16;
    
    const uint8_t mapTileWidth = 16;                            // Map width in tiles ..
    const uint8_t mapTileHeight = 16;                           // Map height in tiles ..
    
    const uint16_t mapWidth = mapTileWidth * tileWidth;         // 16 tiles of 16 pixels
    const uint16_t mapHeight = mapTileHeight * tileHeight;      // 16 tiles of 16 pixels

}


// ---------------------------------------------------------------------------------------


enum TileType {

	Water = 0,
	Green = 1,
	Tree = 2,
	Grass = 3,
    
}; 


// ---------------------------------------------------------------------------------------


Tilemap tilemap;


// ---------------------------------------------------------------------------------------

int main(){

    PC::begin();
    PD::persistence = true;
    PD::invisiblecolor = 0;


    // Initialise the map ..
    
    tilemap.set(16, 16, Data::mapPixels);
    tilemap.tiles[TileType::Green] = Data::green16;
    tilemap.tiles[TileType::Tree] = Data::tree16;
    tilemap.tiles[TileType::Grass] = Data::grass16;
    tilemap.tiles[TileType::Water] = Data::water16;

    
    int16_t x = -20;
    int16_t y = -50;

    int16_t xOffset = 0;
    int16_t yOffset = 0;

    int16_t screenCentreX = PD::width / 2; 
    int16_t screenCentreY = PD::height / 2;

    
    while (PC::isRunning()) {
        
        if (!PC::update()) continue;

        PC::buttons.pollButtons();        


        // Capture the old coordinates in case we need to move back ..
        
        int16_t oldX = x;
        int16_t oldY = y;
        int16_t oldXOffset = xOffset;
        int16_t oldYOffset = yOffset;


        if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, 1))    { 


            // If we are already on the right hand side of the screen, come back to the centre ..
            
            if (xOffset < 0) {                                  
                xOffset++;
            }
            
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (x < 0) {
                x++;
            }
            
            
            // If we cannot scroll the screen, move the player to the left ..
            
            else if (x <= (PD::width / 2)) {     
                xOffset++;
            }


        }
        
        if (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, 1))   { 


            // If we are already on the left hand side of the screen, come back to the centre ..
            
            if (xOffset > 0) {
                xOffset--;
            }
                 
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (x > (PD::width - Constants::mapWidth) && x <= 0) {
                x--;
            }
            
            
            // If we cannot scroll the screen, move the player to the right ..
            
            else if (x >= (PD::width - Constants::mapWidth)) {
                xOffset--;
            }

            
        }
        
        
        if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, 1))      { 

            
            // If we are already in the bottom section of the screen, come back to the centre ..
            
            if (yOffset < 0) {                                  
                yOffset++;
            }
            
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (y < 0) {
                y++;
            }
            
            
            // If we cannot scroll the screen, move the player up ..
            
            else if (y <= (PD::height / 2)) {     
                yOffset++;
            }
            
            
        }
        
        if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.repeat(BTN_DOWN, 1))    { 


            // If we are already in the top section of the screen, come back to the centre ..
            
            if (yOffset > 0) {
                yOffset--;
            }
                 
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (y > (PD::height - Constants::mapHeight) && y <= 0) {
                y--;
            }
            
            
            // If we cannot scroll the screen, move the player down ..
            
            else if (y >= (PD::height - Constants::mapHeight)) {
                yOffset--;
            }
           
        }



        // Check the map tile under the player.
        
        int16_t playerCenterInMapX = (screenCentreX + 6) - x - xOffset;
        int16_t playerCerterInMapY = (screenCentreY + 7) - y - yOffset;
        uint8_t tileId = tilemap.GetTileId(playerCenterInMapX, playerCenterInMapY, 16);


        // If the tile is not green, do not move.
        
        if (tileId != TileType::Green) {
            
            x = oldX;
            y = oldY;
            xOffset = oldXOffset;
            yOffset = oldYOffset;
            
        }

        
        tilemap.draw(x, y);
        PD::drawBitmapData(screenCentreX - xOffset, screenCentreY - yOffset, 12, 15, Data::girl12x15Pixels);

    }
    
    return 0;
    
}

