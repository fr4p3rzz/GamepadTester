#include <dxgi1_6.h>
#include <d3d12.h>
#include <iostream>
#include <Xinput.h>
#include <chrono>
#include <thread>

DWORD NumberOfControllers(XINPUT_STATE state)
{
	DWORD connectedControllers = 0;
	for (DWORD i=0; i< XUSER_MAX_COUNT; i++ )
	{

		// Simply get the state of the controller from XInput.
		DWORD dwResult = XInputGetState( i, &state );

		if( dwResult == ERROR_SUCCESS )
		{
			std::cout << "controller is connected at index " << i << std::endl;
			connectedControllers++;			
		}
		else
		{
			std::cout << "controller is NOT connected at index " << i << std::endl;
		}
	}

	return connectedControllers;
}

void CheckBatteryStatus(DWORD i, XINPUT_BATTERY_INFORMATION batteryInformation)
{
	DWORD battery = XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batteryInformation);
	if (battery == ERROR_SUCCESS)
	{
		switch (batteryInformation.BatteryLevel)
		{
		case BATTERY_LEVEL_FULL:
			std::cout << "Battery status: FULL (70-100%)" << std::endl;
			break;
		case BATTERY_LEVEL_MEDIUM:
			std::cout << "Battery status: MEDIUM (40-70%)" << std::endl;
			break;
		case BATTERY_LEVEL_LOW:
			std::cout << "Battery status: LOW (10-40%)" << std::endl;
			break;
		case BATTERY_LEVEL_EMPTY:
			std::cout << "Battery status: EMPTY (0-10%)" << std::endl;
			break;
		default:
			std::cout << "Error: cannot retrieve battery informations" << std::endl;
			break;
		}
	}
	else
	{
		std::cout << "Error: Cannot retrieve battery informations" << std::endl;
	}
}

void GetInput(DWORD i, XINPUT_KEYSTROKE key, XINPUT_VIBRATION vibration)
{
	bool getInput = true;
	while (getInput)
	{
		DWORD inputReceived = XInputGetKeystroke(i, 0, &key);
		if (inputReceived == ERROR_SUCCESS)
		{
			switch (key.VirtualKey)
			{
			    case VK_PAD_A:
					std::cout << "a" << std::endl;
					break;
				case VK_PAD_B:
					std::cout << "b" << std::endl;
					break;
				case VK_PAD_X:
					std::cout << "x" << std::endl;
					break;
				case VK_PAD_Y:
					std::cout << "y" << std::endl;
					break;
				case VK_PAD_DPAD_UP:
					if (vibration.wLeftMotorSpeed > 0 && vibration.wRightMotorSpeed > 0)
					{
						vibration.wLeftMotorSpeed = 0; // Any value between 0-65535 here
						vibration.wRightMotorSpeed = 0; // Any value between 0-65535 here
						XInputSetState(i, &vibration);
						std::cout << "Vibration stopped" << std::endl;
					}
					else
					{
						std::cout << "Vibration is inactive" << std::endl;
					}
					break;
				case VK_PAD_DPAD_DOWN:
					if (vibration.wLeftMotorSpeed == 0 && vibration.wRightMotorSpeed == 0)
					{
						vibration.wLeftMotorSpeed = 32000; // Any value between 0-65535 here
						vibration.wRightMotorSpeed = 16000; // Any value between 0-65535 here
						XInputSetState(i, &vibration);
						std::cout << "Vibration activated" << std::endl;
					}
					else
					{
						std::cout << "Vibration is already active" << std::endl;
					}
					break;
				case VK_PAD_DPAD_LEFT:
					if (vibration.wLeftMotorSpeed == 0)
					{
						vibration.wLeftMotorSpeed = 32000; // Any value between 0-65535 here
						XInputSetState(i, &vibration);
						std::cout << "Left vibration activated" << std::endl;
					}
					else
					{
						vibration.wLeftMotorSpeed = 0; // Any value between 0-65535 here
						std::cout << "Left vibration deactivated" << std::endl;
					}
					break;
				case VK_PAD_DPAD_RIGHT:
					if (vibration.wRightMotorSpeed == 0)
					{
						vibration.wRightMotorSpeed = 16000; // Any value between 0-65535 here
						XInputSetState(i, &vibration);
						std::cout << "Right vibration activated" << std::endl;
					}
					else
					{
						vibration.wRightMotorSpeed = 0; // Any value between 0-65535 here
						std::cout << "Right vibration deactivated" << std::endl;
					}
					break;
				case VK_PAD_START:
					std::cout << "exiting" << std::endl;
					getInput = false;
					break;
				default:
					break;
			}
		}
	}
}

int main(int argc, char** argv)
{
	static XINPUT_STATE state{};
	static XINPUT_BATTERY_INFORMATION batteryInformation{};
	static XINPUT_KEYSTROKE key{};
	static XINPUT_VIBRATION vibration{};

	DWORD connectedControllers = NumberOfControllers(state);
	DWORD selectedController = 0;
	DWORD selectedInput = 0;


	if (connectedControllers < 1)
	{
		std::cout << "No device found, shutting down" << std::endl;
	}
	else
	{
		if (connectedControllers == 1)
		{
			selectedController = 0;
		}
		else
		{
			std::cout << "select input port between 0 and " << connectedControllers-1 << std::endl;

			do {
				std::cin >> selectedInput;
			} while (selectedInput > connectedControllers - 1 && !std::cin);

		}

		CheckBatteryStatus(selectedController, batteryInformation);
		GetInput(selectedInput, key, vibration);
	}




	return 0;
}

