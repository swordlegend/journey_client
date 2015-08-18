#pragma once
#include "Journey.h"

HANDLE gTimerStopEvent;
HANDLE gTimerDoneEvent;
HANDLE hTimer = NULL;
HANDLE hTimerQueue = NULL;

packetcreator packet_c;
winapp app;
session server;
settings config;

int result = 0;

void quit()
{
	result = 1;
	PostQuitMessage(0);
}

void CALLBACK Update(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
	if (WaitForSingleObject(gTimerStopEvent, 1) != WAIT_TIMEOUT)
	{
		SetEvent(gTimerDoneEvent);
		return;
	}
	WaitForSingleObject(gTimerDoneEvent, INFINITY);
	SetEvent(gTimerDoneEvent);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	result = server.init();
	packet_c.init(&server);

	if (result == 0)
	{
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

		gTimerStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		gTimerDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		hTimerQueue = CreateTimerQueue();

		result = CoInitialize(NULL);

		if (result == 0)
		{
			result = app.init();

			if (result == 0)
			{
				CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)Update, NULL, 60, DPF, 0);

				MSG winmsg;
				while (result == 0)
				{
					WaitForSingleObject(gTimerDoneEvent, DPF);

					result = server.receive();

					if (GetMessage(&winmsg, NULL, 0, 0))
					{
						TranslateMessage(&winmsg);
						DispatchMessage(&winmsg);
					}
				}

				SetEvent(gTimerStopEvent);
				WaitForSingleObject(gTimerDoneEvent, 1000);

				if (result > 1)
				{
					string error = "Runtime error:\n";
					switch (result)
					{
					case 2:
						error.append("You have been disconnected.");
						break;
					}
					MessageBox(NULL, error.c_str(), NULL, MB_OK);
				}
			}
			else
			{
				MessageBox(NULL, (LPCSTR)"Failed to initialize windows app.", NULL, MB_OK);
			}

			CoUninitialize();
		}
	}
	else
	{
		string error = "Connection failure:\n";
		switch (result)
		{
		case 1:
			error.append("Could not connect to server.");
			break;
		case 2:
			error.append("Incorrect handshake packet.");
			break;
		}
		MessageBox(NULL, error.c_str(), NULL, MB_OK);
	}

	return 0;
}
