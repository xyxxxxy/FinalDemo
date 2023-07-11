// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;

UCLASS()
class FINALDEMO_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ASMagicProjectile();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	TSubclassOf<USActionEffect> BurningActionClass;
	
protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent*OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void BeginPlay() override;

};
