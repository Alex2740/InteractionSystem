// Copyright (c) 2023, Me&Myself. All Rights Reserved.

#include "Core/ISInteractiveActor.h"

#include "Components/SphereComponent.h"
#include "Core/ISInteractiveComponent.h"
#include "Core/ISInteractState.h"
#include "Net/UnrealNetwork.h"

AISInteractiveActor::AISInteractiveActor()
{
	PrimaryActorTick.bCanEverTick = false;

	State = EISInteractState::Ready;
	
	TriggerComp = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	TriggerComp->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerComp;
	bReplicates = true;
}


void AISInteractiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AISInteractiveActor::HandleTriggerBeginOverlap);
	TriggerComp->OnComponentEndOverlap.AddDynamic(this, &AISInteractiveActor::HandleTriggerEndOverlap);
}

void AISInteractiveActor::Interact(AActor* InstigatorIn)
{
	State = EISInteractState::Interacted;
	OnRep_State();
	DoInteract(InstigatorIn);
	BP_DoInteract(InstigatorIn);
}

bool AISInteractiveActor::CanBeInteracted(AActor* InstigatorIn)
{
	return State == EISInteractState::Ready;
}

void AISInteractiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AISInteractiveActor, State);
}

void AISInteractiveActor::OnRep_State()
{
	//VFX

	if(State == EISInteractState::Interacted)
	{
		for(UISInteractiveComponent*& Interactor : PossibleInteractors)
		{
			Interactor->RemoveInteractive(this);
		}
	}
}

void AISInteractiveActor::HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor)
	{
		return;
	}
	
	if (UISInteractiveComponent* InteractionComp = Cast<UISInteractiveComponent>(OtherActor->GetComponentByClass(UISInteractiveComponent::StaticClass())))
	{
		PossibleInteractors.Add(InteractionComp);
		InteractionComp->AddInteractive(this);
	}
}

void AISInteractiveActor::HandleTriggerEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor)
	{
		return;
	}
	
	if (UISInteractiveComponent* InteractionComp = Cast<UISInteractiveComponent>(OtherActor->GetComponentByClass(UISInteractiveComponent::StaticClass())))
	{
		PossibleInteractors.Remove(InteractionComp);
		InteractionComp->RemoveInteractive(this);
	}
}