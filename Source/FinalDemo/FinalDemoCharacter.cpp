// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalDemoCharacter.h"

#include "SActionComponent.h"

#include "Camera/CameraComponent.h"
#include "SInteractComponent.h"
#include"SAttributeComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"



AFinalDemoCharacter::AFinalDemoCharacter()
{

	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation=true;
	
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp=CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp=CreateDefaultSubobject<USInteractComponent>("InteractComp");

	AttributeComp=CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp=CreateDefaultSubobject<USActionComponent>("ActionComp");

	//move
	GetCharacterMovement()->bOrientRotationToMovement=true;
	bUseControllerRotationYaw=false;

	TimeToHitParamName="TimeToHit";
	
}

void AFinalDemoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this,&AFinalDemoCharacter::PlayerOnHealthChanged);
}



void AFinalDemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFinalDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFinalDemoCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AFinalDemoCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AFinalDemoCharacter::SprintStop);

	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AFinalDemoCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AFinalDemoCharacter::PrimaryAttack);

	
}




void AFinalDemoCharacter::MoveForward(float Value)
{
	FRotator ControlRot=GetControlRotation();
	ControlRot.Pitch=0.0f;
	ControlRot.Roll=0.0f;
	AddMovementInput(ControlRot.Vector(),Value);
}

void AFinalDemoCharacter::MoveRight(float Value)
{
	FRotator ControlRot=GetControlRotation();
	ControlRot.Pitch=0.0f;
	ControlRot.Roll=0.0f;
	
	FVector RightVector=FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector,Value);
}

void AFinalDemoCharacter::SprintStart()
{
	ActionComp->StartActionByName(this,"Sprint");
}

void AFinalDemoCharacter::SprintStop()
{
	ActionComp->StopActionByName(this,"Sprint");
}




void AFinalDemoCharacter::PrimaryInteract()
{
	if(InteractComp){
		InteractComp->ItemInteract();
	}
}

// attack
void AFinalDemoCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this,"PrimaryAttack");
}






void AFinalDemoCharacter::PlayerOnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                                float Delta)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("be Shoot"));
	if(Delta<0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);
	
		if(NewHealth<=0.0f)
		{
			APlayerController* PC=Cast<APlayerController>(GetController());
			DisableInput(PC);

			SetLifeSpan(5.0f);
		}
	}
}

void AFinalDemoCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}


FVector AFinalDemoCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}


UCameraComponent* AFinalDemoCharacter::GetCamera() const
{
	return CameraComp;
}

