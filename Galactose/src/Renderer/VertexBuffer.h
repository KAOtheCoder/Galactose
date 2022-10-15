#pragma once

#include "GalactosePCH.h"

namespace Galactose {
	enum class DataType;
	class DataTypeInfo;

	class VertexBuffer {
	public:
		class Layout {
		public:
			class Attribute {
			public:
				Attribute(const std::string& name, const DataType type);

				const DataTypeInfo& typeInfo() const;
				size_t offset() const { return m_offset; }

			private:
				DataType m_type;
				std::string m_name;
				int32_t m_offset;

				friend class Layout; // Layout sets offset
			};

			Layout(std::initializer_list<Attribute> elements);
			Layout() = default;

			std::vector<Attribute>::const_iterator begin() const { return m_elements.begin(); }
			std::vector<Attribute>::const_iterator end() const { return m_elements.end(); }

			bool empty() const { return m_elements.empty(); }

			int32_t stride() const { return m_stride; }

		private:
			std::vector<Attribute> m_elements;
			int32_t m_stride = 0;
		};

		static std::shared_ptr<VertexBuffer> create(const void* data, const uint32_t byteSize);
		static std::shared_ptr<VertexBuffer> create(const void* data, const uint32_t count, const Layout& layout);

		virtual ~VertexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		Layout layout() const { return m_layout; }
		void setLayout(const Layout& a_layout);

	protected:
		VertexBuffer() = default;

	private:
		Layout m_layout;
	};
}
