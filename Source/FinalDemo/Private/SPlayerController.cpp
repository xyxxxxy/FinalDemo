// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Blueprint/UserWidget.h"


void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu",IE_Pressed,this,&ASPlayerController::TogglePauseMenu); 
	
}



void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}


void ASPlayerController::TogglePauseMenu()
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Cyan,TEXT("UI"));
	if(PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance=nullptr;

		bShowMouseCursor=false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	PauseMenuInstance=CreateWidget<UUserWidget>(this,PauseMenuClass);

	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
	
		bShowMouseCursor=true;
		SetInputMode(FInputModeUIOnly());
	}

}

