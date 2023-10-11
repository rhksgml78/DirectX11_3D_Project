#pragma once
class Billboard : public Actor
{
public:
    Vector2			pivot;
    static Billboard* Create(string name = "Billboard");
    virtual void	Update() override;
    virtual void	Render() override;
    void			RenderDetail();
};

