// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractActor.h"
#include "SInteract_Cube.generated.h"


UCLASS()
class FINALDEMO_API ASInteract_Cube : public ASInteractActor
{
	GENERATED_BODY()

public:

	ASInteract_Cube();
	
	UPROPERTY(EditAnywhere)
	float TargetYaw;
	
protected:

	UPROPERTY(BlueprintReadOnly,Category="State")
	bool bIsRotate;

public:
	

	UFUNCTION(BlueprintCallable)
	virtual  void Interact_Implementation(APawn* InstigatorPawn) override;
};
