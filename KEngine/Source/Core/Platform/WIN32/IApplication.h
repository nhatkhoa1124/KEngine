#pragma once

namespace Win32
{
	class KENGINE_API IApplication
	{
	public:
		IApplication() = default;
		virtual ~IApplication() = default;
		virtual void SetupPerGameSettings() = 0;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;
	};
}

extern Win32::IApplication* EntryApplication();