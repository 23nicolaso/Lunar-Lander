#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

enum Direction         { LEFT, UP, RIGHT, DOWN              }; 
enum EntityStatus      { ACTIVE, INACTIVE                   };
enum EntityType        { PLAYER, BLOCK, UFO, NONE           };
enum BoostStatus       { BOOSTING, NEUTRAL                  };
enum RotateDirection   { ROTATE_LEFT = -1, ROTATE_RIGHT = 1 };
enum CollisionStatus   { PLAYING, LOSS, WIN };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    float mRotation;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;
    
    Texture2D mTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;
    
    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    BoostStatus mBoostStatus;
    CollisionStatus mCollisionStatus = PLAYING;
    RotateDirection mRotateDirection;

    int mFrameSpeed;
    float mFuel = 20.0f;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    int mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    bool isColliding(Entity *other) const;

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map *map);

    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Map *map);
    
    void resetColliderFlags() 
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   ROTATION_SPEED        = 30;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr int   DEFAULT_BOOST_SPEED   = 30;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;
    static constexpr float DRAG = 0.995f;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
        EntityType entityType);
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::map<Direction, std::vector<int>> animationAtlas, 
        EntityType entityType);
    ~Entity();

    void update(float deltaTime, Entity *player, Map *map, 
        Entity *collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void activate()   { mEntityStatus  = ACTIVE;   }
    void deactivate() { mEntityStatus  = INACTIVE; }
    void displayCollider();

    void applyImpulse(Vector2 impulse) { 
        mAcceleration.x = impulse.x; 
        mAcceleration.y = impulse.y; 
    }

    bool isActive() { return mEntityStatus == ACTIVE ? true : false; }

    void moveUp()      { mMovement.y      = -1; mDirection = UP;    }
    void moveDown()    { mMovement.y      =  1; mDirection = DOWN;  }
    void moveLeft()    { mMovement.x      = -1; mDirection = LEFT;  }
    void moveRight()   { mMovement.x      =  1; mDirection = RIGHT; }

    void rotateLeft()  { mRotation        = ROTATE_LEFT * ROTATION_SPEED; }
    void rotateRight() { mRotation        = ROTATE_RIGHT * ROTATION_SPEED;}
    void boost()       { mBoostStatus     = BOOSTING; }

    void resetMovement() {
        mRotation = 0.0f;  
        mBoostStatus = NEUTRAL;
    }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mScale;                 }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getFuel()                  const { return mFuel;                  }
    float       getAngle()                 const { return mAngle;                 }
    EntityType  getEntityType()            const { return mEntityType;            }
    
    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }
    bool isGameOver()        const { return mCollisionStatus != PLAYING; }
    bool hasWon()            const { return mCollisionStatus == WIN;     }
    bool hasLost()           const { return mCollisionStatus == LOSS;    }

    std::map<Direction, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions) 
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions) 
        { mSpriteSheetDimensions = newDimensions;  }
    void setSpeed(int newSpeed)
        { mSpeed  = newSpeed;                      }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setAngle(float newAngle) 
        { mAngle = newAngle;                       }
    void setEntityType(EntityType entityType)
        { mEntityType = entityType;                }
    void setDirection(Direction newDirection)
    { 
        mDirection = newDirection;

        if (mTextureType == ATLAS) mAnimationIndices = mAnimationAtlas.at(mDirection);
    }
};

#endif // ENTITY_CPP