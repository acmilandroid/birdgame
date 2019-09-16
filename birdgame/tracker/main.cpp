//Basil Lin

#include "engine.h"

int main(int, char*[]) {
   bool keepPlaying = true;
   try {
      while (keepPlaying) {
         Engine* engine = new Engine;
         keepPlaying = engine->play();
         delete engine;
      }
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Exception thrown from engine." << std::endl;
   }
   return 0;
}
