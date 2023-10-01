// Copyright (c) 2023, Me&Myself. All Rights Reserved.

#include "Core/ISInteractiveComponent.h"

#include "Core/ISInteractive.h"

UISInteractiveComponent::UISInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	InteractLineSweepDistance = 100.f;
	bUseMostRelevantActorByDistance = false;
}

void UISInteractiveComponent::Interact()
{
	RecomputeInteractiveRelevancy(); 
	if(!MostRelevantActor)
	{
		return;
	}

	if(!MostRelevantActor->GetInteractiveOwner()->HasAuthority())
	{
		Server_Interact(MostRelevantActor->GetInteractiveOwner(), GetOwner());
		return;
	}
	
	MostRelevantActor->Interact(GetOwner());
}

void UISInteractiveComponent::AddInteractive(IISInteractive* Interactive)
{
	if(!Interactive)
	{
		return;
	}
	
	if(!GetOwner())
	{
		return;
	}
	
	if(!Interactive->CanBeInteracted(GetOwner()))
	{
		return;
	}
	
	if(PossibleInteractive.Contains(Interactive))
	{
		return;
	}
	
	PossibleInteractive.Add(Interactive);
}

void UISInteractiveComponent::RemoveInteractive(IISInteractive* Interactive)
{
	if(!Interactive)
	{
		return;
	}
	
	PossibleInteractive.Remove(Interactive);
	
	if(Interactive == MostRelevantActor)
	{
		MostRelevantActor = nullptr;
	}
}

void UISInteractiveComponent::RecomputeInteractiveRelevancy()
{
	MostRelevantActor = nullptr;
	TArray<FHitResult> Results;
	if(!ensureMsgf(LineSweepDelegate.IsBound(), TEXT("Delegate was not bound")))
	{
		return;
	}

	if(LineSweepDelegate.Execute(InteractLineSweepDistance, Results))
	{
		for(FHitResult& Result : Results)
		{
			if(!Result.GetActor())
			{
				continue;
			}
			
			if(!Result.GetActor()->Implements<UISInteractive>())
			{
				break;
			}
			
			IISInteractive* Interactive = Cast<IISInteractive>(Result.GetActor());
			for(IISInteractive*& Relevant : PossibleInteractive)
			{
				if(Relevant->GetInteractiveOwner() == Result.GetActor())
				{
					MostRelevantActor = Interactive;
					return;
				}
			}

		}
	}
	
	if(!bUseMostRelevantActorByDistance)
	{
		return;
	}
	
	PossibleInteractive.Sort([&](const IISInteractive& A, const IISInteractive& B)
	{
		return FVector::DistSquared(A.GetInteractiveLocation(), GetOwner()->GetTransform().GetLocation()) <
			FVector::DistSquared(B.GetInteractiveLocation(), GetOwner()->GetTransform().GetLocation());
	});

	if(PossibleInteractive.IsEmpty())
	{
		return;
	}
	
	MostRelevantActor = PossibleInteractive[0];
}

void UISInteractiveComponent::Server_Interact_Implementation(AActor* Interactive, AActor* InstigatorIn)
{
	if (IISInteractive* Int = Cast<IISInteractive>(Interactive))
	{
		Int->Interact(InstigatorIn);
	}
}

bool UISInteractiveComponent::Server_Interact_Validate(AActor* Interactive, AActor* InstigatorIn)
{
	return true;
}

