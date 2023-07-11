// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractComponent.generated.h"

class ASInteractActor;
class USWorldUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCursorChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALDEMO_API USInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void ItemInteract();
	
	USInteractComponent();

protected:

	bool IsHover;

	UPROPERTY()
	AActor* FocusActor;

	UPROPERTY()
	APawn* MyCharacter;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* Infocus);

	UFUNCTION()
	void FindBestInteractable();

	UFUNCTION()
	void OpenInteractUI();

	UFUNCTION()
	void CloseInteractUI() const;
	
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnCursorChanged OnCursorDynamic;

	UPROPERTY(BlueprintAssignable)
	FOnCursorChanged OnCursorStatic;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
