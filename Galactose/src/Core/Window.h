#pragma once

#include "Layer.h"

#include <string>
#include <vector>

namespace Galactose {
	class Application;

	class Window : public std::enable_shared_from_this<Window>
	{
	public:
		static std::shared_ptr<Window> create(const std::string& a_title, const int32_t a_width, const int32_t a_height);
		static bool areAllWindowsClosed();

		static Window* getCurrentContext();
		static void setCurrentContext(Window* context);

		virtual ~Window() = default;

		virtual std::string title() const = 0;
		virtual int32_t width() const = 0;
		virtual int32_t height() const = 0;
		virtual bool isClosed() const = 0;

		virtual bool vsync() const = 0;
		virtual void setVSync(const bool a_vsync) = 0;

		virtual void update() = 0;
		virtual void close() = 0;

		virtual void* nativeWindow() const = 0;

		void onEvent(const std::shared_ptr<Event>& event);

		std::vector<std::shared_ptr<Layer>> layers;

	protected:
		inline static std::vector<std::shared_ptr<Window>> s_windows;

		Window() = default;

	private:
		friend class Application;
	};
}
