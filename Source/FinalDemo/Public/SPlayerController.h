// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged,APawn*,NewPawn);

UCLASS()
class FINALDEMO_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	virtual void SetupInputComponent() override;
	
};
