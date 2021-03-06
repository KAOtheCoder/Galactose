#pragma once
#include "GalactosePCH.h"
#include "Application.h"

namespace Galactose {
	class Window
	{
	public:
		static std::shared_ptr<Window> create(const std::string& a_title, const int32_t a_width, const int32_t a_height);
		static bool areAllWindowsClosed();

		virtual ~Window();

		virtual std::string title() const = 0;
		virtual int32_t width() const = 0;
		virtual int32_t height() const = 0;
		virtual bool isClosed() const = 0;

		virtual bool isVSync() const = 0;
		virtual void setVSync(const bool a_vsync) = 0;

		virtual void update() = 0;
		virtual void close() = 0;

		virtual void* nativeWindow() const = 0;

	protected:
		inline static std::vector<std::shared_ptr<Window>> s_windows;

		friend class Application;
	};
}