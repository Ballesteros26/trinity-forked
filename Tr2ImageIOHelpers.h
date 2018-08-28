#pragma once
#ifndef Tr2ImageIOHelpers_H
#define Tr2ImageIOHelpers_H

class Tr2ImageHandler;

namespace Tr2ImageIOHelpers
{

bool CreateTexture(	
	ImageIO::HostBitmap& ih, 
	Tr2TextureAL &out, 
	uint32_t &memoryUse, 
	Tr2PrimaryRenderContext &renderContext, 
	Tr2RenderContextEnum::BufferUsage usage = Tr2RenderContextEnum::USAGE_CPU_READ );

bool Create2DTexture(	
	ImageIO::HostBitmap& ih, 
	Tr2TextureAL &out, 
	uint32_t &memoryUse, 
	Tr2PrimaryRenderContext &renderContext, 
	Tr2RenderContextEnum::BufferUsage usage = Tr2RenderContextEnum::USAGE_CPU_READ );

bool CopyToTexture( 
	ImageIO::HostBitmap& ih, 
	Tr2TextureAL& texture, 
	unsigned int x, 
	unsigned int y, 
	unsigned int margin, 
	Tr2RenderContext& renderContext );

void AddMargin(	const Tr2RenderContextEnum::PixelFormat format,
				const unsigned char* source,
				const unsigned width, const unsigned height,
				const unsigned margin,
				std::vector<unsigned char> &output, 
				unsigned &outputPitch );

bool IsCairoScriptPath( const wchar_t* path );

struct RasterizationOptions
{
	RasterizationOptions()
		:premultipliedAlpha( false )
	{
	}

	bool premultipliedAlpha;
};

bool RasterizeCairoScript( ImageIO::HostBitmap& bitmap, const char* script, size_t length, uint32_t width, uint32_t height, const RasterizationOptions& options = RasterizationOptions() );

struct CairoScript
{
	const char* script;
	size_t length;

	float scale;
	float rotation;
	Vector2 translation;
	Color color;
};

bool ExportCairoScriptsAsSvg( const std::wstring& filePath, const std::vector<CairoScript>& scripts, uint32_t width, uint32_t height );

bool RasterizeCairoScripts( ImageIO::HostBitmap& bitmap, const std::vector<CairoScript>& scripts, uint32_t width, uint32_t height, const RasterizationOptions& options = RasterizationOptions() );

ImageIO::Result RasterizeCairoScript( ImageIO::HostBitmap& bitmap, IBlueStream* stream, const BlueAsyncRes::QueryArguments& arguments, const RasterizationOptions& options = RasterizationOptions() );
}

#endif