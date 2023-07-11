// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "SInteractInterface.h"
#include "GameFramework/Actor.h"
#include "SInteractActor.generated.h"




UCLASS()
class FINALDEMO_API ASInteractActor : public AActor,public ISInteractInterface
{
	GENERATED_BODY()

public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	virtual void OpenOutline_Implementation(APawn* InstigatorPawn) override;
	
	virtual void CloseOutline_Implementation(APawn* InstigatorPawn) override;
	
	
	ASInteractActor();

protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Components")
	UBillboardComponent* BillboardComp;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* BaseMeshComp;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Components")
	USceneCaptureComponent2D* SceneCaptureComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interact")
	FItemStruct Item;

	UPROPERTY()
	FName CollisionName;
	
	
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	
	
	UFUNCTION()
	void SetMesh(FItemStruct ItemStru) const;


	


	

};




