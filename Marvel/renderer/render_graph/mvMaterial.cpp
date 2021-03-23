#include "mvMaterial.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "assimp/Importer.hpp"
#include "assimp/Scene.h"
#include "assimp/postprocess.h"

namespace Marvel {

	mvMaterial::mvMaterial(mvGraphics& graphics, const aiMaterial& material, const std::string& path)
	{

		std::shared_ptr<mvPixelConstantBuffer> buf;
		std::unique_ptr<mvBuffer>              bufferRaw;

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Bool, std::string("useGlossAlpha"));
		root->add(Bool, std::string("useSpecularMap"));
		root->add(Float3, std::string("specularColor"));
		root->add(Float, std::string("specularWeight"));
		root->add(Float, std::string("specularGloss"));
		root->add(Bool, std::string("useNormalMap"));
		root->add(Float, std::string("normalMapWeight"));
		root->finalize(0);

		bufferRaw = std::make_unique<mvBuffer>(std::move(layout));
		bufferRaw->getElement("useGlossAlpha") = false;
		bufferRaw->getElement("useSpecularMap") = true;
		bufferRaw->getElement("specularColor") = glm::vec3{ 1.0f, 1.0f, 1.0f };
		bufferRaw->getElement("specularWeight") = 1.0f;
		bufferRaw->getElement("specularGloss") = 15.0f;
		bufferRaw->getElement("useNormalMap") = true;
		bufferRaw->getElement("normalMapWeight") = 1.0f;

		buf = std::make_shared<mvPixelConstantBuffer>(graphics, *root.get(), 1, bufferRaw.get());

		// create vertex layout
		Marvel::mvVertexLayout vertexLayout;
		vertexLayout.append(ElementType::Position3D);
		vertexLayout.append(ElementType::Normal);
		vertexLayout.append(ElementType::Texture2D);
		vertexLayout.append(ElementType::Tangent);
		vertexLayout.append(ElementType::Bitangent);

		mvStep step("Lambertian");

		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{

			step.addBindable(mvBindableRegistry::GetBindable("sampler"));
			step.addBindable(std::make_shared<mvTexture>(graphics, path + texFileName.C_Str()));
		}
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			step.addBindable(std::make_shared<mvTexture>(graphics, path + texFileName.C_Str(), 1));
		}
		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			step.addBindable(std::make_shared<mvTexture>(graphics, path + texFileName.C_Str(), 2));
		}

		// create vertex shader
		auto vshader = mvBindableRegistry::GetBindable("vs_texture");

		step.addBindable(vshader);
		step.addBindable(std::make_shared<mvInputLayout>(graphics, vertexLayout,
			static_cast<mvVertexShader*>(vshader.get())));
		step.addBindable(mvBindableRegistry::GetBindable("ps_texture"));
		step.addBindable(mvBindableRegistry::GetBindable("gs_null"));
		step.addBindable(mvBindableRegistry::GetBindable("transCBuf"));
		step.addBindable(mvBindableRegistry::GetBindable("blender"));
		step.addBindable(std::make_shared<mvRasterizer>(graphics, true));

		step.addBindable(buf);

		m_steps.push_back(step);
	}

	std::vector<mvStep> mvMaterial::getSteps() const
	{
		return m_steps;
	}

}