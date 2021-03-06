// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "ITimelineModel.h"
#include "Core/Algorithm/Algorithm.h"

MEDUSA_BEGIN;

ITimelineModel::ITimelineModel(const FileIdRef& fileId, float duration /*= 0.f*/)
	:IResource(fileId), mDuration(duration), mIsPreCalculated(false), mFPS(0.f), mFrameCount(0)
{

}

ITimelineModel::~ITimelineModel()
{

}

bool ITimelineModel::Initialize()
{
	if (mDuration <= 0.f)
	{
		if (!mFrames.IsEmpty())
		{
			mDuration = mFrames.Last().Time;
		}
	}
	mFrameCount = mFrames.Count();
	return true;
}

void ITimelineModel::Clear()
{
	mDuration = 0.f;
	mFrames.Clear();
	mFrameCount = 0;
	mIsPreCalculated = false;
	mFPS = 0.f;
}

bool ITimelineModel::TryGetFrame(float time, uint& outPrevFrameIndex, uint& outNextFrameIndex, float& outPercent) const
{
	if (mIsPreCalculated)
	{
		RETURN_FALSE_IF(time < 0.f);

		float index = time *mFPS;
		outNextFrameIndex = (int)Math::Ceil(index);
		double integer;
		outPercent = (float)Math::Mod(index, &integer);
		outPrevFrameIndex = (int)integer;
	}
	else
	{
		return TryGetFrameHelper(time, outPrevFrameIndex, outNextFrameIndex, outPercent);
	}
	return true;

}


void ITimelineModel::AddFrame(float time, uint index, Math::TweenType tweenType /*= Math::TweenType::Linear*/)
{
	mFrames.Add(TimelineFrame(time, index, tweenType));
	mFrameCount = mFrames.Count();

}

void ITimelineModel::AddFrame(float time, uint index, Math::TweenType tweenType, const List<float>& args)
{
	mFrames.Add(TimelineFrame(time, index, tweenType, args));
	mFrameCount = mFrames.Count();

}

void ITimelineModel::AddFrameWithInterval(float frameInterval, uint index, Math::TweenType tweenType /*= Math::TweenType::Linear*/)
{
	AddFrame(frameInterval*index, index, tweenType);

}

intp ITimelineModel::GetSteppedFrameIndex(float time) const
{
	if (mIsPreCalculated)
	{
		return GetPreCalculatedIndex(time);
	}
	else
	{
		uint outPrevFrameIndex;
		uint outNextFrameIndex;
		float outPercent;
		if (TryGetFrame(time, outPrevFrameIndex, outNextFrameIndex, outPercent))
		{
			return outPrevFrameIndex;
		}
		return -1;
	}

}

intp ITimelineModel::GetPreCalculatedIndex(float time) const
{
	if (time < 0.f)
	{
		return -1;
	}

	if (time > mDuration)
	{
		return mFrameCount;
	}
	return (intp)Math::Floor(time*mFPS);
}

void ITimelineModel::PreCalculate(float fps)
{
	RETURN_IF_TRUE(mIsPreCalculated);
	RETURN_IF(fps < 0.f);
	RETURN_IF_EMPTY(mFrames);

	mIsPreCalculated = true;
	mFPS = fps;
	int preCalculateframeCount = (int)Math::Ceil(mDuration * fps);
	RETURN_IF(preCalculateframeCount <= 0);
	mFrameCount = (uint)preCalculateframeCount;

	OnPreCalculateBegin();
	float firstTime = mFrames.First().Time;
	AddPreCalcuatedItem(Math::IsZero(firstTime), 0U, 0U, 0.f);	//add zero frame


	const float frameInterval = 1.f / mFPS;
	float time = 0.f;
	uint outPrevFrameIndex;
	uint outNextFrameIndex;
	float outPercent;

	FOR_EACH_SIZE(i, mFrameCount)
	{
		time += frameInterval;
		bool isFound = TryGetFrameHelper(time, outPrevFrameIndex, outNextFrameIndex, outPercent);
		AddPreCalcuatedItem(isFound, outPrevFrameIndex, outNextFrameIndex, outPercent);
	}

	OnPreCalculateEnd();
}

void ITimelineModel::RemovePreCalculated()
{
	RETURN_IF_FALSE(mIsPreCalculated);
	mIsPreCalculated = false;
	mFPS = 0.f;
	mFrameCount = mFrames.Count();
}


bool ITimelineModel::TryGetFrameHelper(float time, uint& outPrevFrameIndex, uint& outNextFrameIndex, float& outPercent) const
{
	outPrevFrameIndex = 0;
	outNextFrameIndex = 0;
	outPercent = 0.f;

	RETURN_FALSE_IF_EMPTY(mFrames);
	RETURN_FALSE_IF(time < 0.f);

	intp index = Algorithm::BinarySearchLastGreaterThan<TimelineFrame, DefaultCompare<TimelineFrame> >(mFrames.Items(), TimelineFrame(time), 0, mFrames.Count() - 1);
	if (index == 0)
	{
		//time < first frame
		outNextFrameIndex = 0;
		outPrevFrameIndex = 0;
		outPercent = 1.f;
		return false;
	}
	else if (index < 0)
	{
		//not found,so it's at the last frame

		outNextFrameIndex = mFrames.Last().Index;
		outPercent = 1.f;
		outPrevFrameIndex = outNextFrameIndex;

	}
	else
	{
		outNextFrameIndex = (uint)index;
		outPrevFrameIndex = outNextFrameIndex > 1 ? outNextFrameIndex - 1 : 0;
		if (mFrames[outPrevFrameIndex].TweenType == Math::TweenType::None)
		{
			outPercent = 0.f;
			outNextFrameIndex = outPrevFrameIndex;
			return true;
		}

		float nextTime = mFrames[outNextFrameIndex].Time;
		float prevTime = mFrames[outPrevFrameIndex].Time;
		outPercent = (time - prevTime) / (nextTime - prevTime);
		outPercent = Math::Tween::TweenTo(outPercent, mFrames[outPrevFrameIndex].TweenType, mFrames[outPrevFrameIndex].TweenArgs.Items());

		if (outPercent<0.f || outPercent>1.f)
		{
			return false;
		}
	}

	return true;
}




MEDUSA_END;