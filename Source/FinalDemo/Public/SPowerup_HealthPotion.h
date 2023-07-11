// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

UCLASS()
class FINALDEMO_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()
	
public:	

	ASPowerup_HealthPotion();

protected:

	UFUNCTION(BlueprintCallable)
	void Interact_Implementation(APawn* InstigatorPawn) override;



};
