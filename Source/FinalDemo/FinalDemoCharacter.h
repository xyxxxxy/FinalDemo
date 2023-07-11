// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FinalDemoCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class UParticleSystem;

class USAttributeComponent;
class USInteractComponent;
class USActionComponent;


UCLASS(config=Game)
class AFinalDemoCharacter : public ACharacter
{
	GENERATED_BODY()
protected:

	// normal component
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UCameraComponent* CameraComp;
	
	// special component
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USInteractComponent* InteractComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;

	//other
	UPROPERTY(VisibleAnywhere,Category="Flash")
	FName TimeToHitParamName;

protected:

	//move
	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();

	//act
	void PrimaryInteract();
	
	void PrimaryAttack();
	

	
	UFUNCTION()
	void PlayerOnHealthChanged(AActor* InstigatorActor,USAttributeComponent* OwningComp,float NewHealth,float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:

	AFinalDemoCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Exec)
	void HealSelf(float Amount=100);

	UFUNCTION()
	UCameraComponent* GetCamera() const;
	
};


