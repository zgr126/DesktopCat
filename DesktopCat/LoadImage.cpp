#include "LoadImage.h"

LLoadImage* LLoadImage::instance = nullptr;

LLoadImage* LLoadImage::create()
{
	if (instance == nullptr)
		instance = new LLoadImage();
	return instance;
}

LLoadImage* LLoadImage::GetInstance()
{
	return instance;
}

void LLoadImage::release()
{
	delete instance;
	instance = nullptr;
}

LLoadImage::LLoadImage()
{

}

HRESULT LLoadImage::LoadintImage(LPCTSTR _fileName, ID2D1RenderTarget* _pRT, IWICImagingFactory* _pimageFactory,
	ID2D1Bitmap** _pBitmap)
{
	//IWICStream* pStream = NULL;
	//IWICBitmapScaler* pScaler = NULL;
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICFormatConverter* pConverter = NULL;

	HRESULT hr = _pimageFactory->CreateDecoderFromFilename(
		_fileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);
	if (!SUCCEEDED(hr))
		OutputDebugString(L"CreateDecoderFromFilenameÊ§°Ü\n");

	pDecoder->GetFrame(0, &pSource);


	hr = _pimageFactory->CreateFormatConverter(&pConverter);
	if (!SUCCEEDED(hr))
		OutputDebugString(L"CreateFormatConverterÊ§°Ü\n");

	hr = pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut);
	if (!SUCCEEDED(hr))
		OutputDebugString(L"InitalizeÊ§°Ü\n");

	hr = _pRT->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		_pBitmap);

	if (!SUCCEEDED(hr))
		OutputDebugString(L"CreateBitmapFromWicBitmapÊ§°Ü\n");

	pDecoder->Release();
	pSource->Release();
	//pStream->Release();
	pConverter->Release();
	//pScaler->Release();
	return hr;
}