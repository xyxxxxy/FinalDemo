// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"


bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	
	USActionComponent* Comp=GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running: %s"),*GetNameSafe(this));

	USActionComponent* Comp=GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning=true;

	Comp->OnActionStarted.Broadcast(Comp,this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("stopped: %s"),*GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Comp=GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning=false;

	Comp->OnActionStopped.Broadcast(Comp,this);
}


UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp=Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}


