// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"

#include "DrawDebugHelpers.h"
#include "SInteractActor.h"
#include "SInteractInterface.h"
#include"Camera/CameraComponent.h"
#include "FinalDemo/FinalDemoCharacter.h"
#include"SWorldUserWidget.h"
#include "HAL/ExceptionHandling.h"


USInteractComponent::USInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FocusActor=nullptr;
	
	IsHover=false;
	
	MyCharacter=nullptr;
	
	TraceDistance=500.0f;

	CollisionChannel=ECC_GameTraceChannel1;
}


void USInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn=Cast<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}




void USInteractComponent::FindBestInteractable()
{

	AActor* MyOwner=GetOwner();
	APawn* MyPawn=Cast<APawn>(MyOwner);
	FVector EyeLocation;
	FRotator EyeRotation;
	AFinalDemoCharacter* Player=Cast<AFinalDemoCharacter>(MyOwner);
	
	if(!Player)
	{
		return;
	}
	
	FVector CameraForward=Player->GetCamera()->GetForwardVector();
	
	MyPawn->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	
	FVector End=EyeLocation+(CameraForward*TraceDistance);
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,End,CollisionChannel);
	
	//FColor LineColor=Hit.IsValidBlockingHit()?FColor::Green:FColor::Red;
	//DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,2.0f,0,2.0f);
	if(!Hit.bBlockingHit)
	{
		if(FocusActor)
		{
			// Close outline
			if(FocusActor->Implements<USInteractInterface>())
			{
				ISInteractInterface::Execute_CloseOutline(FocusActor,MyPawn);
				//change cursor
				OnCursorStatic.Broadcast();
				//Close UI
				CloseInteractUI();
			}

		}
		FocusActor=nullptr;
		IsHover=false;
		return;
	}
	
	AActor* HitActor=Hit.GetActor();
	
	if(HitActor)
	{
		if(HitActor==FocusActor)
		{
			return;
		}
		if(HitActor->Implements<USInteractInterface>())
		{
			if(MyPawn)
			{
				FocusActor=HitActor;
				IsHover=true;
				MyCharacter=MyPawn;
				
				//Open outline
				ISInteractInterface::Execute_OpenOutline(HitActor,MyPawn);
				//change cursor
				OnCursorDynamic.Broadcast();
				// Open UI
				OpenInteractUI();
			}
		}
	}

}


void USInteractComponent::OpenInteractUI()
{
	if(DefaultWidgetInstance==nullptr && ensure(DefaultWidgetClass))
	{
		DefaultWidgetInstance=CreateWidget<USWorldUserWidget>(GetWorld(),DefaultWidgetClass);
	}

	if(DefaultWidgetInstance)
	{
		DefaultWidgetInstance->AttachedActor=FocusActor;

		if(!DefaultWidgetInstance->IsInViewport())
		{
			DefaultWidgetInstance->AddToViewport();
		}

	}
}


void USInteractComponent::CloseInteractUI() const
{
	if(DefaultWidgetInstance)
	{
		DefaultWidgetInstance->RemoveFromParent();
	}
}


void USInteractComponent::ItemInteract() 
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Interact!"));
	ServerInteract(FocusActor);
}


void USInteractComponent::ServerInteract_Implementation(AActor* Infocus)
{
	if(!Infocus)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("No InteractActor!"));
		return;
	}

		ISInteractInterface::Execute_Interact(Infocus,MyCharacter);


}



