#include <stdio.h>
#include "Animation.h"

void update_animation(Animation* animation)
{
    animation->frameCount++;
    if (animation->frameCount > animation->frameDelay)
    {
        animation->curFrame++;
        animation->frameCount = 0;
        if (animation->curFrame >= animation->maxFrame)
        {
            if (animation->loop)
            {
                // Go back to the first frame if it should loop->
                animation->curFrame = 0;
            }
            else
            {
                // Freeze the animation on the last frame->
                // And flag it as ended->
                animation->curFrame = animation->maxFrame;
                animation->ended = true;
                // Once the animation has ended assume
                // that it may now be cancelled->
                animation->cancellable = true;
            }
        }
    }
}

Animation create_animation(enum AnimationType type) {
    Animation animation;
    animation.type = type;
    animation.curFrame = 0;
    animation.frameCount = 0;
    animation.animationColumns = 0;
    animation.animationRow = 0;
    animation.ended = false;

    switch (type) {
    case CLOCK_TICK:
        animation.animationColumns = 17;
        animation.animationRow = 1;
        animation.loop = true;
        animation.frameDelay = 2;
        animation.maxFrame = 17;
        break;
    case IDLE:
        animation.loop = true;
        animation.cancellable = true;
        animation.frameDelay = 10;
        break;
    case ATTACK:
        animation.loop = false;
        animation.cancellable = false;
        animation.frameDelay = 10;
        break;
    case PROTECT:
        animation.loop = false;
        animation.cancellable = false;
        animation.frameDelay = 10;
        break;
    case REST:
        animation.loop = false;
        animation.cancellable = false;
        animation.frameDelay = 10;
        break;
    case HAND_DRAW:
        animation.loop = false;
        animation.cancellable = false;
        animation.frameDelay = 1;
        animation.maxFrame = 18;
        break;
    case SCREEN_TRANSITION:
        animation.loop = false;
        animation.cancellable = false;
        animation.frameDelay = 1;
        animation.maxFrame = 48;
        break;
    }

    return animation;
}