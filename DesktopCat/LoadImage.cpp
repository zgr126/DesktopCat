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

HRESULT LLoadImage::LoadintImage(LPCTSTR fileName, ID2D1RenderTarget* pRT, IWICImagingFactory* pimageFactory,
	ID2D1Bitmap** pBitmap)
{
	//IWICStream* pStream = NULL;
	//IWICBitmapScaler* pScaler = NULL;
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICFormatConverter* pConverter = NULL;

	HRESULT hr = pimageFactory->CreateDecoderFromFilename(
		fileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);
	if (!SUCCEEDED(hr))
		OutputDebugString(L"CreateDecoderFromFilenameÊ§°Ü\n");

	pDecoder->GetFrame(0, &pSource);


	hr = pimageFactory->CreateFormatConverter(&pConverter);
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

	hr = pRT->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		pBitmap);

	if (!SUCCEEDED(hr))
		OutputDebugString(L"CreateBitmapFromWicBitmapÊ§°Ü\n");

	pDecoder->Release();
	pSource->Release();
	//pStream->Release();
	pConverter->Release();
	//pScaler->Release();
	return hr;
}