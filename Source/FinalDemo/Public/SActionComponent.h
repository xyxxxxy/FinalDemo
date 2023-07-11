// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"
class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged,class USActionComponent*,OwningComp,USAction*,Action);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALDEMO_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable,Category="Actions")
	void AddAction(AActor* Instigator,TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category="Actions")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable,Category="Actions")
	bool StartActionByName(AActor* Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable,Category="Actions")
	bool StopActionByName(AActor* Instigator,FName ActionName);
	
	USActionComponent();

protected:

	UFUNCTION(Server,Reliable)
	void ServerStartAction(AActor* Instigator,FName ActionName);
	
	UPROPERTY(EditAnywhere,Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly)
	TArray<USAction*> Actions;
	
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
