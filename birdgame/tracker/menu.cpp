#include<sstream>
#include "menu.h"

Menu::Menu(SDL_Renderer* rend) :  
  renderer(rend), 
  gdata(Gamedata::getInstance()), 
  hudFrame( {gdata.getXmlInt("menu/loc/x"),
             gdata.getXmlInt("menu/loc/y"),
             gdata.getXmlInt("menu/width"),
             gdata.getXmlInt("menu/height")}
  ),
  backColor({static_cast<Uint8>(gdata.getXmlInt("menu/backColor/r")),
             static_cast<Uint8>(gdata.getXmlInt("menu/backColor/g")),
             static_cast<Uint8>(gdata.getXmlInt("menu/backColor/b")),
             static_cast<Uint8>(gdata.getXmlInt("menu/backColor/a"))}
  ),
  menuColor({static_cast<Uint8>(gdata.getXmlInt("menu/color/r")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/g")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/b")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/a"))}
  ),
  clock( Clock::getInstance() ),
  io( IoMod::getInstance() ),
  options(),
  optionLoc( { gdata.getXmlInt("menu/optionLoc/x"),
               gdata.getXmlInt("menu/optionLoc/y")}
           ),
  clicks( {Sprite("clickOff"), Sprite("clickOn")} ),
  currentClick(0),
  currentOption(0),
  spaces(gdata.getXmlInt("menu/spaces")),
  startClickX(optionLoc[0]-spaces),
  startClickY(optionLoc[1]+spaces),
  clickX(startClickX),
  clickY(startClickY)
{
  int noOfOptions = gdata.getXmlInt("menu/noOfOptions");
  std::stringstream strm;
  for (int i = 0; i < noOfOptions; ++i) {
    strm.clear();
    strm.str("");
    strm << i;
    std::string option("menu/option"+strm.str());
    options.push_back(gdata.getXmlStr(option));
  }
}

void Menu::incrIcon() { 
  clickY += spaces; 
  if ( clickY > static_cast<int>(options.size())*spaces+optionLoc[1]) {
    clickY = startClickY;
    currentOption = 0;
  }
  else ++currentOption;
}

void Menu::decrIcon() { 
  clickY -= spaces; 
  if ( clickY < spaces+optionLoc[1]) {
    clickY = startClickY+2*spaces;
    currentOption = options.size()-1;
  }
  else --currentOption;
}

void Menu::drawBackground() const {
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the hud background color:
  SDL_SetRenderDrawColor( renderer, backColor.r, backColor.g, 
     backColor.b, backColor.a );

  // Draw the filled rectangle:
  SDL_RenderFillRect( renderer, &hudFrame );

  // Set the  color for the Menu outline:
  SDL_Rect menuFrame = {hudFrame.x+50, hudFrame.y+40,
                        hudFrame.w-100, hudFrame.h-100};
  SDL_SetRenderDrawColor( renderer, menuColor.r, 
                          menuColor.g, menuColor.b, menuColor.a );
  SDL_RenderFillRect( renderer, &menuFrame );

}

void Menu::draw() const {
  drawBackground();
  io.writeText("Options Menu", hudFrame.x+350, hudFrame.y+60);
  int space = spaces;
  for ( const std::string& option : options ) {
    io.writeText(option, optionLoc[0], optionLoc[1]+space);
    space += spaces;
  }
  // We have to draw the clickOn & clickOff relative to the screen,
  // and we don't want to offset by the location of the viewprot:
  clicks[currentClick].getImage()->draw(0, 0, clickX, clickY);
}

void Menu::help() {
  drawBackground();
  std::ostringstream stream;
  stream << "Poop on the planes to collect points!";
  io.writeText(stream.str(), hudFrame.x+200, hudFrame.y+80);
  stream.str("");
  stream << "Avoid being hit by planes or eagles.";
  io.writeText(stream.str(), hudFrame.x+200, hudFrame.y+110);
  stream.str("");
  stream << "Fly into other pigeons to collect more lives!";
  io.writeText(stream.str(), hudFrame.x+200, hudFrame.y+140);
  stream.str("");
  stream << "F1 - toggle help";
  io.writeText(stream.str(),  hudFrame.x+200, hudFrame.y+170);
  stream.str("");
  stream << "WASD - move";
  io.writeText(stream.str(),  hudFrame.x+200, hudFrame.y+200);
  stream.str("");
  stream << "P - pause / unpause";
  io.writeText(stream.str(),  hudFrame.x+200, hudFrame.y+230);
  stream.str("");
  stream << "SPACE - poop";
  io.writeText(stream.str(),  hudFrame.x+200, hudFrame.y+260);
  stream.str("");
  stream << "G - toggle god mode";
  io.writeText(stream.str(),  hudFrame.x+200, hudFrame.y+290); 
  stream.str("");
  stream << "Press return or escape to go back";
  io.writeText(stream.str(),  hudFrame.x+200, hudFrame.y+350); 
  stream.str("");
  SDL_RenderPresent(renderer);
  
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  
  while (!done) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || 
            keystate[SDL_SCANCODE_Q] ||
            keystate[SDL_SCANCODE_RETURN] ) {
          done = true;
          break;
        }
      }
    }
  }
}

