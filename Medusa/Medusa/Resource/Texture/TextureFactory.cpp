// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "TextureFactory.h"
#include "Resource/Texture/GPUTexture.h"
#include "Resource/Texture/ImageTexture.h"
#include "Resource/Image/ImageFactory.h"

MEDUSA_BEGIN;

TextureFactory::TextureFactory()
{

}

TextureFactory::~TextureFactory()
{
	
}

bool TextureFactory::Initialize()
{
	return true;
}

bool TextureFactory::Uninitialize()
{
	Clear();
	return true;
}


ImageTexture* TextureFactory::CreateFromFile( const FileIdRef& fileId ,StringRef samplerName/*=ShaderSamplerNames::Texture*/,
													 GraphicsTextureUnits unit/*=GraphicsTextureUnits::Texture0*/,ResourceShareType shareType /*= ResourceShareType::Share*/)
{
	
	ImageTexture* result=(ImageTexture*)Find(fileId);
	RETURN_SELF_IF_NOT_NULL(result);

	IImage* image=ImageFactory::Instance().CreateFromFile(fileId);
	RETURN_NULL_IF_NULL(image);
	return CreateFromImage(fileId,image,samplerName,unit,shareType);
}

ImageTexture* TextureFactory::CreateFromOrderItem(const FileIdRef& fileId, const FileMapOrderItem& orderItem, StringRef samplerName /*= ShaderSamplerNames::Texture*/, GraphicsTextureUnits unit /*= GraphicsTextureUnits::Texture0*/, ResourceShareType shareType /*= ResourceShareType::Share*/)
{
	ImageTexture* result = (ImageTexture*)Find(fileId);
	RETURN_SELF_IF_NOT_NULL(result);

	IImage* image = ImageFactory::Instance().CreateFromOrderItem(fileId,orderItem);
	RETURN_NULL_IF_NULL(image);
	return CreateFromImage(fileId, image, samplerName, unit, shareType);
}

ImageTexture* TextureFactory::CreateFromImage(const FileIdRef& fileId, IImage* image, StringRef samplerName/*=ShaderSamplerNames::Texture*/,
													  GraphicsTextureUnits unit/*=GraphicsTextureUnits::Texture0*/,ResourceShareType shareType /*= ResourceShareType::Share*/)
{
	ImageTexture* result=(ImageTexture*)Find(fileId);
	RETURN_SELF_IF_NOT_NULL(result);

	result=new ImageTexture(fileId,image,samplerName,unit);
	result->ResetDefaultParameters();
	Add(result, shareType);

	return result;
}


GPUTexture* TextureFactory::CreateGPUTexture( const FileIdRef& fileId,const Size2U& size,GraphicsTextureType textureType/*=GraphicsTextureType::Texture2D*/,
											  GraphicsTextureTarget textureTarget/*=GraphicsTextureTarget::Texture2D*/,
											  GraphicsInternalFormat internalFormat/*=GraphicsInternalFormat::RGB*/, 
											  int level/*=0*/,int border/*=0*/,
											  GraphicsPixelFormat pixelFormat/*=GraphicsPixelFormat::RGB*/,
											  GraphicsPixelDataType pixelType/*=GraphicsPixelDataType::Byte*/,
											  StringRef samplerName/*=StringRef::Empty*/,
											  GraphicsTextureUnits unit/*=GraphicsTextureUnits::Texture0*/,ResourceShareType shareType /*= ResourceShareType::Share*/)
{
	GPUTexture* result=(GPUTexture*)Find(fileId);
	RETURN_SELF_IF_NOT_NULL(result);

	result=new GPUTexture(fileId,size,textureType,textureTarget,internalFormat,level,border,pixelFormat,pixelType,samplerName,unit);
	result->ResetDefaultParameters();
	Add(result, shareType);

	return result;
}



MEDUSA_END;