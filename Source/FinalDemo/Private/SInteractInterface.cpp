// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractInterface.h"

// Add default functionality here for any ISInteractInterface functions that are not pure virtual.
void ISInteractInterface::Interact_Implementation(APawn* InstigatorPawn)
{
	//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,TEXT("interact"));
}

void ISInteractInterface::OpenOutline_Implementation(APawn* InstigatorPawn)
{
	//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,TEXT("OpenOutline"));
}

void ISInteractInterface::CloseOutline_Implementation(APawn* InstigatorPawn)
{
	//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,TEXT("CloseOutline"));
}

void ISInteractInterface::OnActorLoad_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,TEXT(":OnActorLoad"));
}
