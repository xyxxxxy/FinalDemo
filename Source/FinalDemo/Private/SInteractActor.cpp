// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractActor.h"


#include "UGamePlayBPLibrary.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneCaptureComponent2D.h"




ASInteractActor::ASInteractActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;


	
	BaseMeshComp=CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");

	RootComponent=BaseMeshComp;

	BillboardComp=CreateDefaultSubobject<UBillboardComponent>("BillboardComp");
	BillboardComp->SetupAttachment(RootComponent);
	
	SceneCaptureComp=CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComp");
	SceneCaptureComp->SetupAttachment(BillboardComp);
	
	BaseMeshComp->SetRenderInMainPass(true);
	BaseMeshComp->SetRenderCustomDepth(false);
	BaseMeshComp->SetCustomDepthStencilValue(5.0f);
	
	BillboardComp->SetHiddenInGame(true,false);

	CollisionName="TraceInteract";
	
	
	BaseMeshComp->SetCollisionProfileName(CollisionName);
	

	//SetReplicates(true);

	
	
}


void ASInteractActor::BeginPlay()
{
	Super::BeginPlay();

	//BaseMeshComp->SetSimulatePhysics(false);
}

void ASInteractActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetMesh(Item);
	
	
}


void ASInteractActor::Interact_Implementation(APawn* InstigatorPawn)
{
	ISInteractInterface::Interact_Implementation(InstigatorPawn);


}

void ASInteractActor::OpenOutline_Implementation(APawn* InstigatorPawn)
{
	ISInteractInterface::OpenOutline_Implementation(InstigatorPawn);

	BaseMeshComp->SetRenderCustomDepth(true);

	
	
}


void ASInteractActor::CloseOutline_Implementation(APawn* InstigatorPawn)
{
	ISInteractInterface::CloseOutline_Implementation(InstigatorPawn);
	
	BaseMeshComp->SetRenderCustomDepth(false);
	
	
}



void ASInteractActor::SetMesh(FItemStruct ItemStru) const
{
	bool bIsFind;
	FItemPropertyStruct ItemProperty;
	UUGamePlayBPLibrary::FindItemInDataTable(ItemStru,bIsFind,ItemProperty);
	if(bIsFind)
	{
		UE_LOG(LogTemp,Log,TEXT("Mesh: %s"),*GetNameSafe(this));
		BaseMeshComp->SetStaticMesh(ItemProperty.Mesh);
		return;
	}
	UE_LOG(LogTemp,Log,TEXT("NoMesh: %s"),*GetNameSafe(this));
}



