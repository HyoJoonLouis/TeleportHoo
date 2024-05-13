#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Structs/CharacterStructs.h"
#include "../Interfaces/CharacterInterface.h"
#include <Components/TimelineComponent.h>
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadDelegate, AActor*, DeadCharacter);

USTRUCT(Blueprintable)
struct FAttackAnimMontages
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UAnimMontage*> AttackMontages;

	FORCEINLINE uint32 GetLength() { return AttackMontages.Num(); }
};

UCLASS()
class TELEPORTHOO_API ABaseCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	//Interfaces
	virtual void OnTargeted_Implementation(const AActor* CauseActor) override;
	virtual void OnUntargeted_Implementation() override;

	//Client
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterStates GetState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckCurrentState(TArray<ECharacterStates> States) const { return States.Contains(CurrentState); }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentMomentum() const { return CurrentMomentum; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FMomentumValues GetActorMomentumValues() const { return MomentumValues; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EDamageDirection GetActorDirection() const { return CurrentDirection; }


	UFUNCTION()
	void TargetingTimelineFunction(float Value);
	UFUNCTION(BlueprintCallable)
	virtual void StartWeaponCollision();
	UFUNCTION(BlueprintCallable)
	virtual void EndWeaponCollision();
	UFUNCTION(BlueprintCallable)
	bool CanTargetBlockAttack();

	void ChangeToControllerDesiredRotation();
	void ChangeToRotationToMovement();

	// Inputs
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Targeting();
	UFUNCTION()
	void Dodge();
	UFUNCTION()
	void WeakAttack();
	UFUNCTION()
	void HeavyAttack();
	UFUNCTION()
	void Skill();
	
	// Servers
	UFUNCTION()
	void OnRep_SetState();
	UFUNCTION()
	void OnRep_SetHealth();
	UFUNCTION()
	void OnRep_SetMomentum();
	UFUNCTION()
	void OnRep_SetDirection();
	UFUNCTION()
	void OnRep_SetTargeting();

	UFUNCTION(Server, UnReliable, BlueprintCallable)
	void Server_SetState(ECharacterStates NewState);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetHealth(float Value);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetMomentum(float Value);
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_SetDirection(EDamageDirection Value);
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_Targeting();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_WeakAttack(int Index);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_HeavyAttack();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_TargetBlockAttack(AActor* Attacker, AActor* Blocker, EDamageDirection Direction);

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void Client_TakeDamage(AActor* CauseActor, FDamageInfo DamageInfo);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_TakeDamage(AActor* CauseActor, FDamageInfo DamageInfo);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_PlayAnimMontage(class UAnimMontage* AnimMontage);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_PlayAnimMontage(class UAnimMontage* AnimMontage);

protected:
	// Cameras
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* TargetingAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* WeakAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SkillAction;

	UPROPERTY()
	FVector2D MovementVector;

	// Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Health")
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_SetHealth, EditAnywhere, BlueprintReadWrite, Category = "Status | Health")
	float CurrentHealth;
	UPROPERTY()
	class UHealthBarWidget* HealthBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Momentum")
	float MaxMomentum;
	UPROPERTY(ReplicatedUsing = OnRep_SetMomentum, EditAnywhere, BlueprintReadWrite, Category = "Status | Momentum")
	float CurrentMomentum;
	UPROPERTY()
	class UHealthBarWidget* MomentumBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Momentum")
	FMomentumValues MomentumValues;

	UPROPERTY(ReplicatedUsing = OnRep_SetState)
	ECharacterStates CurrentState;
	UPROPERTY(ReplicatedUsing = OnRep_SetDirection)
	EDamageDirection CurrentDirection;
	UPROPERTY()
	class UDirectionWidget* DirectionWidget;

	// Attacks
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* ShieldMesh;

	UPROPERTY(BlueprintReadWrite)
	uint8 AttackIndex;
	TArray<AActor*> AlreadyHitActors;
	bool bActivateCollision;
	UPROPERTY(BlueprintReadWrite)
	FDamageInfo CurrentDamageInfo;

	// Targeting
	UPROPERTY(BlueprintReadOnly)
	AActor* TargetActor;
	UPROPERTY(ReplicatedUsing = OnRep_SetTargeting, BlueprintReadOnly)
	bool bTargeting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	class UDecalComponent* TargetDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	class UMaterialInstance* TargetingOutline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	class UNiagaraSystem* TargetingParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline | Curve")
	UCurveFloat* TargetingCurve;
	FTimeline TargetingTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Weak")
	TMap<EDamageDirection, FAttackAnimMontages> WeakAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Weak")
	TMap<EDamageDirection, class UAnimMontage*> WeakAttackBlockedMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Heavy")
	TMap<EDamageDirection, class UAnimMontage*>	HeavyAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Block")
	TMap<EDamageDirection, class UAnimMontage*> BlockMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Dodge")
	class UAnimMontage* ForwardDodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Dodge")
	TMap<EDamageDirection, class UAnimMontage*> DodgeMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Hit")
	TMap<EDamageDirection, class UAnimMontage*> HitMontages;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Attack | Skill")
	class UAnimMontage* SkillMontage;


	// Delegates
	FOnDeadDelegate OnDeadDelegate;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HealthBarComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* MomentumBarComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* DirectionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCharacterTrajectoryComponent* TrajectoryComponent;
};