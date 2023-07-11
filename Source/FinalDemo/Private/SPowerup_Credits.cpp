// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Credits.h"
#include"SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_Credits::ASPowerup_Credits()
{
	CreditAmount=80.0f;
}

void ASPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	USAttributeComponent* AttributeComp=USAttributeComponent::GetAttributes(InstigatorPawn);

	if(ensure(AttributeComp))
	{
		ASPlayerState* PS=InstigatorPawn->GetPlayerState<ASPlayerState>();

		if(PS)
		{
			PS->AddCredits(CreditAmount);
			HideAndCooldownPowerup();
		}
	}
}
