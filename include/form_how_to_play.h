#ifndef FORMHOWTOPLAY_H
#define FORMHOWTOPLAY_H

#include "form.h"
#include "video_system.h"

class FormHowToPlay : public Form
{
  public:
    FormHowToPlay();
    virtual ~FormHowToPlay();
  protected:
    void Draw() override;
    void HandleInput(Uint8* keyboardState) override;
    void Open() override;
    void Close() override;
  private:
};

#endif // FORMHOWTOPLAY_H
