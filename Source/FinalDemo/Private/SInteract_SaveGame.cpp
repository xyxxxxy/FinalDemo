// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteract_SaveGame.h"

#include "SGameModeBase.h"

void ASInteract_SaveGame::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	ASGameModeBase* GM=GetWorld()->GetAuthGameMode<ASGameModeBase>();

	if(GM)
	{
		GM->WriteSaveGame();
	}
}
