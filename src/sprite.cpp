#include "sprite.h"

Sprite::Sprite()
{
  //printf ("Sprite ctor\t");

  _screenX = 0;
  _screenY = 0;

  _angle = 0.0;
  _scaleFactor = 1.0;

  _imageWrapper = nullptr;

  _spriteColor = Colors::AsIs;
}

Sprite::~Sprite()
{
  //printf ("Sprite dtor 0x%x 0x%x\t", this, _imageWrapper);
}

void Sprite::CalculateSATAxes()
{
  _projectionAxes.clear();
  _projectionAxesV2.clear();

  size_t length = _translatedCollider.size();
  for (int i = 0; i < length - 1; i++)
  {
    SDL_Point p1 = _translatedCollider[i];
    SDL_Point p2 = _translatedCollider[i + 1];

    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;

    SDL_Point p;
    Vector2 v;

    p.x = dy;
    p.y = -dx;

    v.Set(p.x, p.y);
    v.Normalize();

    _projectionAxes.push_back(p);
    _projectionAxesV2.push_back(v);
  }
}

int Sprite::Init(int textureIndex)
{
  _imageWrapper = TextureManager::Get().GetTextureWrapper(textureIndex);

  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Could not find texture with index %i!\n", textureIndex);
    return 1;
  }

  _colliderMinX = 0;
  _colliderMaxX = 0;

  _screenX = 0;
  _screenY = 0;

  _sourceRect.x = 0;
  _sourceRect.y = 0;
  _sourceRect.w = _imageWrapper->Width();
  _sourceRect.h = _imageWrapper->Height();

  _destRect.x = _screenX;
  _destRect.y = _screenY;
  _destRect.w = _imageWrapper->Width() * _scaleFactor;
  _destRect.h = _imageWrapper->Height() * _scaleFactor;

  _originalCollider = TextureManager::Get().GetCollider(textureIndex);

  if (_originalCollider != nullptr)
  {
    for (int i = 0; i < _originalCollider->size(); i++)
    {
      if (_originalCollider->at(i).x < _colliderMinX)
      {
        _colliderMinX = _originalCollider->at(i).x;
      }

      if (_originalCollider->at(i).x > _colliderMaxX)
      {
        _colliderMaxX = _originalCollider->at(i).x;
      }

      _originalColliderCopy.push_back(_originalCollider->at(i));
      _rotatedCollider.push_back(_originalCollider->at(i));
      _translatedCollider.push_back(_originalCollider->at(i));
      _scaledCollider.push_back(_originalCollider->at(i));
    }
  }

  _convex = IsConvex();

  if (!_convex)
  {
    TriangulateCollider();
  }

  return 0;
}

void Sprite::SetScaleFactor(double scaleFactor)
{
  _scaleFactor = scaleFactor;

  for (int i = 0; i < _originalCollider->size(); i++)
  {
    double tmpX = (double)_originalCollider->at(i).x * scaleFactor;
    double tmpY = (double)_originalCollider->at(i).y * scaleFactor;

    _scaledCollider[i].x = (int)tmpX;
    _scaledCollider[i].y = (int)tmpY;
  }

  _destRect.w = _imageWrapper->Width() * _scaleFactor;
  _destRect.h = _imageWrapper->Height() * _scaleFactor;
}

void Sprite::SetAngle(double angle)
{
  _angle = angle;

  int cs = _scaledCollider.size();
  Vector2 res;
  for (int i = 0; i < cs; i++)
  {
    Vector2 tmp(_scaledCollider[i].x, _scaledCollider[i].y);
    Vector2::RotateVector(res, Vector2(0.0, 0.0), tmp, angle);

    _rotatedCollider[i].x = res.X();
    _rotatedCollider[i].y = res.Y();
  }

}

