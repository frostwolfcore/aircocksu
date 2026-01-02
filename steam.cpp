#include "steam.hpp"
#include "globals.hpp"

c_steam_sdk g_steam_sdk;

typedef uint32_t SteamPipeHandle;
typedef uint32_t SteamUserHandle;

void c_steam_sdk::initialize_local()
{
	SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)())GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
	SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)())GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();

	SteamClient = ((ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();
	SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamGameCoordinator001");
	SteamUser = (ISteamUser*)SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
	SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
	SteamUtils = SteamClient->GetISteamUtils(hSteamPipe, "SteamUtils009");

	if (SteamUser)
	{
		int iAvatar = SteamFriends->GetLargeFriendAvatar(SteamUser->GetSteamID());
		if (iAvatar != -1 && iAvatar != 0)
		{
			uint32 width = 0, height = 0;
			if (SteamUtils->GetImageSize(iAvatar, &width, &height) && width > 0 && height > 0)
			{
				const int size_in_bytes = width * height * 4;
				std::vector<uint8_t> rgba_buffer(size_in_bytes);
				if (SteamUtils->GetImageRGBA(iAvatar, rgba_buffer.data(), size_in_bytes))
				{
					for (size_t i = 0; i < rgba_buffer.size(); i += 4) {
						std::swap(rgba_buffer[i], rgba_buffer[i + 2]);
					}
					BITMAPFILEHEADER file_header;
					BITMAPINFOHEADER info_header;

					ZeroMemory(&file_header, sizeof(file_header));
					ZeroMemory(&info_header, sizeof(info_header));

					file_header.bfType = 0x4D42; // "BM"
					file_header.bfSize = sizeof(file_header) + sizeof(info_header) + size_in_bytes;
					file_header.bfOffBits = sizeof(file_header) + sizeof(info_header);

					info_header.biSize = sizeof(info_header);
					info_header.biWidth = width;
					info_header.biHeight = -(LONG)height;
					info_header.biPlanes = 1;
					info_header.biBitCount = 32;
					info_header.biCompression = BI_RGB;

					std::string& avatar_storage = HACKS->cheat_info.user_avatar;
					avatar_storage.resize(file_header.bfSize);

					memcpy(avatar_storage.data(), &file_header, sizeof(file_header));
					memcpy(avatar_storage.data() + sizeof(file_header), &info_header, sizeof(info_header));
					memcpy(avatar_storage.data() + sizeof(file_header) + sizeof(info_header), rgba_buffer.data(), size_in_bytes);
				}
			}
		}
	}

	HACKS->cheat_info.user_name = SteamFriends->GetPersonaName();
}