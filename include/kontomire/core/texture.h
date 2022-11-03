#ifndef __KONTOMIRE_CORE_TEXTURE__
#define __KONTOMIRE_CORE_TEXTURE__

#include <memory>
#include <string>

namespace knt
{

class Texture
{
  public:
    virtual ~Texture() = default;

    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;
    virtual uint32_t id() const = 0;

    virtual void set_data(void* data, uint32_t size, uint8_t channels) = 0;

    virtual bool operator==(const Texture& other) const = 0;

    virtual void bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
  public:
    static std::shared_ptr<Texture2D> create(const std::string& path);
    static std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height);
};

} // namespace knt

#endif // __KONTOMIRE_CORE_TEXTURE__
