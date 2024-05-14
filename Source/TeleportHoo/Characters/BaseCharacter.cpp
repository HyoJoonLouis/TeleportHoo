#include "BaseCharacter.h"
#include "../UI/HealthBarWidget.h"
#include "../UI/DirectionWidget.h"
#include "../UI/IngameHUD.h"
#include "../GameModes/IngamePlayerController.h"
#include "CharacterTrajectoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetReceivesDecals(false);

	AttackIndex = 0;
	CurrentState = ECharacterStates::IDLE;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetReceivesDecals(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

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
	WeaponMesh->SetReceivesDecals(false);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetupAttachment(GetMesh(), FName("Shield"));
	ShieldMesh->SetCollisionProfileName(FName("NoCollision"), false);
	ShieldMesh->SetReceivesDecals(false);

	bTargeting = false;
	bActivateCollision = false;
	TargetDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetDecal"));
	TargetDecal->SetupAttachment(GetMesh());
	TargetDecal->DecalSize = FVector(100, 100, 100);
	TargetDecal->SetRelativeRotation(FVector(0,90,0).Rotation().Quaternion());
	TargetDecal->SetVisibility(false, true);

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetMesh());
	HealthBarComponent->SetRelativeLocation(FVector(0, 0, 200));
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);

	MomentumBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MomentumBarComponent"));
	MomentumBarComponent->SetupAttachment(GetMesh());
	MomentumBarComponent->SetRelativeLocation(FVector(0, 0, 190));
	MomentumBarComponent->SetWidgetSpace(EWidgetSpace::Screen);

	DirectionComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DirectionComponent"));
	DirectionComponent->SetupAttachment(GetMesh(), FName("DirectionWidget"));
	DirectionComponent->SetRelativeLocation(FVector(0, 0, 130));
	DirectionComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DirectionComponent->SetVisibility(false, true);

	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));
	TrajectoryComponent->SetIsReplicated(true);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidget = Cast<UHealthBarWidget>(HealthBarComponent->GetWidget());
	MomentumBarWidget = Cast<UHealthBarWidget>(MomentumBarComponent->GetWidget());
	DirectionWidget = Cast<UDirectionWidget>(DirectionComponent->GetWidget());

	CurrentHealth = MaxHealth;
	CurrentMomentum = MaxMomentum / 2;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

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
			//SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()), DeltaTime, 5).Quaternion());
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

			bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetActorLocation(), GetActorLocation(), 1000, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
			if (Result)
			{
				TargetActor = HitResult.GetActor();
				auto TargetActorInterface = Cast<ICharacterInterface>(TargetActor);
				if (TargetActorInterface)
					TargetActorInterface->Execute_OnTargeted(TargetActor, this);
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

		bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(this, WeaponMesh->GetSocketLocation(FName("Start")), WeaponMesh->GetSocketLocation(FName("End")), 30.0f, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true);
		if (Result)
		{
			for (const auto& HitResult : HitResults)
			{
				ABaseCharacter* HitActor = Cast<ABaseCharacter>(HitResult.GetActor());
				if (AlreadyHitActors.Contains(HitActor) || HitActor->GetState() == ECharacterStates::DODGE)
					continue;
				AlreadyHitActors.AddUnique(HitActor);
				if (IsValid(HitActor))
				{
					HitActor->Server_TakeDamage(this, CurrentDamageInfo);
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
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ABaseCharacter::Dodge);
		EnhancedInputComponent->BindAction(WeakAttackAction, ETriggerEvent::Started, this, &ABaseCharacter::WeakAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ABaseCharacter::HeavyAttack);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &ABaseCharacter::Skill);
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
	DOREPLIFETIME(ABaseCharacter, CurrentMomentum);
	DOREPLIFETIME(ABaseCharacter, CurrentDirection);
	DOREPLIFETIME(ABaseCharacter, CurrentState);
	DOREPLIFETIME(ABaseCharacter, bTargeting);
}

void ABaseCharacter::OnTargeted_Implementation(const AActor* CauseActor)
{
	DirectionComponent->SetVisibility(true, true);
	TargetDecal->SetVisibility(true, true);
	GetMesh()->SetOverlayMaterial(TargetingOutline);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TargetingParticle, GetActorLocation() );
}

void ABaseCharacter::OnUntargeted_Implementation()
{
	DirectionComponent->SetVisibility(false, true);
	TargetDecal->SetVisibility(false, true);
	GetMesh()->SetOverlayMaterial(nullptr);
}

void ABaseCharacter::OnRep_SetHealth()
{
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealth(CurrentHealth / MaxHealth);
	}
	else
	{
		HealthBarWidget = Cast<UHealthBarWidget>(HealthBarComponent->GetWidget());
	}
}

void ABaseCharacter::OnRep_SetMomentum()
{
	if(IsValid(MomentumBarWidget))
	{
		MomentumBarWidget->SetHealth(CurrentMomentum / MaxMomentum);
	} 
	else
	{
		MomentumBarWidget = Cast<UHealthBarWidget>(MomentumBarComponent->GetWidget());
	}
}

