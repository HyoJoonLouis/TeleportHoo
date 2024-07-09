#include "Leonidas.h"
#include "Kismet/GameplayStatics.h"

ALeonidas::ALeonidas()
{
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordMesh->SetupAttachment(GetMesh(), FName("Sword"));
	SwordMesh->SetCollisionProfileName(FName("NoCollision"), false);
	SwordMesh->SetReceivesDecals(false);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetupAttachment(GetMesh(), FName("Shield"));
	ShieldMesh->SetCollisionProfileName(FName("NoCollision"), false);
	ShieldMesh->SetReceivesDecals(false);
}

void ALeonidas::SetSwordAsCollision()
{
	CollisionMesh = SwordMesh;
}

void ALeonidas::SetShieldAsCollision()
{
	CollisionMesh = ShieldMesh;
}
