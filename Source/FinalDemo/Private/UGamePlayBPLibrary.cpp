// Fill out your copyright notice in the Description page of Project Settings.


#include "UGamePlayBPLibrary.h"
#include "SAttributeComponent.h"
#include "SPlayerController.h"
#include "Engine/DataTable.h"

void UUGamePlayBPLibrary::FindItemInDataTable(FItemStruct Item,bool& isFound,FItemPropertyStruct& ItemProperty)
{
	isFound=false;
	UDataTable* pDataTable=LoadObject<UDataTable>(NULL,
		TEXT("DataTable'/Game/DataDrive/Data_ItemProperty.Data_ItemProperty'"));
	if(pDataTable)
	{
		for(FName RowName:pDataTable->GetRowNames())
		{
			FItemPropertyStruct* pItemProperty=pDataTable->FindRow<FItemPropertyStruct>(RowName,TEXT("name"));
			if(pItemProperty)
			{
				
				if(Item.ID.IsEqual(RowName))
				{
					ItemProperty=*pItemProperty;
					//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,RowName.ToString());
					isFound=true;
					break;
				}
			}
		}
	}
	return;
}

bool UUGamePlayBPLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp=USAttributeComponent::GetAttributes(TargetActor);
	if(AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser,-DamageAmount);
	}
	return false;
}

bool UUGamePlayBPLibrary::ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{

	if(ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{

		UPrimitiveComponent* HitComp=HitResult.GetComponent();
		if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal*30000.f,HitResult.ImpactPoint,HitResult.BoneName);
		}
		return true;
	}
	return false;
}

bool UUGamePlayBPLibrary::GetPlayer(APawn* Pawn, AFinalDemoCharacter*& Player)
{
	Player=Cast<AFinalDemoCharacter>(Pawn);
	if(Player)
	{
		return true;
	}
	return false;
}

void UUGamePlayBPLibrary::PlayerInput(APawn* & MyPawn,bool bIsEnabled)
{
	ASPlayerController* PC=Cast<ASPlayerController>(MyPawn->GetController());

	if(PC)
	{
		bIsEnabled?MyPawn->EnableInput(PC):MyPawn->DisableInput(PC);
	}
}


