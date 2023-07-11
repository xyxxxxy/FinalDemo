// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "FinalDemo/FinalDemoCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UGamePlayBPLibrary.generated.h"


UCLASS()
class FINALDEMO_API UUGamePlayBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Gameplay")
	static void FindItemInDataTable(FItemStruct Item,bool& isFound,FItemPropertyStruct& ItemProperty);

	UFUNCTION(BlueprintCallable,Category="Gameplay")
	static bool ApplyDamage(AActor* DamageCauser,AActor* TargetActor,float DamageAmount);

	UFUNCTION(BlueprintCallable,Category="GamePlay")
	static bool ApplyDirectionDamage(AActor* DamageCauser,AActor* TargetActor,float DamageAmount,const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable,Category="Character")
	static bool GetPlayer(APawn* Pawn,AFinalDemoCharacter* & Player);

	UFUNCTION(BlueprintCallable,Category="Character")
	static void PlayerInput(APawn* &MyPawn,bool bIsEnabled);
};
