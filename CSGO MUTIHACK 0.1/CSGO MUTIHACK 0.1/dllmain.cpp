#define WIN32_LEAN_AND_MEAN
#include "csgo.hpp"
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <thread>

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

bool bHop = false, AntiFlash = false, RadarHack = false, TriggerBot =  false, KeyHeld = false, RCS = false, Glow = false;

float red, green, blue;

struct Entity
{
	int dwFlags;
	int Health;
	int TeamFlag;
	int CrossHairInt;
	DWORD EntityList;

	void ForceJump(DWORD GameModule)
	{
		*(int*)(GameModule + dwForceJump) = 2;
	}
}Game;

struct vector
{
	float x, y, z;
};

struct vector2
{
	float x = { }, y = { };
};






void Shoot(uintptr_t ModuleBaseAddr)
{
	*reinterpret_cast<int*>(ModuleBaseAddr + dwForceAttack) = 2;

	Sleep(20);

	*reinterpret_cast<int*>(ModuleBaseAddr + dwForceAttack) = 4;
	
}






void HackThread(HMODULE hModule) noexcept
{

	AllocConsole();

	FILE * f;

	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "NUMPAD 1 = BHOP " << std::endl;
	Sleep(500);
	std::cout << "NUMPAD 2 = ANTIFLASH " << std::endl;
	Sleep(500);
	std::cout << "NUMPAD 3 = RADAR HACK " << std::endl;
	Sleep(500);
	std::cout << "NUMPAD 4 = TOGGLE TRIGGER BOT / HOLD ALT TRIGGER BOT " << std::endl;
	Sleep(500);
	std::cout << "NUMPAD 5 = GLOW HACK" << std::endl;
	Sleep(500);
	std::cout << "NUMPAD 6 = RECOIL COMPENSATION" << std::endl;

	const auto ModuleBase = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));

	const auto Engine = reinterpret_cast<uintptr_t>(GetModuleHandle("engine.dll"));

	auto LocalPlayer = *reinterpret_cast<uintptr_t*>(ModuleBase + dwLocalPlayer);

	if (LocalPlayer == NULL)
	{
		while (LocalPlayer == NULL)
		{
			LocalPlayer = *reinterpret_cast<uintptr_t*>(ModuleBase + dwLocalPlayer);
		}
	}

	auto oldPuch = vector2{ };

	

	while (true)
	{

		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHop = !bHop;

			if (bHop)
			{
				std::cout << "bHop Active" << std::endl;
			}
			else
			{
				std::cout << "bHop Not Active" << std::endl;
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			AntiFlash = !AntiFlash;

			if (AntiFlash)
			{
				std::cout << "Anti Flash Active" << std::endl;
			}
			else
			{
				std::cout << "Anti Flash Not Active" << std::endl;
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			RadarHack = !RadarHack;

			if (RadarHack)
			{
				std::cout << "Radar Hack Active" << std::endl;
			}
			else
			{
				std::cout << "Radar Hack Not Active" << std::endl;
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			TriggerBot = !TriggerBot;

			if (TriggerBot)
			{
				std::cout << "Trigger Bot Active" << std::endl;
			}
			else
			{
				std::cout << "Tigger Bot Not Active" << std::endl;
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			Glow = !Glow;

			float brightNess = 25.0f;

			int Bright = *reinterpret_cast<int*>(Engine + model_ambient_min - 0x2C);

			int xorptr = *(int*)&brightNess ^ Bright;

			*reinterpret_cast<int*>(Engine + model_ambient_min) = xorptr;

			if (Glow)
			{
				std::cout << "Glow Hack Active" << std::endl;
			}
			else
			{
				std::cout << "Glow Hack Not Active" << std::endl;
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD6) & 1)
		{
			RCS = !RCS;

			if (RCS)
			{
				std::cout << "RCS Hack Active" << std::endl;
			}
			else
			{
				std::cout << "RCS Hack Not Active" << std::endl;
			}
		}

		if (RCS)
		{

			const auto ShotsFired = *reinterpret_cast<int*>(LocalPlayer + m_iShotsFired);

			if (ShotsFired)
			{
				const auto ClientState = *reinterpret_cast<uintptr_t*>(Engine + dwClientState);
				const auto veiwAngles = *reinterpret_cast<vector2*>(ClientState + dwClientState_ViewAngles);

				const auto aimPuch = *reinterpret_cast<vector2*>(LocalPlayer + m_aimPunchAngle);

				auto newAngles = vector2
				{
					veiwAngles.x + oldPuch.x - aimPuch.x * 2.f,
					veiwAngles.y + oldPuch.y - aimPuch.y * 2.f
				};

				if (newAngles.x > 89.f)
				{
					newAngles.x = 89.f;
				}
				if (newAngles.x < -89.f)
				{
					newAngles.x = -89.f;
				}
				if (newAngles.y > 180.f)
				{
					newAngles.y -= 360.f;
				}
				if (newAngles.y < -180.f)
				{
					newAngles.y += -360.f;
				}

				*reinterpret_cast<vector2*>(ClientState + dwClientState_ViewAngles) = newAngles;

				oldPuch.x = aimPuch.x * 2.f;
				oldPuch.y = aimPuch.y * 2.f;
			}
			else
			{
				oldPuch.x = oldPuch.y = 0.f;
			}

			
		}

		if (Glow)
		{
			DWORD GlowObject = *reinterpret_cast<DWORD*>(ModuleBase + dwGlowObjectManager);

			for (auto i = 0; i < 64; i++)
			{
				DWORD EntityList2 = *reinterpret_cast<uintptr_t*>(ModuleBase + dwEntityList + i * 0x10);

				if (EntityList2 != NULL)
				{

					if (*reinterpret_cast<int*>(EntityList2 + m_iTeamNum) == *reinterpret_cast<int*>(LocalPlayer + m_iTeamNum))
					{
						continue;
					}

					auto health2 = *reinterpret_cast<int*>(EntityList2 + m_iHealth);

					bool defusing = *reinterpret_cast<bool*>(EntityList2 + m_bIsDefusing);

					if (defusing)
					{
						red = 1.0f;
						blue = 1.0f;
						green = 1.0f;
					}
					else
					{
						red = health2 * -0.01 + 1;
						green = health2 * 0.01;
						blue = 0.0f;
					}

					DWORD GlowIndex = *reinterpret_cast<int*>(EntityList2 + m_iGlowIndex);

					*reinterpret_cast<int*>(EntityList2 + m_clrRender) = 255;
					*reinterpret_cast<int*>(EntityList2 + m_clrRender + 0x1) = 0;
					*reinterpret_cast<int*>(EntityList2 + m_clrRender + 0x2) = 0;

					*reinterpret_cast<float*>(GlowObject + (GlowIndex * 0x38) + 0x08) = red; //r
					*reinterpret_cast<float*>(GlowObject + (GlowIndex * 0x38) + 0x0C) = green; //g
					*reinterpret_cast<float*>(GlowObject + (GlowIndex * 0x38) + 0x10) = blue; //b
					*reinterpret_cast<float*>(GlowObject + (GlowIndex * 0x38) + 0x14) = 1.0f;//a
					*reinterpret_cast<int*>(GlowObject + (GlowIndex * 0x38) + 0x30) = 0;

					*reinterpret_cast<bool*>(GlowObject + (GlowIndex * 0x38) + 0x27) = true;
					*reinterpret_cast<bool*>(GlowObject + (GlowIndex * 0x38) + 0x28) = true;
				}
				
			}
		}

		if (GetAsyncKeyState(VK_MENU) == -32768 && !KeyHeld)
		{
			KeyHeld = true;
			TriggerBot = true;
			std::cout << "HELD" << std::endl;
		}
		if (GetAsyncKeyState(VK_MENU) == 0 && KeyHeld)
		{
			KeyHeld = false;
			TriggerBot = false;
			std::cout << "RELEASED" << std::endl;
		}

		if (TriggerBot)
		{
			Game.CrossHairInt = *reinterpret_cast<int*>(LocalPlayer + m_iCrosshairId);

			if (Game.CrossHairInt > 0 && Game.CrossHairInt < 64)
			{
				Game.EntityList = *reinterpret_cast<DWORD*>(ModuleBase + dwEntityList + ((Game.CrossHairInt - 1) * 0x10));

				Game.TeamFlag = *reinterpret_cast<int*>(LocalPlayer + m_iTeamNum);
				Game.dwFlags = *reinterpret_cast<int*>(Game.EntityList + m_iTeamNum);
				Game.Health = *reinterpret_cast<int*>(Game.EntityList + m_iHealth);
				if (Game.TeamFlag != Game.dwFlags && Game.Health > 0)
				{
					Shoot(ModuleBase);
				}
				else
				{
					continue;
				}
			}
		}

		if (RadarHack)
		{
			for (int i = 0; i < 64; i++)
			{
				DWORD EntityList = *reinterpret_cast<DWORD*>(ModuleBase + dwEntityList + i * 0x10);

				if (EntityList != NULL)
				{
					*reinterpret_cast<bool*>(EntityList + m_bSpotted) = true;
				}
			}
		}

		if (AntiFlash)
		{
			*reinterpret_cast<int*>(LocalPlayer + m_flFlashDuration) = 0;
		}

		if (bHop)
		{

			if(GetAsyncKeyState(VK_SPACE) && bHop)
			{
				std::cout << "Pressed!" << std::endl;

				DWORD LocalPlayerVel = *(DWORD*)(LocalPlayer + m_vecVelocity);
				vector PlayerVel = *(vector*)(LocalPlayer + m_vecVelocity);
				int vel = PlayerVel.x + PlayerVel.y + PlayerVel.z;


				if (LocalPlayer != NULL)
				{
					std::cout << "localPlayerPassed " << std::hex << LocalPlayer << std::endl << LocalPlayerVel << std::endl << PlayerVel.x;
					int Health = *(int*)(LocalPlayer + m_iHealth);
					int dwFlagsptr = *(int*)(LocalPlayer + m_fFlags);

					if (vel != 0)
					{
						std::cout << "velocityPassed " << std::dec << PlayerVel.x << std::endl;
						if (Health > 0 && dwFlagsptr == 257)
						{
							std::cout << "healthPassed" << Health << dwFlagsptr;
							Game.ForceJump(ModuleBase);
						}

					}

				}

			}
		}
		

		Sleep(1);
	}
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

int __stdcall DllMain(const HMODULE instance, const uintptr_t reason, const void* reserved)
{
	if (reason == 1)
	{
		DisableThreadLibraryCalls(instance);

		const auto Thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(HackThread), instance, 0, nullptr);

		if (Thread)
		{
			CloseHandle(Thread);
		}
	}

	return 1;
}