void ABaseCharacter::OnRep_SetDirection()
{
	if (IsValid(DirectionWidget))
	{
		if (IsLocallyControlled())
		{
			DirectionWidget->ChangeDirection(CurrentDirection, CurrentState);
		}
		else
		{
			if(CurrentDirection == EDamageDirection::LEFT)
				DirectionWidget->ChangeDirection(EDamageDirection::RIGHT, CurrentState);
			else if(CurrentDirection == EDamageDirection::RIGHT)
				DirectionWidget->ChangeDirection(EDamageDirection::LEFT, CurrentState);
		}
	}
}

void ABaseCharacter::OnRep_SetState()
{
	OnRep_SetDirection();
}

void ABaseCharacter::OnRep_SetTargeting()
{
	if (bTargeting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		TargetingTimeline.PlayFromStart();
		ChangeToControllerDesiredRotation();
		AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PlayerController->GetIngameHUD()->PlayCinema(true);
		DirectionComponent->SetVisibility(true, true);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		TargetingTimeline.Reverse();
		ChangeToRotationToMovement();
		AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PlayerController->GetIngameHUD()->PlayCinema(false);
		DirectionComponent->SetVisibility(false, true);
		if (IsValid(TargetActor))
		{
			auto TargetActorInterface = Cast<ICharacterInterface>(TargetActor);
			if (TargetActorInterface)
				TargetActorInterface->Execute_OnUntargeted(TargetActor);
		}
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

void ABaseCharacter::Server_SetHealth_Implementation(float Value)
{
	if (HasAuthority())
	{
		CurrentHealth = Value;
		OnRep_SetHealth();
	}
}

void ABaseCharacter::Server_SetMomentum_Implementation(float Value)
{
	if (HasAuthority())
	{
		CurrentMomentum = Value;
		OnRep_SetMomentum();
	}
}

void ABaseCharacter::Server_SetDirection_Implementation(EDamageDirection Value)
{
	if (HasAuthority())
	{
		if (CurrentDirection == Value)
			return;
		CurrentDirection = Value;
		OnRep_SetDirection();
	}
}

void ABaseCharacter::Server_Targeting_Implementation()
{
	if (HasAuthority())
	{
		bTargeting = !bTargeting;
		OnRep_SetTargeting();
	}
}

void ABaseCharacter::Server_WeakAttack_Implementation(int Index)
{
	Server_SetState(ECharacterStates::ATTACK);
	if(CurrentDirection == EDamageDirection::RIGHT)
	{
		Server_PlayAnimMontage(WeakAttackMontages[EDamageDirection::RIGHT].AttackMontages[Index]);
	}
	else
	{
		Server_PlayAnimMontage(WeakAttackMontages[EDamageDirection::LEFT].AttackMontages[Index]);
	}
}


void ABaseCharacter::Server_HeavyAttack_Implementation()
{
	Server_SetState(ECharacterStates::ATTACK);
	if (CurrentDirection == EDamageDirection::RIGHT)
	{
		Server_PlayAnimMontage(HeavyAttackMontages[EDamageDirection::RIGHT]);
	}
	else
	{
		Server_PlayAnimMontage(HeavyAttackMontages[EDamageDirection::RIGHT]);
	}
}


void ABaseCharacter::Server_TargetBlockAttack_Implementation(AActor* Attacker, AActor* Blocker, EDamageDirection Direction)
{
	ABaseCharacter* AttackActor = Cast<ABaseCharacter>(Attacker);
	ABaseCharacter* BlockActor = Cast<ABaseCharacter>(Blocker);

	AttackActor->Server_SetState(ECharacterStates::STUN);
	BlockActor->Server_SetState(ECharacterStates::BLOCK);

	BlockActor->Server_SetMomentum(BlockActor->GetCurrentMomentum() + BlockActor->GetActorMomentumValues().OnBlockSucceedAddAmount);

	if (Direction == EDamageDirection::LEFT)
	{
		AttackActor->Server_PlayAnimMontage(WeakAttackBlockedMontages[EDamageDirection::LEFT]);
		BlockActor->Server_PlayAnimMontage(BlockMontages[EDamageDirection::LEFT]);
	}
	else if (Direction == EDamageDirection::RIGHT)
	{
		AttackActor->Server_PlayAnimMontage(WeakAttackBlockedMontages[EDamageDirection::RIGHT]);
		BlockActor->Server_PlayAnimMontage(BlockMontages[EDamageDirection::LEFT]);
	}

}

void ABaseCharacter::Client_TakeDamage_Implementation(AActor* CauseActor, FDamageInfo DamageInfo)
{
	AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->GetIngameHUD()->OnHitEffect();
}

void ABaseCharacter::Server_TakeDamage_Implementation(AActor* CauseActor, FDamageInfo DamageInfo)
{
	if (HasAuthority())
	{
		if (GetState() == ECharacterStates::DEAD)
			return;

		ABaseCharacter* DamageActor = Cast<ABaseCharacter>(CauseActor);
		if ((CurrentDirection == EDamageDirection::RIGHT && DamageActor->GetActorDirection() == EDamageDirection::LEFT)
			|| CurrentDirection == EDamageDirection::LEFT && DamageActor->GetActorDirection() == EDamageDirection::RIGHT)
			return;

		CurrentHealth -= DamageInfo.Amount;
		DamageActor->Server_SetMomentum(DamageActor->GetCurrentMomentum() + DamageActor->GetActorMomentumValues().OnHitSucceedAddAmount);

		ABaseCharacter* BaseCharacterCauseActor = Cast<ABaseCharacter>(CauseActor);
		Client_TakeDamage(CauseActor, DamageInfo);
		BaseCharacterCauseActor->Server_SetMomentum(BaseCharacterCauseActor->GetCurrentMomentum() + BaseCharacterCauseActor->GetActorMomentumValues().OnHitSucceedAddAmount);
		OnRep_SetHealth();

		Server_SetState(ECharacterStates::HIT);
		if (DamageInfo.DamageDirection == EDamageDirection::RIGHT)
			Server_PlayAnimMontage(HitMontages[EDamageDirection::RIGHT]);
		else if (DamageInfo.DamageDirection == EDamageDirection::LEFT)
			Server_PlayAnimMontage(HitMontages[EDamageDirection::LEFT]);

		if (CurrentHealth <= 0)
		{
			Server_SetState(ECharacterStates::DEAD);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dead"));
			if (OnDeadDelegate.IsBound())	
				OnDeadDelegate.Broadcast(this);
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
	CameraBoom->SocketOffset = FVector(0, UKismetMathLibrary::Lerp(50, 100, Value), 0);
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

bool ABaseCharacter::CanTargetBlockAttack()
{
	if(bTargeting && IsValid(TargetActor))
	{
		if (FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation()) >= 300)
			return false;
		ABaseCharacter* Target = Cast<ABaseCharacter>(TargetActor);
		if ((CurrentDirection == EDamageDirection::RIGHT && Target->GetActorDirection() == EDamageDirection::LEFT) 
			|| CurrentDirection == EDamageDirection::LEFT && Target->GetActorDirection() == EDamageDirection::RIGHT)
		{
			return true;
		}
	}
	return false;
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
	MovementVector = Value.Get<FVector2D>();
	if (!CheckCurrentState({ECharacterStates::IDLE}))
		return;

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
	if (!CheckCurrentState({ ECharacterStates::IDLE }))
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (bTargeting && IsValid(TargetActor))
	{
		if (LookAxisVector.X <= -0.5f)
		{
			Server_SetDirection(EDamageDirection::LEFT);
		}
		else if (LookAxisVector.X >= 0.5f)
		{
			Server_SetDirection(EDamageDirection::RIGHT);
		}
	}

	else
	{
		if (Controller != nullptr)
		{
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void ABaseCharacter::Targeting()
{
	Server_Targeting();
}

void ABaseCharacter::Dodge()
{
	if (GetState() != ECharacterStates::IDLE)
		return;
	if (bTargeting && IsValid(TargetActor))
	{
		if (CurrentMomentum < MomentumValues.OnDodgeRemoveAmount)
			return;
		Server_SetMomentum(CurrentMomentum - MomentumValues.OnDodgeRemoveAmount);
		if (MovementVector.X > 0)
			Server_PlayAnimMontage(DodgeMontages[EDamageDirection::RIGHT]);
		else if (MovementVector.X < 0)
			Server_PlayAnimMontage(DodgeMontages[EDamageDirection::LEFT]);
	}
	else
	{
		Server_PlayAnimMontage(ForwardDodgeMontage);
	}

}

void ABaseCharacter::WeakAttack()
{
	if (GetState() != ECharacterStates::IDLE)
		return;
	Server_WeakAttack(AttackIndex);
	AttackIndex++;
	if (AttackIndex > 1)
		AttackIndex = 0;
}

void ABaseCharacter::HeavyAttack()
{
	if (GetState() != ECharacterStates::IDLE)
		return;
	
	AttackIndex = 0;
	
	if (bTargeting && IsValid(TargetActor) && Cast<ABaseCharacter>(TargetActor)->GetState() == ECharacterStates::PARRIABLE)
	{
		Parry(TargetActor, this);
		return;
	}
	
	Server_HeavyAttack();
}

void ABaseCharacter::Parry(AActor* Attacker, AActor* Blocker)
{
	Server_TargetBlockAttack(Attacker, Blocker, CurrentDirection);
}

void ABaseCharacter::Skill()
{
	if (GetState() != ECharacterStates::IDLE)
		return;
	if (CurrentMomentum < MomentumValues.OnSkillRemoveAmount)
		return;
	Server_SetState(ECharacterStates::SKILL);
	Server_SetMomentum(CurrentMomentum - MomentumValues.OnSkillRemoveAmount);
	Server_PlayAnimMontage(SkillMontage);
}
