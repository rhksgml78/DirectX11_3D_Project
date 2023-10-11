#pragma once
class MaterialBuffer
{
public:
	Color	ambient;
	Color	diffuse;
	Color	specular;
	Color	emissive;
	float	shininess;
	float	opacity;
	float   environment;
	float	shadow;
};
class Material : public MaterialBuffer
{
	static ID3D11Buffer* materialBuffer;
public:
	static void CreateStaticMember();
	static void DeleteStaticMember();
public:
	shared_ptr<Texture>			normalMap;
	shared_ptr<Texture>			diffuseMap;
	shared_ptr<Texture>			specularMap;
	shared_ptr<Texture>			emissiveMap;
	string						file;
public:
	Material();
	~Material();

	void RenderDetail();
	virtual void Set();
	void LoadFile(string file);
	void SaveFile(string file);
};

