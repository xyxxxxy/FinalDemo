// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "SWorldUserWidget.generated.h"


UCLASS()
class FINALDEMO_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()


	
protected:

	UPROPERTY(meta=(BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere,Category="UI")
	FVector WorldOffset;
	
	UPROPERTY(BlueprintReadWrite,Category="UI",meta=(ExposeOnSpawn=true))
	AActor* AttachedActor;
};
