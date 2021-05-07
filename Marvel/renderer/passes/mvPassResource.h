#pragma once

#include <string>
#include <memory>

namespace Marvel {

	class mvPassProduct;

	class mvPassResource
	{

	public:

		mvPassResource(const std::string& name);

		virtual ~mvPassResource() = default;

		virtual void bind(mvPassProduct& product) = 0;

		void setTarget(const std::string& pass, const std::string& product);

		const std::string& getName() const;
		const std::string& getPass() const;
		const std::string& getProduct() const;
		bool               isPreLinked() const { return m_prelinked; }
		bool               isLinked() const { return m_linked; }

	protected:

		std::string m_name;    // internal name
		std::string m_pass;    // pass where resource resides after linking
		std::string m_product; // name of resource (product) in linked pass

		bool m_prelinked = false;
		bool m_linked = false;

	};

	template<typename T>
	class mvBindPassResource : public mvPassResource
	{
		
	public:

		mvBindPassResource(const std::string& name, std::shared_ptr<T>& resource)
			:
			mvPassResource(name),
			m_resource(resource)
		{

		}

		void bind(mvPassProduct& product) override
		{
			auto ptr = std::dynamic_pointer_cast<T>(product.getBindable());
			m_resource = std::move(ptr);
			m_linked = true;
		}

	private:

		std::shared_ptr<T>& m_resource;

	};

	template<typename T>
	class mvBufferPassResource : public mvPassResource
	{

	public:

		mvBufferPassResource(const std::string& name, std::shared_ptr<T>& resource)
			:
			mvPassResource(name),
			m_resource(resource)
		{

		}

		void bind(mvPassProduct& product) override
		{
			auto ptr = std::dynamic_pointer_cast<T>(product.getBuffer());
			m_resource = std::move(ptr);
			m_linked = true;
		}

	private:

		std::shared_ptr<T>& m_resource;

	};

}