// In order to avoid "acceleration" of results, we always offset from some point when performing calculations.
// I.e. first we take original collider, rotate it and remember the values of rotated points,
// then we offset those points to somewhere we want.
// Previously offset was taken from _originalCollider, thus incrementing the wrong position in the first place.
// And if we would just replace _originalCollider with += of the _rotatedCollider, it would "fly away", since
// increment performs every frame. Therefore, three colliders.
void Sprite::MoveCollider(double newX, double newY)
{
  int csize = _rotatedCollider.size();
  for (int i = 0; i < csize; i++)
  {
    _translatedCollider[i].x = _rotatedCollider[i].x + newX;
    _translatedCollider[i].y = _rotatedCollider[i].y + newY;
  }
}

void Sprite::Draw(int x, int y)
{
  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Sprite 0x%zX _imageWrapper is NULL, so drawing nothing\n", this);
    return;
  }

  _destRect.x = x - (_imageWrapper->Width() * _scaleFactor) / 2;
  _destRect.y = y - (_imageWrapper->Height() * _scaleFactor) / 2;

  SDL_SetTextureColorMod(_imageWrapper->Texture(), _spriteColor.r, _spriteColor.g, _spriteColor.b);
  SDL_SetTextureAlphaMod(_imageWrapper->Texture(), _spriteColor.a);

  int res = SDL_RenderCopyEx(VideoSystem::Get().Renderer(), _imageWrapper->Texture(), &_sourceRect, &_destRect, _angle, nullptr, SDL_FLIP_NONE);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
}

void Sprite::Draw(int x, int y, std::vector<SDL_Point>* colliderToDraw)
{
  Draw(x, y);

  if (colliderToDraw != nullptr)
  {
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), colliderToDraw->data(), colliderToDraw->size());
  }
}

void Sprite::SetColor(SDL_Color c)
{
  _spriteColor = c;
}

// ==================== Private Methods =================== //

bool Sprite::IsConvex()
{
  if (_originalCollider != nullptr)
  {
    int size = _originalCollider->size() - 1;
    for (int i = 0; i < size; i++)
    {
      double d1x = _originalCollider->at( (i + 1) % size ).x - _originalCollider->at(i).x;
      double d1y = _originalCollider->at( (i + 1) % size ).y - _originalCollider->at(i).y;
      double d2x = _originalCollider->at( (i + 2) % size ).x - _originalCollider->at( (i + 1) % size ).x;
      double d2y = _originalCollider->at( (i + 2) % size ).y - _originalCollider->at( (i + 1) % size ).y;

      double cross = d1x * d2y - d1y * d2x;

      // Positive Y goes down here,
      // so we need negative cross products in contrast to Unity triangulation test project
      if (cross > 0)
      {
        //printf("Concave\n");
        return false;
      }
    }

    //printf("Convex\n");

    return true;
  }
}

