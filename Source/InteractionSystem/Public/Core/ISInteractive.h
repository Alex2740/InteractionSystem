// Copyright (c) 2023, Me&Myself. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISInteractive.generated.h"

UINTERFACE()
class UISInteractive : public UInterface
{
	GENERATED_BODY()
};

class INTERACTIONSYSTEM_API IISInteractive
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* InstigatorIn) = 0;
	virtual bool CanBeInteracted(AActor* InstigatorIn) = 0;
	virtual FVector GetInteractiveLocation() const = 0;
	virtual AActor* GetInteractiveOwner() const = 0;
};
