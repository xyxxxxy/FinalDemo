// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ID;
	
	FItemStruct(FName Name="0");
	
	FName GetID() const; 
};

USTRUCT(BlueprintType)
struct FItemPropertyStruct:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemData")
	FName ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ItemData")
	UStaticMesh* Mesh;

	FItemPropertyStruct();
	
};
