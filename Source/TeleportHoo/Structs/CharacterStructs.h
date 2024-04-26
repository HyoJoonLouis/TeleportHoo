#pragma once

#include "CoreMinimal.h"
#include "CharacterStructs.generated.h"

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	IDLE	UMETA(DisplayName = "Idle"),
	ATTACK	UMETA(DisplayName = "Attack"),
	BLOCK	UMETA(DisplayName = "Block"),
	DODGE	UMETA(DisplayName = "Dodge"),
};

UENUM(BlueprintType)
enum class EDamageDirection : uint8
{
	LEFT	UMETA(DisplayName = "Left"),
	RIGHT	UMETA(DisplayName = "Right")
};

USTRUCT(Blueprintable)
struct FDamageInfo
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	EDamageDirection DamageType;
};