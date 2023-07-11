// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractActor.h"
#include "SInteract_SaveGame.generated.h"

UCLASS()
class FINALDEMO_API ASInteract_SaveGame : public ASInteractActor
{
	GENERATED_BODY()

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;


};
