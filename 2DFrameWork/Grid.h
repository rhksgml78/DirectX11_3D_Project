#pragma once
//Ư���ϰ� �����̴� ����
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

