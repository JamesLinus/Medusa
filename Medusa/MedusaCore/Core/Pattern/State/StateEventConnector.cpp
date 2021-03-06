// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaCorePreCompiled.h"
#include "StateEventConnector.h"
#include "Core/Pattern/Predicate/IPredicate.h"
#include "Core/Command/EventArg/IEventArg.h"

MEDUSA_BEGIN;

StateEventConnector::StateEventConnector()
{
}

StateEventConnector::~StateEventConnector(void)
{
	
}

bool StateEventConnector::IsFit(IEventArg& e) const
{
	return e.ClassName() == mEventType;
}


MEDUSA_END;

