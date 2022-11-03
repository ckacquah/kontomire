#ifndef __KONTOMIRE_RENDERER_CAMERA__
#define __KONTOMIRE_RENDERER_CAMERA__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace knt
{

namespace CameraDefaults
{
static const float YAW{90.0f};
static const float PITCH{0.0f};

static const float ZOOM{45.0f};
static const float SPEED{2.5f};
static const float SENSITIVITY{0.1f};
} // namespace CameraDefaults

class Camera
{
  private:
    float yaw_{CameraDefaults::YAW};
    float zoom_{CameraDefaults::ZOOM};
    float pitch_{CameraDefaults::PITCH};

    float aspect_ratio_{};

    glm::vec3 up_{};
    glm::vec3 right_{};
    glm::vec3 front_{};
    glm::vec3 world_up_{};
    glm::vec3 position_{};

    glm::mat4 view_projection_{};

  public:
    enum Direction
    {
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD
    };

  private:
    void update()
    {
        glm::vec3 front{};
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);
        right_ = glm::normalize(glm::cross(front_, world_up_));
        up_ = glm::normalize(glm::cross(right_, front_));

        view_projection_ = glm::perspective(glm::radians(zoom_), aspect_ratio_, 0.1f, 100.0f) *
                           glm::lookAt(position_, position_ + front_, up_);
    }

  public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = CameraDefaults::YAW, float pitch = CameraDefaults::PITCH)
        : position_(position), up_(up), world_up_(up), yaw_(yaw), pitch_(pitch), front_(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        update();
    }

    virtual ~Camera() = default;

    const glm::mat4& view_projection() const
    {
        return view_projection_;
    }

    void set_aspect_ratio(float ratio)
    {
        aspect_ratio_ = ratio;
    }

    void zoom(float level)
    {
        zoom_ = CameraDefaults::ZOOM - level;
        update();
    }

    void rotate(float yaw, float pitch)
    {
        yaw_ = yaw;
        pitch_ = pitch;
        update();
    }

    void move(Direction direction, float speed = CameraDefaults::SPEED)
    {
        switch (direction)
        {
        case Direction::LEFT:
            position_ -= right_ * speed;
            break;
        case Direction::RIGHT:
            position_ += right_ * speed;
            break;
        case Direction::FORWARD:
            position_ += front_ * speed;
            break;
        case Direction::BACKWARD:
            position_ -= front_ * speed;
            break;
        }
        update();
    }
};

} // namespace knt

#endif // __KONTOMIRE_RENDERER_CAMERA__
