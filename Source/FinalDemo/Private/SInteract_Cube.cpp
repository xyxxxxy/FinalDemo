// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteract_Cube.h"


ASInteract_Cube::ASInteract_Cube()
{
	TargetYaw=45.0f;

	bIsRotate=false;

	BaseMeshComp->SetSimulatePhysics(true);
}




void ASInteract_Cube::Interact_Implementation(APawn* InstigatorPawn)
{
	
	BaseMeshComp->AddWorldRotation(FRotator(0.0f,TargetYaw,0.0f));
}
