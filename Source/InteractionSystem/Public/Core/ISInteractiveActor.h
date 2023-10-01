// Copyright (c) 2023, Me&Myself. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISInteractive.h"
#include "GameFramework/Actor.h"
#include "ISInteractiveActor.generated.h"

class UISInteractiveComponent;

enum class EISInteractState : uint8;

UCLASS(Abstract)
class INTERACTIONSYSTEM_API AISInteractiveActor : public AActor, public IISInteractive
{
	GENERATED_BODY()

public:
	AISInteractiveActor();
	
	virtual void PostInitializeComponents() override;
	
	virtual void Interact(AActor* InstigatorIn) override;
	virtual bool CanBeInteracted(AActor* InstigatorIn) override;
	virtual FVector GetInteractiveLocation() const override
	{
		return GetTransform().GetLocation();
	}
	
	virtual AActor* GetInteractiveOwner() const override
	{
		return Cast<AActor>(_getUObject());
	}
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void DoInteract(AActor* InstigatorIn){};

	UFUNCTION(BlueprintImplementableEvent)
	void BP_DoInteract(AActor* InstigatorIn);
	
	UFUNCTION()
	void OnRep_State();

	UFUNCTION()
	void HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* TriggerComp;
	
	UPROPERTY(ReplicatedUsing=OnRep_State, BlueprintReadOnly)
	EISInteractState State;
	
	UPROPERTY()
	TArray<UISInteractiveComponent*> PossibleInteractors;
};
