#ifndef FORMHIGHSCORES_H
#define FORMHIGHSCORES_H

#include "form.h"

class FormHighScores : public Form
{
  public:
    FormHighScores();
    virtual ~FormHighScores();
  protected:
    void Draw() override;
    void Open() override;
    void HandleInput(Uint8* keyboardState) override;
    void Close() override;
  private:
};

#endif // FORMHIGHSCORES_H
