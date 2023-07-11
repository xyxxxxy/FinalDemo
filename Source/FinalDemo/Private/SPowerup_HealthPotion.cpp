// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	
	CreditAmount=50.0f;

}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	USAttributeComponent* AttributeComp=USAttributeComponent::GetAttributes(InstigatorPawn);

	if(ensure(AttributeComp)&&!AttributeComp->IsFullHealth())
	{
		ASPlayerState* PS=InstigatorPawn->GetPlayerState<ASPlayerState>();
		
		if(PS->RemoveCredits(CreditAmount) && AttributeComp->ApplyHealthChange(this,AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}


