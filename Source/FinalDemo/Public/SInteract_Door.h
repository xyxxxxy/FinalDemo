// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractActor.h"
#include "Components/TimelineComponent.h"
#include "SInteract_Door.generated.h"


UCLASS()
class FINALDEMO_API ASInteract_Door : public ASInteractActor
{
	GENERATED_BODY()




public:

	ASInteract_Door();



	virtual  void Interact_Implementation(APawn* InstigatorPawn) override;
	
	virtual void OpenOutline_Implementation(APawn* InstigatorPawn) override;
	
	virtual void CloseOutline_Implementation(APawn* InstigatorPawn) override;

	virtual void OnActorLoad_Implementation() override;
	
protected:

	UPROPERTY(BlueprintReadOnly,SaveGame,Category="States")
	bool bOnIsOpen;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* DoorMeshComp;
	
	UPROPERTY(EditDefaultsOnly,Category="Curve")
	UCurveFloat* DoorCurve;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Timeline")
	UTimelineComponent* DoorTimeLineComp;
	
	FOnTimelineFloat UpdateFunctionFloat;
	
	UFUNCTION()
	void UpdateTimelineComp(float Output);
	
	UFUNCTION()
	void OnRep_DoorOpened();

	virtual void BeginPlay() override;
	
};
