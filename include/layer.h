#ifndef __LAME_GAME_LAYER__
#define __LAME_GAME_LAYER__

class Layer {
public:
    virtual void init() const noexcept {};
    virtual void destroy() const noexcept {};
    virtual void update() const noexcept {};
};

#endif // __LAME_GAME_LAYER__