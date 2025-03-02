#include <map_part.h>
#include <resload.h>
void MapPart::Start() {
  texture = ResLoad::LoadImage(path);
}

void MapPart::Update() {
}

void MapPart::Fixed() {
}

void MapPart::Quit() {
  SDL_DestroyTexture(texture);
}
