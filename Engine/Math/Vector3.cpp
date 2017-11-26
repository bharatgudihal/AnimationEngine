#include "Vector3.h"

Engine::Graphics::Math::Vector3::Vector3():x(0.0f),y(0.0f),z(0.0f)
{
}

Engine::Graphics::Math::Vector3::Vector3(float x, float y, float z):x(x),y(y),z(z)
{
}

Engine::Graphics::Math::Vector3 Engine::Graphics::Math::Vector3::UP(0.0f, 1.0f, 0.0f);
Engine::Graphics::Math::Vector3 Engine::Graphics::Math::Vector3::ZERO;
Engine::Graphics::Math::Vector3 Engine::Graphics::Math::Vector3::RIGHT(1.0f, 0.0f, 0.0f);
Engine::Graphics::Math::Vector3 Engine::Graphics::Math::Vector3::FORWARD(0.0f, 0.0f, 1.0f);