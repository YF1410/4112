#include "TextWrite.h"
#include"Library.h"
#include"LibWinAPI.h"

using namespace MelLib;

ComPtr<IDWriteFactory> TextWrite::dWriteFactory;
std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>TextWrite::pTextFormat;

ComPtr<ID3D11Device> TextWrite::d3d11Device;
ComPtr<ID3D11DeviceContext>TextWrite::d3d11context;
ComPtr<ID3D11On12Device>TextWrite::d3d11On12device;
ComPtr<ID3D11Resource> TextWrite::wrappedBackBuffer[2];

ComPtr<ID2D1Factory3> TextWrite::d2dFactory;
ComPtr<ID2D1Device2>TextWrite::d2dDevice;
ComPtr<IDXGIDevice>TextWrite::dxgiDevice;
ComPtr<ID2D1DeviceContext> TextWrite::d2dContext;
ComPtr<ID2D1Bitmap1>TextWrite::d2dRenderTerget[2];

ComPtr<ID2D1HwndRenderTarget> TextWrite::d2dRenderTarget;
ComPtr<ID2D1SolidColorBrush> TextWrite::d2dSolidColorBrush;
HWND TextWrite::hwnd;

std::vector<std::wstring> TextWrite::tests;
std::vector<std::string> TextWrite::fontNames;
std::vector<std::tuple<Vector2, Color,/* TextWrite::TextAlignment,*/std::wstring, std::string>> TextWrite::drawTextDatas;

LRESULT TextWrite::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    /* switch (msg)
     {
     case WM_DESTROY:
         PostQuitMessage(0);
         return 0;
     }*/
    return DefWindowProc(hwnd, msg, wparam, lparam);

}


bool TextWrite::Initialize
(
    ID3D12Device* pD3D12Device,
    ID3D12CommandQueue** pPD3D12Queue,
    ID3D12Resource* pBuckBuffers[2]
)
{


    UINT flag =
        D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    auto result = D3D11On12CreateDevice
    (
        pD3D12Device,
        flag,
        nullptr,//nullptr�ɂ����D3D12�̕��Ɠ������x���ɂȂ�
        0,//��̈����̔z��(����͂����Ŏw�肵�Ȃ�����0)
        reinterpret_cast<IUnknown**>(pPD3D12Queue),//D3D12�̃L���[�̃|�C���^���L���X�g�������̂ł���?
        1,//�L���[�̐�
        0,//D3D12�f�o�C�X�̃m�[�h
        &d3d11Device,
        &d3d11context,
        nullptr//�I�΂ꂽ���x�����擾����ꍇ�|�C���^��n��
    );

    //���꒲�ׂ�
    //https://docs.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(q)
    //https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
    //�`���[�g���A���ɂ́uID3D11Device �̍쐬��A�������� ID3D11On12Device �C���^�[�t�F�C�X���Ɖ�ł��܂��B
    //����́AD2D ��ݒ肷�邽�߂Ɏg�p�����v���C�}�� �f�o�C�X �I�u�W�F�N�g�ł��B�v
    //�Ə�����Ă���
    //�����ɓn�������nullptr����Ȃ��Ȃ邩��A
    //As���Ă�ID3D�I�u�W�F�N�g�Ɠ����ݒ��Create������Ă���?
    result = d3d11Device.As(&d3d11On12device);

    result = D2D1CreateFactory
    (
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        IID_PPV_ARGS(&d2dFactory)
    );

    result = d3d11On12device.As(&dxgiDevice);

    //D3D11_CREATE_DEVICE_DEBUG�̂����Ő����ł��ĂȂ�����
    result = d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);

    result = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);

    IDWriteFactory** pPWriteFactory = &dWriteFactory;
    result = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(pPWriteFactory)
    );  


    //1�C���`(2.54cm)������̃h�b�g���̎擾
    float dpiX, dpiY;
    d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    //�r�b�g�}�b�v�I�v�V������J���[�R���e�L�X�g���̐ݒ�?
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1
    (
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,//�r�b�g�}�b�v�𐶐����邽�߂�DXGI�`���ƃA���t�@���[�h
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpiX,
        dpiY
    );

    //�o�b�N�o�b�t�@�����[�v
    for (int i = 0; i < 2; i++)
    {
        D3D11_RESOURCE_FLAGS d3d11Flags = {};
        //D3D12�̃��\�[�X���AD3D11�������ł���悤�ɂ��邽�߂̂���(����̏ꍇ�̓o�b�N�o�b�t�@������D3D11_BIND_RENDER_TARGET���w��?)
        d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET;

        //12�̃o�b�N�o�b�t�@�����Ƃ�11�̃o�b�N�o�b�t�@�𐶐�?
        //�u���̃��\�b�h�́AD3D11on12�Ŏg�p����D3D11���\�[�X���쐬���܂��B�v�h�L�������g���
        //�u�����_�[ �^�[�Q�b�g��Ԃ� "IN" ��ԂƂ��A�\����Ԃ� "OUT" ��ԂƂ��Ďw��v�`���[�g���A�����
        //�����ɏ�����Ă���D3D12_RESOURCE_STATE�������Ŕ��f�����Ă����?(�߂����߂���ƃG���[�o��)
        //GPU�f�o�b�K�ɂĕ����`�施�߂̌�ɖ߂��Ă閽�߂��������B�����Ŗ߂��Ă���ۂ��B
        //GPU�f�o�b�K�ł́AD3D12_RESOURCE_STATE_PRESENT�ł͂Ȃ�D3D12_RESOURCE_STATE_COMMON�Ə������
        //DirectX11�ł̓��\�[�X�o���A�������I�ł͂Ȃ�?�����œ���?
        //https://hexadrive.jp/hexablog/program/13072/�@
        result = d3d11On12device->CreateWrappedResource
        (
            pBuckBuffers[i],
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(&wrappedBackBuffer[i])
        );

        //�u���̃o�b�N�o�b�t�@�ɒ��ڕ`�悷��D2D�̃����_�[�^�[�Q�b�g���쐬���܂��B�v�`���[�g���A���̕�
        //IDXGISurface�C���^�[�t�F�C�X�́A�摜�f�[�^�I�u�W�F�N�g�̃��\�b�h���������܂��B(�h�L�������g�̐�����|�󂵂���)
        ComPtr<IDXGISurface>dxgiSurface;
        result = wrappedBackBuffer[i].As(&dxgiSurface);
        result = d2dContext->CreateBitmapFromDxgiSurface
        (
            dxgiSurface.Get(),
            &bitmapProperties,
            &d2dRenderTerget[i]
        );

        //Offset�́A�|�C���^�����炷���̂��ۂ�(�w�b�_�t�@�C���݂��炻��Ȋ���������)
        //(���炷��,���炷�T�C�Y)
        //ptr += ����1 * ����2
        //�`���[�g���A���ł́A�n���h����z��Ɋi�[���ĂȂ��̂ŁA���炳�Ȃ���2�ڂ�RTV�r���[�����Ȃ�����Offset�֐����g���Ă���
        //�������������̏ꍇ�����Ńr���[���Ȃ�����n���h���󂯎��K�v�Ȃ�
        //backBufferHandle[i].Offset(1, pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    }

    //�r���ŐF�ς�������ۂ�
    result = d2dContext->CreateSolidColorBrush
    (
        D2D1::ColorF(0, 0, 0, 1), 
        &d2dSolidColorBrush
    );


    //result = d2dFactory->CreateHwndRenderTarget
    //(
    //    D2D1::RenderTargetProperties(),
    //    D2D1::HwndRenderTargetProperties(Library::GetHWND(), D2D1::SizeU(Library::GetWindowWidth(), Library::GetWindowHeight())),
    //    &d2dRenderTarget
    //);

    //result = d2dContext->CreateSolidColorBrush
    //(
    //    D2D1::ColorF(D2D1::ColorF::Black),
    //    &d2dSolidColorBrush
    //);





    return true;
}

