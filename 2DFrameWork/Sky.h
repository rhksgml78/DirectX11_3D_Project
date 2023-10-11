#pragma once
class Sky : public Actor
{
public:
	shared_ptr<Texture> texCube;
	static Sky* Create(string name = "Sky");
	virtual void	Render() override;
	virtual void	Render(shared_ptr<Shader> shader) override;
	//virtual void	CubeMapRender() override;
};
