// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "Core/String/StringRef.h"
#include "Core/Pattern/Singleton.h"
#include "Core/Collection/Dictionary.h"
#include "Resource/Font/FontId.h"
#include "Core/Geometry/Size2.h"
#include "Resource/IResourceFactory.h"
#include "Core/IO/FileId.h"
#include "FontId.h"

MEDUSA_BEGIN;

class FontFactory :public Singleton<FontFactory>, public IResourceFactory < FontId, IFont* >
{
	friend class Singleton < FontFactory > ;
public:
	FontFactory();
	~FontFactory();
public:
	virtual bool Initialize();
	virtual bool Uninitialize();
	virtual void Shrink();
	virtual void Clear();
	virtual void ReleaseCache();


	virtual bool Remove(const FontId& fontId);
	virtual IFont* Find(const FontId& fontId)const;
	virtual bool Add(IFont* val, ResourceShareType shareType = ResourceShareType::Share);
public:
	IFont* Create(const FontId& fontId, ResourceShareType shareType = ResourceShareType::Share);
	IFont* CreateFromSingleTexture(const FontId& fontId, wchar_t firstChar = L'0', ResourceShareType shareType = ResourceShareType::Share);


	IFont* CreateFromMemory(const FontId& fontId, const MemoryByteData& data, ResourceShareType shareType = ResourceShareType::Share);

	IMaterial* AddGlyphImage(FontImageDepth destDepth, const Size2U& size, int pitch, const MemoryByteData& imageData, FontImageDepth srcDepth, Size2U& outImageSize, Rect2U& outRect);
private:
	Dictionary<FontId, IFont*, DefaultHashCoder<FontId>, EqualCompare<FontId> > mItems;
	List<IFont*> mCacheItems;


	Size2U mInitialImageSize;	//default image size
	Size2U mMaxImageSize;
	//3 type images to cache all ttf glyphs,in these case to make more chars into less draw calls

	List<IMaterial*> mRGBAMaterials;
	List<IMaterial*> mRGBMaterials;
	List<IMaterial*> mAlphaMaterials;

};

MEDUSA_END;