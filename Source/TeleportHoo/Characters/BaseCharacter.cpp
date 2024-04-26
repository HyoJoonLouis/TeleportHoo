#include "BaseCharacter.h"
#include "../HealthBarWidget.h"
#include "CharacterTrajectoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0, 50, 0);
	CameraBoom->TargetOffset = FVector(0, 0, 65);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 9.0f;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetIsReplicated(true);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->FieldOfView = 78;
	FollowCamera->SetIsReplicated(true);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("Sword"));
	WeaponMesh->SetCollisionProfileName(FName("NoCollision"), false);

	bTargeting = false;
	bActivateCollision = false;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetMesh());
	HealthBarComponent->SetRelativeLocation(FVector(0, 0, 200));
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);


	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));
	TrajectoryComponent->SetIsReplicated(true);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidget = Cast<UHealthBarWidget>(HealthBarComponent->GetWidget());

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	Server_SetHealth(MaxHealth);

	// Timeline
	if (TargetingCurve)
	{
		FOnTimelineFloat TargetingCurveCallback;
		TargetingCurveCallback.BindUFunction(this, FName("TargetingTimelineFunction"));

		TargetingTimeline.AddInterpFloat(TargetingCurve, TargetingCurveCallback);
		TargetingTimeline.SetTimelineLength(0.4f);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Timeline
	TargetingTimeline.TickTimeline(DeltaTime);

	if (IsLocallyControlled() && bTargeting)
	{
		if (IsValid(TargetActor) && FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation()) <= 1000)
		{
			GetController()->SetControlRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()), DeltaTime, 5));
		}
		else
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
			ObjectTypes.Add(Pawn);
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(GetOwner());
			FHitResult HitResult;

			bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetActorLocation(), GetActorLocation(), 1000, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, HitResult, true);
			if (Result)
			{
				TargetActor = HitResult.GetActor();
			}
		}
	}
	else
	{
		TargetActor = nullptr;
	}

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
					Cast<ABaseCharacter>(HitActor)->Server_TakeDamage(this, CurrentDamageInfo);
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
		EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Started, this, &ABaseCharacter::Targeting);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error In PlayerSetupPlayerInputComponent"));
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CurrentHealth);
	DOREPLIFETIME(ABaseCharacter, CurrentState);
	DOREPLIFETIME(ABaseCharacter, bTargeting);
}

void ABaseCharacter::OnRep_SetHealth()
{
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealth(CurrentHealth / MaxHealth);
	}
}

void ABaseCharacter::OnRep_SetState()
{
}

void ABaseCharacter::OnRep_SetTargeting()
{
	if (bTargeting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		TargetingTimeline.PlayFromStart();
		ChangeToControllerDesiredRotation();
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		TargetingTimeline.Reverse();
		ChangeToRotationToMovement();
	}
}

void ABaseCharacter::Server_SetHealth_Implementation(float Value)
{
	if (HasAuthority())
	{
		CurrentHealth = Value;
		OnRep_SetHealth();
	}
}

void ABaseCharacter::Server_SetState_Implementation(ECharacterStates NewState)
{
	if (HasAuthority())
	{
		if (CurrentState == NewState)
			return;
		CurrentState = NewState;
		OnRep_SetState();
	}
}

void ABaseCharacter::Server_Targeting_Implementation()
{
	if (HasAuthority())
	{
		if (bTargeting)
			bTargeting = false;
		else
			bTargeting = true;
		OnRep_SetTargeting();
	}
}

void ABaseCharacter::Server_TakeDamage_Implementation(AActor* CauseActor, FDamageInfo DamageInfo)
{
	if (HasAuthority())
	{
		CurrentHealth -= DamageInfo.Amount;
		OnRep_SetHealth();
		Server_SetState(ECharacterStates::HIT);
		if (CurrentHealth <= 0)
		{
			Server_SetState(ECharacterStates::DEAD);
		}
	}
}

void ABaseCharacter::Server_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	Multicast_PlayAnimMontage(AnimMontage);
}

void ABaseCharacter::Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void ABaseCharacter::TargetingTimelineFunction(float Value)
{
	CameraBoom->SocketOffset = FVector(0, UKismetMathLibrary::Lerp(50, 70, Value), 0);
	CameraBoom->TargetOffset = FVector(0, 0, UKismetMathLibrary::Lerp(65, 40, Value));
	FollowCamera->FieldOfView = UKismetMathLibrary::Lerp(78, 60, Value);
	
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

void ABaseCharacter::ChangeToControllerDesiredRotation()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ABaseCharacter::ChangeToRotationToMovement()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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

	if (!CheckCurrentState({ ECharacterStates::IDLE }))
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABaseCharacter::Targeting()
{
	Server_Targeting();
}