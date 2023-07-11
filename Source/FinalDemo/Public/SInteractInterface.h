// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FINALDEMO_API ISInteractInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Interaction")
	void Interact(APawn* InstigatorPawn);
	virtual void Interact_Implementation(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Interaction")
	void OpenOutline(APawn* InstigatorPawn);
	virtual void OpenOutline_Implementation(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Interaction")
	void CloseOutline(APawn* InstigatorPawn);
	virtual void CloseOutline_Implementation(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Load")
	void OnActorLoad();
	virtual void OnActorLoad_Implementation();
};
