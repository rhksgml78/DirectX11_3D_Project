#pragma once
//특수하게 움직이는 액터
class Grid : public Actor
{
public:
    static Grid* Create(string name = "Grid");
private:
    Grid();
    virtual ~Grid();
public:
    void	Update() override;
    void	Release() override;

};

