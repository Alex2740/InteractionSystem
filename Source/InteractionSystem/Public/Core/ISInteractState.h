// Copyright (c) 2023, Me&Myself. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ISInteractState.generated.h"

UENUM(BlueprintType)
enum class EISInteractState : uint8
{
	NotReady,
	Ready,
	Interacted
};
