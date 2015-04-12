#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "png_loader.h"
#include "texture_manager.h"
#include "util.h"

class Sprite
{
  public:
    Sprite();
    virtual ~Sprite();
    int Init(int textureIndex);
    void SetAngle(double angle);
    void MoveCollider(double newX, double newY);
    void Draw(int x, int y);
    void Draw(int x, int y, std::vector<SDL_Point>* colliderToDraw);
    void Draw(int x, int y, std::vector<std::vector<SDL_Point>>* colliderToDraw);

    void SetColor(SDL_Color c);
    void SetScaleFactor(double scaleFactor);

    PNGLoader* ImageWrapper() { return _imageWrapper; }
    SDL_Rect* SourceRect() { return &_sourceRect; }
    SDL_Rect* DestinationRect() { return &_destRect; }
    // Collider points with default origin ( [0;0] at upper left corner of the image)
    std::vector<SDL_Point>* OriginalCollider() { return _originalCollider; }
    // Original collider after scale
    std::vector<SDL_Point>& ScaledCollider() { return _scaledCollider; }
    // The same points after rotation
    std::vector<SDL_Point>& RotatedCollider() { return _rotatedCollider; }
    // The above points after translation (basically, you should use this one)
    std::vector<SDL_Point>& TranslatedCollider() { return _translatedCollider; }
    // Axes to project on for SAT test
    void CalculateSATAxes();

    std::vector<std::vector<SDL_Point>>& TriangulatedTranslatedCollider() { return _triangulatedTranslatedCollider; }

    std::vector<SDL_Point>& GetAxes() { return _projectionAxes; }
    std::vector<Vector2>& GetAxesV2() { return _projectionAxesV2; }

    CollisionInfo& GetCollisionInfo()
    {
      CalculateSATAxes();

      return _collisionInfo;
    }

    bool Convex() { return _convex; }

  private:
    SDL_Color _spriteColor;
    SDL_Rect _sourceRect;
    SDL_Rect _destRect;
    PNGLoader* _imageWrapper;
    int _screenX;
    int _screenY;
    double _scaleFactor;
    double _angle;
    bool _triangulated;

    CollisionInfo _collisionInfo;

    // Owner is TextureManager
    std::vector<SDL_Point>* _originalCollider;

    std::vector<SDL_Point> _rotatedCollider;
    std::vector<SDL_Point> _translatedCollider;
    std::vector<SDL_Point> _scaledCollider;

    std::vector<SDL_Point> _projectionAxes;
    std::vector<Vector2> _projectionAxesV2;

    // Used up in triangulation method
    std::vector<SDL_Point> _originalColliderCopy;

    std::vector<std::vector<SDL_Point>> _triangulatedCollider;
    std::vector<std::vector<SDL_Point>> _triangulatedScaledCollider;
    std::vector<std::vector<SDL_Point>> _triangulatedRotatedCollider;
    std::vector<std::vector<SDL_Point>> _triangulatedTranslatedCollider;

    int _colliderMinX;
    int _colliderMaxX;

    bool _convex;

    bool IsConvex();
    void TriangulateCollider();
    bool IsTriangleValid(std::vector<Vector2>& triangle, std::vector<SDL_Point>& collider);
    bool IsPointOutsideTriangle(std::vector<Vector2>& triangle, Vector2 point);
    void PrintVertices(std::vector<SDL_Point>& data);
};

#endif // SPRITE_H
