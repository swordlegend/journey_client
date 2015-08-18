//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
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
