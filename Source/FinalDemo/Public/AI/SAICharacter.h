// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USActionComponent;
class UPawnSensingComponent;
class USAttributeComponent;

class USWorldUserWidget;
class UUserWidget;

UCLASS()
class FINALDEMO_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASAICharacter();

protected:

	UPROPERTY()
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParamName;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void AIOnHealthChanged(AActor* InstigatorActor,USAttributeComponent* OwningComp,float NewHealth,float Delta);
	

};
