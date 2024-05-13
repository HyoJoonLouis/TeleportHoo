#pragma once

#include "CoreMinimal.h"
#include "CharacterStructs.generated.h"

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	IDLE		UMETA(DisplayName = "Idle"),
	ATTACK		UMETA(DisplayName = "Attack"),
	BLOCK		UMETA(DisplayName = "Block"),
	STUN		UMETA(DisplayName = "Stun"),
	PARRIABLE	UMETA(DisplayName = "Parriable"),
	DODGE		UMETA(DisplayName = "Dodge"),
	HIT			UMETA(DisplayName = "Hit"),
	DEAD		UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EDamageDirection : uint8
{
	NONE	UMETA(DisplayName = "None"),
	LEFT	UMETA(DisplayName = "Left"),
	RIGHT	UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	WEAK	UMETA(DisplayName = "Weak"),
	HEAVY	UMETA(DisplayName = "Heavy"),
};

USTRUCT(Blueprintable)
struct FDamageInfo
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageDirection DamageDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;
};

USTRUCT(Blueprintable)
struct FMomentumValues
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OnHitSucceedAddAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OnBlockSucceedAddAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OnDodgeRemoveAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OnSkillRemoveAmount;

};