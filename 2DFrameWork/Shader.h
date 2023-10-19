#pragma once
class Shader
{
    ID3D11InputLayout*      vertexLayout;
    ID3D11VertexShader*     vertexShader;
    ID3D11PixelShader*      pixelShader;
    ID3D11GeometryShader*   geometryShader;

    ID3D11ShaderReflection* reflection;

    ID3D11HullShader* hullShader = nullptr;
    ID3D11DomainShader* domainShader = nullptr;

public:
    VertexType              vertextype;
    string                  file;
    Shader();
    ~Shader();
    void LoadFile(string file);
    void LoadGeometry();
    void LoadTessellation();
    void Set();
};
