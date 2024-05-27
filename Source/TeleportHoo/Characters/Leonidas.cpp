#include "Leonidas.h"
#include "Kismet/GameplayStatics.h"

ALeonidas::ALeonidas()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("Sword"));
	WeaponMesh->SetCollisionProfileName(FName("NoCollision"), false);
	WeaponMesh->SetReceivesDecals(false);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetupAttachment(GetMesh(), FName("Shield"));
	ShieldMesh->SetCollisionProfileName(FName("NoCollision"), false);
	ShieldMesh->SetReceivesDecals(false);
}

void ALeonidas::Tick(float DeltaTime)
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

void ALeonidas::StartWeaponCollision()
{
	bActivateCollision = true;
	AlreadyHitActors.Empty();
}

void ALeonidas::EndWeaponCollision()
{
	bActivateCollision = false;
}