void Sprite::TriangulateCollider()
{
  // If we get stuck in infinite recursion.
  bool loopDetector = true;

  /*
  // Not very necessary check for remainig vertices collinearity
  if (_originalColliderCopy.size() != 0)
  {
    bool found = false;
    int size = _originalColliderCopy.size();
    for (int i = 0; i < size; i++)
    {
      Vector2 p1Min(_originalColliderCopy[i].x, _originalColliderCopy[i].y);
      Vector2 p1Max(_originalColliderCopy[(i + 1) % size].x, _originalColliderCopy[(i + 1) % size].y);

      Vector2 p2Min(_originalColliderCopy[(i + 1) % size].x, _originalColliderCopy[(i + 1) % size].y);
      Vector2 p2Max(_originalColliderCopy[(i + 2) % size].x, _originalColliderCopy[(i + 2) % size].y);

      Vector2Pair s1(p1Min, p1Max);
      Vector2Pair s2(p2Min, p2Max);

      float cross = Util::Vector2Cross(s1.Direction(), s2.Direction());

      if (cross != 0.0f)
      {
        found = true;
      }
    }

    if (!found)
    {
      polyRef.Clear();
      Debug.Log("Remaining vertices are collinear - removing");
      return;
    }
  }
  else
  {
    Debug.Log("We're done");
    return;
  }
  */

  // If we got all triangles we need (which are n-2 =>  Points.Count-3 in our case, since we have first vertex excplicitly defined in vertex filelist)
  if ( _triangulatedCollider.size() == (_originalCollider->size() - 3) )
  {
    Logger::Get().LogPrint("Triangulation finished\n");
    return;
  }

  // Look for two convex (or, more precise, with proper winding) sides.
  // If we found one, check if triangle formed by these vertices contains any vertices from the polygon.
  int size = _originalColliderCopy.size();
  for (int i = 0; i < size; i++)
  {
    Vector2 v1(_originalColliderCopy[i].x, _originalColliderCopy[i].y);
    Vector2 v2(_originalColliderCopy[ (i + 1) %  size ].x, _originalColliderCopy[ (i + 1) % size ].y);
    Vector2 v3(_originalColliderCopy[ (i + 2) %  size ].x, _originalColliderCopy[ (i + 2) % size ].y);

    Vector2Pair side1(v1, v2);
    Vector2Pair side2(v2, v3);

    double cross = Util::Vector2Cross(side1.Direction(), side2.Direction());

    if (cross < 0)
    {
      std::vector<Vector2> triangle;
      triangle.push_back(v1);
      triangle.push_back(v2);
      triangle.push_back(v3);

      if (IsTriangleValid(triangle, _originalColliderCopy))
      {
        loopDetector = false;
        //Logger::Get().LogPrint("Adding triangle: " + t);
        _triangulatedPolygon.push_back(triangle);
        _originalColliderCopy.erase(_originalColliderCopy.begin() + (i + 1));
        //PrintVertices(polyRef);
        break;
      }
    }
  }

  //Logger::Get().LogPrint("Total Triangles: " + _triangulatedPolygon.Count);

  if (loopDetector)
  {
    Logger::Get().LogPrint("Looks like we're running in circles - exiting\n");
    return;
  }

  TriangulateCollider();
}

bool Sprite::IsTriangleValid(std::vector<Vector2>& triangle, std::vector<SDL_Point>& collider)
{
  for (auto& v : collider)
  {
    if ( (v.X() == t[0].X() && v.Y() == t[0].Y() )
      || (v.X() == t[1].X() && v.Y() == t[1].Y() )
      || (v.X() == t[2].X() && v.Y() == t[2].Y() ) )
    {
      continue;
    }

    bool result = IsPointOutsideTriangle(t, v);

    //Debug.Log(t + " " + v + " result: " + result);

    if (!result) return false;
  }

  return true;
}

bool Sprite::IsPointOutsideTriangle(std::vector<Vector2>& triangle, Vector2 point)
{
  Vector2Pair rayLeft(point, Vector2(_colliderMinX, point.y));
  Vector2Pair rayRight(point, Vector2(_colliderMaxX, point.y));

  int leftCount = 0;
  int rightCount = 0;

  int size = triangle.size();
  for (int i = 0; i < size; i++)
  {
    Vector2 v1(triangle[i].x, triangle[i].y);
    Vector2 v2(triangle[ (i + 1) % size ].x, triangle[ (i + 1) % size ].y);
    Vector2 v3(triangle[ (i + 2) % size ].x, triangle[ (i + 2) % size ].y);

    Vector2Pair s1(v1, v2);
    Vector2Pair s1(v2, v3);
    Vector2Pair s1(v3, v1);

    // Special case
    if (Util.IsPointOnTheLine(diag, point)) continue;

    if (Util.AreLinesIntersecting(rayLeft, side, true))
    {
      if (side.Min().Y() > point.Y() || side.Max().Y() > point.Y())
      {
        leftCount++;
      }
    }

    if (Util.AreLinesIntersecting(rayRight, side, true))
    {
      if (side.Min().Y() > point.Y() || side.Max().Y() > point.Y())
      {
        rightCount++;
      }
    }
  }

  //Debug.Log(point + " " + leftCount + " " + rightCount);

  return (leftCount % 2 == 0 && rightCount % 2 == 0);
}
