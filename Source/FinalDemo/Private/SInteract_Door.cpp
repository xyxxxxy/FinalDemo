


#include "SInteract_Door.h"



ASInteract_Door::ASInteract_Door()
{
	DoorMeshComp=CreateDefaultSubobject<UStaticMeshComponent>("DoorMeshComp");

	DoorTimeLineComp=CreateDefaultSubobject<UTimelineComponent>("DoorTimeLineComp");

	DoorMeshComp->SetupAttachment(BaseMeshComp);

	DoorMeshComp->SetRenderInMainPass(true);
	DoorMeshComp->SetRenderCustomDepth(false);
	DoorMeshComp->SetCustomDepthStencilValue(5.0f);

	DoorMeshComp->SetCollisionProfileName(CollisionName);
	
}


void ASInteract_Door::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateFunctionFloat.BindDynamic(this,&ASInteract_Door::UpdateTimelineComp);

	if(ensureMsgf(DoorCurve,TEXT("Where is your DoorCurve?")))
	{
		DoorTimeLineComp->AddInterpFloat(DoorCurve,UpdateFunctionFloat);
	}
}

void ASInteract_Door::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!bOnIsOpen)
	{
		bOnIsOpen=true;
		DoorTimeLineComp->Play();
	}
	else
	{
		bOnIsOpen=false;
		DoorTimeLineComp->Reverse();
	}
	
}


void ASInteract_Door::OpenOutline_Implementation(APawn* InstigatorPawn)
{
	DoorMeshComp->SetRenderCustomDepth(true);
}


void ASInteract_Door::CloseOutline_Implementation(APawn* InstigatorPawn)
{
	DoorMeshComp->SetRenderCustomDepth(false);
}


void ASInteract_Door::OnActorLoad_Implementation()
{
	Super::OnActorLoad_Implementation();

	OnRep_DoorOpened();
}


void ASInteract_Door::OnRep_DoorOpened()
{
	
	//float CurrentYaw=bOnIsOpen?120.0f:0.0f;
	bOnIsOpen?DoorTimeLineComp->Play():DoorTimeLineComp->Reverse();
	//DoorMeshComp->SetRelativeRotation(FRotator(0.0f,CurrentYaw,0.0f));
}



void ASInteract_Door::UpdateTimelineComp(float Output)
{
	DoorMeshComp->SetRelativeRotation(FRotator(0.0f,Output,0.0f));
}
