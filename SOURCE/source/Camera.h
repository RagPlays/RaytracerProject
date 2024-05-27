#pragma once
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

namespace dae
{
	struct Camera
	{
		Camera() = default;

		Vector3 origin{};
		int fovAngle{};
		float fovValue{};

		Vector3 forward{ 0.f, 0.f, 1.f };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		float totalPitch{};
		float totalYaw{};

		const Matrix CalculateCameraToWorld()
		{
			right = { Vector3::Cross(Vector3::UnitY, forward).Normalized() };
			up = { Vector3::Cross(forward, right).Normalized() };

			return
			{ 
				{ right, 0.f },
				{ up, 0.f },
				{ forward, 0.f },
				{ origin, 1.f }
			};
		}

		void Update(Timer* pTimer)
		{
			const int fovMin{ 0 };
			const int fovMax{ 180 };
			const float movementSpeed{ 15.f };
			const float sensitivity{ 0.2f };

			// DeltaTime //
			const float deltaTimeSpeed{ pTimer->GetElapsed() * movementSpeed };

			// Keyboard Input //
			const uint8_t* pKeyboardState{ SDL_GetKeyboardState(nullptr) };

			// WASD
			if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP])	// forward
			{
				origin += deltaTimeSpeed * forward;
			}
			else if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN]) // backwards
			{
				origin -= deltaTimeSpeed * forward;
			}

			if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT]) // left
			{
				origin -= deltaTimeSpeed * right;
			}
			else if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT]) // right
			{
				origin += deltaTimeSpeed * right;
			}

			// Up and Down
			if (pKeyboardState[SDL_SCANCODE_E]) // up
			{
				origin += deltaTimeSpeed * Vector3::UnitY;
			}
			else if (pKeyboardState[SDL_SCANCODE_Q]) // down
			{
				origin += deltaTimeSpeed * -Vector3::UnitY;
			}

			// Change POV
			if (pKeyboardState[SDL_SCANCODE_T])	// pov down
			{
				fovAngle -= 10;

				if (fovAngle < fovMin) fovAngle = fovMin;

				fovValue = tanf((fovAngle * TO_RADIANS) * 0.5f);
			}
			else if (pKeyboardState[SDL_SCANCODE_G])	// pov up
			{
				fovAngle += 10;

				if (fovAngle > fovMax) fovAngle = fovMax;

				fovValue = tanf((fovAngle * TO_RADIANS) * 0.5f);
			}
			else if (pKeyboardState[SDL_SCANCODE_R])	// reset pov
			{
				fovAngle = 45;
				fovValue = tanf((fovAngle * TO_RADIANS) * 0.5f);
			}

			// Mouse Input //
			int mouseX;
			int mouseY;
			const uint32_t mouseState{ SDL_GetRelativeMouseState(&mouseX, &mouseY) };

			if (!mouseState) return;

			// MOUSE
			if (mouseState & SDL_BUTTON_RMASK && mouseState & SDL_BUTTON_LMASK)
			{
				origin += -mouseY * sensitivity * Vector3::UnitY;
			}
			else if (mouseState & SDL_BUTTON_RMASK)
			{
				totalPitch += mouseY * sensitivity;
				totalYaw += mouseX * sensitivity;
			}
			else if (mouseState & SDL_BUTTON_LMASK)
			{
				origin += -mouseY * sensitivity * Vector3::UnitZ;
				totalYaw += mouseX * sensitivity;
			}

			forward = Matrix::CreateRotation(totalPitch * TO_RADIANS, totalYaw * TO_RADIANS, 0.f).TransformVector(Vector3::UnitZ).Normalized();
		}
	};
}
