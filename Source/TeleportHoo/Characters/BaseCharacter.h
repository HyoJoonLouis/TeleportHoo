#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Structs/CharacterStructs.h"
#include <Components/TimelineComponent.h>
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadDelegate, AActor*, DeadCharacter);

UCLASS()
class TELEPORTHOO_API ABaseCharacter : public ACharacter
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
	//Client
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterStates GetState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckCurrentState(TArray<ECharacterStates> States) const { return States.Contains(CurrentState); }
	UFUNCTION()
	void TargetingTimelineFunction(float Value);
	UFUNCTION(BlueprintCallable)
	virtual void StartWeaponCollision();
	UFUNCTION(BlueprintCallable)
	virtual void EndWeaponCollision();
	void ChangeToControllerDesiredRotation();
	void ChangeToRotationToMovement();

	// Inputs
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Targeting();
	
	// Servers
	UFUNCTION()
	void OnRep_SetHealth();
	UFUNCTION()
	void OnRep_SetMomentum();

	UFUNCTION()
	void OnRep_SetState();
	UFUNCTION()
	void OnRep_SetTargeting();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetHealth(float Value);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetMomentum(float Value);
	UFUNCTION(Server, UnReliable, BlueprintCallable)
	void Server_SetState(ECharacterStates NewState);
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_Targeting();


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

	UPROPERTY(ReplicatedUsing = OnRep_SetState)
	ECharacterStates CurrentState;
	UPROPERTY(Replicated)
	EDamageDirection CurrentDirection;

	// Attacks
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack | Mesh")
	class UStaticMeshComponent* WeaponMesh;
	TArray<AActor*> AlreadyHitActors;
	bool bActivateCollision;
	UPROPERTY(BlueprintReadWrite)
	FDamageInfo CurrentDamageInfo;

	UPROPERTY()
	AActor* TargetActor;
	UPROPERTY(ReplicatedUsing = OnRep_SetTargeting, BlueprintReadOnly)
	bool bTargeting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline | Curve")
	UCurveFloat* TargetingCurve;
	FTimeline TargetingTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Weak")
	TMap<EDamageDirection, class UAnimMontage*> WeakAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Heavy")
	TMap<EDamageDirection, class UAnimMontage*>	HeavyAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Block")
	TMap<EDamageDirection, class UAnimMontage*> BlockMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Dodge")
	TMap<EDamageDirection, class UAnimMontage*> DodgeMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack | Hit")
	TMap<EDamageDirection, class UAnimMontage*> HitMontages;

	// Delegates
	FOnDeadDelegate OnDeadDelegate;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HealthBarComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* MomentumBarComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCharacterTrajectoryComponent* TrajectoryComponent;
};