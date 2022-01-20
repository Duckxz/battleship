#include "common.hh"
#include "animation.hh"

namespace bship {
  Animation::Animation(std::string name, std::string path, int fps, int frameCount, int width, int height)
  :
    name(name),
    playing(true), 
    frameCount(frameCount),
    fps(fps),
    width(width),
    height(height)
  {
    this->_tex = LoadTexture(path.c_str());
    this->currentFrame = 0;
  }

  Animation::~Animation()
  {
    UnloadTexture(this->_tex);
  }

  void Animation::tickAnim()
  {
    // check this first to prevent unnecessary calculations
    if(!this->playing)
      return;

    float ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastAnimTick).count(); 
    bool shouldTick = ms >= 1000/this->fps;
    if (shouldTick == false)
    {
      return;
    }
    this->lastAnimTick = std::chrono::high_resolution_clock::now();
    this->currentFrame = this->currentFrame >= this->frameCount ? 0 : this->currentFrame + 1;
  }


  void Animation::render()
  {
    //std::cout << "drawing frame " << this->current_frame << " of anim '" << this->name << "' at (" << x << ", " << y << ") with rec [" << 0 << ", " << 8*current_frame << ", " << 8 << ", " << 8 << "]\n";
    DrawTextureRec(this->_tex,{0,float(this->width*currentFrame),(float)this->width,(float)this->height},{float(this->x),float(this->y)},WHITE);
  }
}
