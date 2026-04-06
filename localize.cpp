#include "localize.hpp"
#include <clocale>
#include <filesystem>
#include <string>

#if defined(_WIN32)
	#include <windows.h>
#elif defined(__APPLE__)
	#include <mach-o/dyld.h>
#endif

void localizeInit() {
	setlocale(LC_ALL, "");
	bindtextdomain(
		"craftive",
		[&] -> std::string {
			#ifdef _CRAFTIVE_DEBUG
				#ifdef _CRAFTIVE_DEBUGGING_LOCALES_DIR
					return _CRAFTIVE_DEBUGGING_LOCALES_DIR;
				#else
					return (std::filesystem::current_path() / "locales").string();
				#endif
			#else
				#if defined(_WIN32) || defined(__APPLE__)
					std::filesystem::path epath;
					#if defined(_WIN32)
						wchar_t wpath[MAX_PATH]{};
						GetModuleFileNameW(nullptr, wpath, MAX_PATH);
						epath = wpath;
					#elif defined(__APPLE__)
						char path[PATH_MAX];
						auto size = sizeof(path);
						_NSGetExecutablePath(path, &size);
						epath = path;
					#endif
					if (epath.empty())
					#if defined(_WIN32)
						return (std::filesystem::current_path() / "locales").string();
					#else
						return "/usr/share/locale";
					#endif
					auto dir = epath.parent_path();
					#if defined(__APPLE__)
						dir = dir.parent_path() / "Resources";
					#endif
					return (dir / "locales").string();
				#else
					return "/usr/share/locale";
				#endif
			#endif
		}().c_str()
	);
	bind_textdomain_codeset("craftive", "UTF-8");
	textdomain("craftive");
}
