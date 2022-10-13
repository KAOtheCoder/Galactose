#pragma once

namespace Galactose {
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void onUpdate() {}
	};
}