void TextWrite::LoopStartProcess()
{
    drawTextDatas.clear();
}

void TextWrite::LoopEndProcess(const UINT rtIndex)
{
    //���ꏑ���ƃ��\�[�X�o���A�؂�ւ����Ȃ��ăG���[�o��
    //�R�}���h���X�g��Close�ɂ�����ɌĂяo���Ȃ��Ƃ����Ȃ��̂Ńe�L�X�g�`�悵�Ȃ��ꍇ�Ɏ����Ŏ������Ƃ��s��
    //�e�L�X�g�`�悵�Ȃ��Ă����ɂ����A�̏��������Ȃ��Ƃ����Ȃ�
    //if (drawTextDatas.size() == 0)return;

    //D3D11on12�Ŏg�p����D3D11���\�[�X(�o�b�N�o�b�t�@)���Z�b�g(d3d11On12device�ɃZ�b�g)
    d3d11On12device->AcquireWrappedResources(wrappedBackBuffer[rtIndex].GetAddressOf(), 1);

    //�����_�[�^�[�Q�b�g�Z�b�g
    d2dContext->SetTarget(d2dRenderTerget[rtIndex].Get());
    d2dContext->BeginDraw();
    d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

    //�`��
    for (const auto& d : drawTextDatas)
    {
        
        Color color = std::get<1>(d);
        d2dSolidColorBrush->SetColor(D2D1::ColorF(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.f));


        Vector2 pos = std::get<0>(d);
        IDWriteTextFormat* pFormat = pTextFormat[std::get<3>(d)].Get();
       /* DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
        switch (std::get<2>(d))
        {
        case TextAlignment::LEFT_JUSTIFIED:
            alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
            break;
        case  TextAlignment::RIGHT_JUSTIFIED:
            alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
            break;
        case TextAlignment::POINT:


            alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
            break;
        }
        pFormat->SetTextAlignment(alignment);*/

        D2D1_RECT_F layoutRect = D2D1::RectF(pos.x, pos.y, Library::GetWindowWidth(), Library::GetWindowHeight());

        const std::wstring& text = std::get<2>(d);
        
        if(!pFormat)
        {
            std::wstring outputWStr = L"�e�L�X�g�u";
            outputWStr += text.c_str();
            outputWStr += L"�v�̕`��Ɏ��s���܂����B���݂��Ă��Ȃ�FontData���w�肵�Ă���\��������܂��B\n";
            OutputDebugStringW(outputWStr.c_str());

            std::string outputStr = "FontData���u";
            outputStr += std::get<3>(d);
            outputStr += "�v\n";
            OutputDebugStringA(outputStr.c_str());

            continue;
        }

        d2dContext->DrawTextW
        (
            text.c_str(),
            text.size(),
            pFormat,
            layoutRect,
            d2dSolidColorBrush.Get()
        );

        
    }

    auto result = d2dContext->EndDraw();

    //D3D11on12�p�Ƀ��b�v���ꂽD3D11���\�[�X�̉��
    //�uReleaseWrappedResources ���Ăяo�����ƂŁA
    //�쐬���Ɏw�肵�� "OUT" ��ԂɎw��̃��\�[�X���ڍs�����郊�\�[�X �o���A��w��Ŕ��������܂��B�v�`���[�g���A�����
    //�uFlush���Ăяo���O�ɂ��̃��\�b�h���Ăяo���āA
    //���\�[�X�o���A��K�؂ȁu�A�E�g�v��Ԃɑ}�����A���\�[�X�o���A���u�C���v��Ԃł���Ɨ\�z����邱�Ƃ��}�[�N���܂��B�v
    //0�܂�16�t���[���ڂɂ߂��Ⴍ���Ⴡ���������Ă����B�Ȃ������ׂ�
    //���������ɂ��Ȃ�̃������̃I�[�o�[�w�b�h������Ə����Ă��������炻��?
    d3d11On12device->ReleaseWrappedResources(wrappedBackBuffer[rtIndex].GetAddressOf(), 1);

    //D3D12�̃R�}���h�L���[�ɑ��M
    //�u �Ō�ɁA11On12 �f�o�C�X�Ŏ��s����邷�ׂẴR�}���h�����L ID3D12CommandQueue �ɑ��M���邽�߂ɁA
    //ID3D11DeviceContext �� Flush ���Ăяo���K�v������܂��B�v�`���[�g���A�����
    d3d11context->Flush();
}


