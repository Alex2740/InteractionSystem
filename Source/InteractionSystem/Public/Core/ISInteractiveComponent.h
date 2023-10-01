// Copyright (c) 2023, Me&Myself. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ISInteractiveComponent.generated.h"

DECLARE_DELEGATE_RetVal_TwoParams(bool, FInteractiveLineSweepDelegateSignature, float /* Distance */,
								  TArray<FHitResult>& /* Out Result */)

class IISInteractive;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UISInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UISInteractiveComponent();

	void Interact();

	void AddInteractive(IISInteractive* Interactive);
	void RemoveInteractive(IISInteractive* Interactive);

	void SetInteractionLineSweepDelegate(FInteractiveLineSweepDelegateSignature Delegate){LineSweepDelegate = Delegate;}
	
protected:
	void RecomputeInteractiveRelevancy();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(AActor* Interactive, AActor* InstigatorIn);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	float InteractLineSweepDistance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	bool bUseMostRelevantActorByDistance;

	TArray<IISInteractive*> PossibleInteractive;
	FInteractiveLineSweepDelegateSignature LineSweepDelegate;
	
	IISInteractive* MostRelevantActor;
};
