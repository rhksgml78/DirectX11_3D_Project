#pragma once
class Solar : public Actor
{
public:
    static Solar* Create(string name = "Solar");
private:
    Solar();
    virtual ~Solar();
public:
    void	Update() override;
    void	Release() override;

};

