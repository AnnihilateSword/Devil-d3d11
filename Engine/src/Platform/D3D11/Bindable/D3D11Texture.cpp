#include "EnginePCH.h"

#include "D3D11Texture.h"
#include "Vendor/stb_image.h"

namespace Devil
{
    D3D11Texture::D3D11Texture(D3D11Renderer& renderer, const std::string filename, unsigned int slot)
        : m_Slot{ slot }
    {
        // load texture data from file. ( stb_image )
        int imageWidth{};
        int imageHeight{};
        int imageChannels{};
        int imageDesiredChannels = 4;
        unsigned char* imageData = stbi_load(filename.c_str(),
            &imageWidth, &imageHeight, &imageChannels, imageDesiredChannels);
        assert(imageData);
        int imagePitch = imageWidth * 4;

        // create d3d11 texture resources.
        D3D11_TEXTURE2D_DESC td{};
        td.ArraySize = 1u;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0u;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.Height = imageHeight;
        td.Width = imageWidth;
        td.MipLevels = 1u;
        td.MiscFlags = 0u;
        td.SampleDesc.Count = 1u;
        td.SampleDesc.Quality = 0u;
        td.Usage = D3D11_USAGE_DEFAULT;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D{};
        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = imageData;
        sd.SysMemPitch = imagePitch;

        GetDevice(renderer)->CreateTexture2D(&td, &sd, &pTexture2D);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
        srvd.Format = td.Format;
        srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2D.MipLevels = 1u;
        srvd.Texture2D.MostDetailedMip = 0u;
        GetDevice(renderer)->CreateShaderResourceView(pTexture2D.Get(), &srvd, &m_TextureView);
    }

    void D3D11Texture::Bind(D3D11Renderer& renderer) noexcept
    {
        GetDeviceContext(renderer)->PSSetShaderResources(m_Slot, 1u, m_TextureView.GetAddressOf());
    }
}