bool TextWrite::CreateFontData(const std::wstring& fontName,const float size,const std::string& name)
{
    ComPtr<IDWriteTextFormat>pWriteTextFormat;

    //https://docs.microsoft.com/ja-jp/windows/win32/api/dwrite/nf-dwrite-idwritefactory-createtextformat
  
    auto result = dWriteFactory->CreateTextFormat
    (
        fontName.c_str(),//�t�H���g�t�@�~���[�̖��O���܂ޕ�����
        NULL,//�t�H���g�R���N�V�����I�u�W�F�N�g�ւ̃|�C���^
        DWRITE_FONT_WEIGHT_REGULAR,//����(enum�Ŏw��)
        DWRITE_FONT_STYLE_NORMAL,//�X�^�C��(enum�Ŏw��)�B�����ŕ������΂߂ɂ�����ł���
        DWRITE_FONT_STRETCH_NORMAL,//�X�g���b�`(enum�Ŏw��)�B�����̈����L�΂��
        size,//�s�N�Z�����P�ʂ̃t�H���g�T�C�Y?
        L"en_us",//���P�[�������܂ޕ����� ���P�[���Ƃ́A����̏������[��
        &pWriteTextFormat
    );

    // �t�H���g���Ȃ������肵�Đ������s������AArial�Ő���
    if(result != S_OK)
    {
        result = dWriteFactory->CreateTextFormat
        (
            L"Arial",//�t�H���g�t�@�~���[�̖��O���܂ޕ�����
            NULL,//�t�H���g�R���N�V�����I�u�W�F�N�g�ւ̃|�C���^
            DWRITE_FONT_WEIGHT_REGULAR,//����(enum�Ŏw��)
            DWRITE_FONT_STYLE_NORMAL,//�X�^�C��(enum�Ŏw��)�B�����ŕ������΂߂ɂ�����ł���
            DWRITE_FONT_STRETCH_NORMAL,//�X�g���b�`(enum�Ŏw��)�B�����̈����L�΂��
            size,//�s�N�Z�����P�ʂ̃t�H���g�T�C�Y?
            L"en_us",//���P�[�������܂ޕ����� ���P�[���Ƃ́A����̏������[��
            &pWriteTextFormat
        );
    }

    //�����̑������ݒ�B���̏ꍇ�A��������
    //���E�ʒu
    result = pWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    //�㉺�ʒu
    result = pWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);


    pTextFormat.emplace(name, pWriteTextFormat);
    return true;

}

void TextWrite::Draw
(
    const Vector2& position, 
    const Color& color,
    /*const TextAlignment allgnment,*/
    const std::wstring& text, 
    const std::string& fontName
)
{
    drawTextDatas.push_back(std::make_tuple(position,color,/* allgnment,*/ text, fontName));
}
