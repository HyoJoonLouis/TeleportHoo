#include "BaseCharacter.h"
#include "CharacterTrajectoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("weapon-r1Socket"));
	WeaponMesh->SetCollisionProfileName(FName("NoCollision"), false);

	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bActivateCollision)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
		ObjectTypes.Add(Pawn);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		TArray<FHitResult> HitResults;

		bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(this, WeaponMesh->GetSocketLocation(FName("Start")), WeaponMesh->GetSocketLocation(FName("End")), 30.0f, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);
		if (Result)
		{
			for (const auto& HitResult : HitResults)
			{
				AActor* HitActor = HitResult.GetActor();
				if (AlreadyHitActors.Contains(HitActor))
					continue;
				AlreadyHitActors.AddUnique(HitActor);
				if (IsValid(HitActor))
				{

				}
			}
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CurrentHealth);
}

void ABaseCharacter::SetState(ECharacterStates NewState)
{
	if (CurrentState == NewState)
		return;
	CurrentState = NewState;
}

void ABaseCharacter::StartWeaponCollision()
{
	bActivateCollision = true;
	AlreadyHitActors.Empty();
}

void ABaseCharacter::EndWeaponCollision()
{
	bActivateCollision = false;
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (!CheckCurrentState({ECharacterStates::IDLE}))
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (bTargeting && IsValid(TargetActor))
		return;

	if (CheckCurrentState({ ECharacterStates::IDLE }))
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

