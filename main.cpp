#include "../include/Hooking.h"

void R_AddCmdDrawStretchPic(float X, float Y, float Width, float Height, int UK1, int UK2, int UK3, int UK4, const float *Color, int Material)
{
	char *material = *(char **)Material;

	char welcomeMessage[100];
	snprintf(welcomeMessage, sizeof(welcomeMessage), "UK1: %i; UK2: %i; UK3: %i; UK4: %i; Material: %i", UK1, UK2, UK3, UK4, Material);
	sceSysUtilSendSystemNotificationWithText(222, welcomeMessage);

	R_AddCmdDrawStretchPic_s(X, Y, Width, Height, UK1, UK2, UK3, UK4, Color, Material);
}

void CL_WritePacketHook(int LocalClientNum)
{
	if (Key_IsDown(BUTTON_DPAD_RIGHT))
	{
		if ((LevelTime - PreviousLevelTime) > 1)//Delay
		{
			PreviousLevelTime = LevelTime;
			*(int *)(0x76A86BC + 0x04) ^= 1;//getPlayerState::mFlag 
		}
	}
	if (Key_IsDown(BUTTON_DPAD_LEFT))
		((void (*)(const char *, ...))0xAC5510)("");

	CL_WritePacket_s(LocalClientNum);
}

void Menu_PaintAll(uint64_t UIContext)
{
	DrawText("Sample Text", 0x7FFFFFFF, 0xE678D88, 27, White, 0, -1, 0, 0, 1080, 400, 1, 1, 0);
	DrawShader(300, 300, 200, 200, 0, 0, 1, 1, White, DB_FindXAssetHeader(XASSET_TYPE_MATERIAL, "white", 1));

	Menu_PaintAll_s(UIContext);
}

void GameCheck()
{
	if (!strcmp((char *)0xF8B682, "multiplayer")) //MWR 1.02
	{
		CL_WritePacket_s = (CL_WritePacket_t)DetourFunction(0x7BEBA0, (void *)CL_WritePacketHook, 17);
		Menu_PaintAll_s = (Menu_PaintAll_t)DetourFunction(0xC5CAF0, (void *)Menu_PaintAll, 16);
		*(int *)0x72EE42 = 0x01AB7D80;
		XNotify("[MWR 1.02 MP] Successfully Hooked.");
	}
	else
		XNotify("[Paradigm]: Invalid Title or Update!");
}

extern "C" void _main(void)
{
	initImports();
	GameCheck();
}