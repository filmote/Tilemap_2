// Copyright (C) 2019 Hannu Viitala
// 
// The source code in this file is released under the MIT license.
// Go to http://opensource.org/licenses/MIT for the full license details.
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
    
    const uint8_t mapTileWidth = 16;                              // Map width in tiles ..
    const uint8_t mapTileHeight = 16;                             // Map height in tiles ..
    
    const uint16_t worldWidth = mapTileWidth * tileWidth;         // 16 tiles of 16 pixels
    const uint16_t worldHeight = mapTileHeight * tileHeight;      // 16 tiles of 16 pixels
    
    const uint16_t screenCentreX = PD::width / 2; 
    const uint16_t screenCentreY = PD::height / 2;

}



// ---------------------------------------------------------------------------------------


enum TileType {
    
	Water = 0,
	Green = 1,
	Tree = 2,
	Grass = 3,

}; 


enum Direction {
    
	Up,
	Down,
	Left,
	Right
	
}; 


struct Entity {
    
    int16_t x;
    int16_t y;

    const uint8_t width = 12;
    const uint8_t height = 15;

};


// ---------------------------------------------------------------------------------------


Tilemap tilemap;
Entity player;


// ---------------------------------------------------------------------------------------


void calculateViewPortPosition(Entity &entity, int16_t &xViewPort, int16_t &yViewPort) {
    
    if (entity.x < PD::width / 2) {
        
        xViewPort = 0;
        
    }
    else if (entity.x > Constants::worldWidth - PD::width / 2) {

        xViewPort = PD::width - Constants::worldWidth;
        
    }
    else {
        
        xViewPort = PD::width / 2 - entity.x;

    }
    
    if (entity.y < PD::height / 2) {
        
        yViewPort = 0;
        
    }
    else if (entity.y > Constants::worldHeight - PD::height / 2) {

        yViewPort = PD::height - Constants::worldHeight;
        
    }
    else {
        
        yViewPort = PD::height / 2 - entity.y;

    }
    
}


// ---------------------------------------------------------------------------------------


void calculatePlayerPosition(Entity &entity, int16_t &xPlayer, int16_t &yPlayer) {
    
    if (entity.x < PD::width / 2) {
        
        xPlayer = entity.x;
        
    }
    else if (entity.x > Constants::worldWidth - PD::width / 2) {

        xPlayer = entity.x - (Constants::worldWidth - PD::width);
        
    }
    else {
        
        xPlayer = PD::width / 2;

    }
    
    if (entity.y < PD::height / 2) {
        
        yPlayer = entity.y;
        
    }
    else if (entity.y > Constants::worldHeight - PD::height / 2) {

        yPlayer = entity.y - (Constants::worldHeight - PD::height);
        
    }
    else {
        
        yPlayer = PD::height / 2;

    }

}


// ---------------------------------------------------------------------------------------
//
//  Check to see if the move the entity is about to make is into a green (empty) tile.  As 
//  the player is 12 x 15 pixels and the tiles are bigger (16 x 16) its possible that the 
//  player could be straddling two tiles in either direction ..
//
bool checkMovement(Entity &entity, int16_t x, int16_t y, Direction direction) {

    int8_t tileId1 = 0;
    int8_t tileId2 = 0;

    switch (direction) {
        
        case Direction::Left:
            tileId1 = tilemap.GetTileId(x, y, 16);
            tileId2 = tilemap.GetTileId(x, y + entity.height, 16);
            break;
        
        case Direction::Right:
            tileId1 = tilemap.GetTileId(x + entity.width, y, 16);
            tileId2 = tilemap.GetTileId(x + entity.width, y + entity.height, 16);
            break;

        case Direction::Up:
            tileId1 = tilemap.GetTileId(x, y, 16);
            tileId2 = tilemap.GetTileId(x + entity.width, y, 16);
            break;
        
        case Direction::Down:
            tileId1 = tilemap.GetTileId(x, y + player.height, 16);
            tileId2 = tilemap.GetTileId(x + entity.width, y + entity.height, 16);
            break;
            
    }
    


    // If either tile is not green, do not move.
    
    if (tileId1 != TileType::Green || tileId2 != TileType::Green) {
        return false;
        
    }

    return true;
        
}


// ---------------------------------------------------------------------------------------
//
//  Handle the player movements ..
//
void handlePlayerMovements() {

    if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, 1))    { 

        if (player.x > 0 && checkMovement(player, player.x - 1, player.y, Direction::Left)) {

            player.x--;

        }

    }
    
    if (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, 1))   { 

        if (player.x < Constants::worldWidth && checkMovement(player, player.x + 1, player.y, Direction::Right)) {

            player.x++;
        
            
        }

    }
    
    
    if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, 1))      { 

        if (player.y > 0 && checkMovement(player, player.x, player.y - 1, Direction::Up)) {

            player.y--;

        }
        
    }
    
    if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.repeat(BTN_DOWN, 1))    { 

        if (player.y < Constants::worldHeight && checkMovement(player, player.x, player.y + 1, Direction::Down)) {

            player.y++;
            
        }

    }
    
}



// ---------------------------------------------------------------------------------------


int main(){

    PC::begin();
    PD::setFont(fontC64);
    PD::loadRGBPalette(palettePico);   
    PD::persistence = true;
    PD::invisiblecolor = 12;



    // Initialise the map ..
    
    tilemap.set(16, 16, Data::mapPixels);
    tilemap.setTile(TileType::Green, 16, 16, Data::green16);
    tilemap.setTile(TileType::Tree, 16, 16, Data::tree16);
    tilemap.setTile(TileType::Grass, 16, 16, Data::grass16);
    tilemap.setTile(TileType::Water, 16, 16, Data::water16);


    // Position the player into a vacant spot on the map ..
    
    player.x = 48;
    player.y = 48;
    
    
    while (PC::isRunning()) {
        
        if (!PC::update()) continue;

        PC::buttons.pollButtons();        



        // Handle player movements ..

        handlePlayerMovements();


        // Render screen ..

        int16_t xViewPort;
        int16_t yViewPort;
        
        calculateViewPortPosition(player, xViewPort, yViewPort);
        tilemap.draw(xViewPort, yViewPort);

        
        // Render player ..
        
        int16_t xPlayer;
        int16_t yPlayer;
        
        calculatePlayerPosition(player, xPlayer, yPlayer);
        PD::drawBitmapData(xPlayer, yPlayer, 12, 15, Data::girl12x15Pixels);        

    }
    
    return 0;
    
}

