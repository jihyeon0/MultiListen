#include "Hammer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AHammer::AHammer()
{
	PrimaryActorTick.bCanEverTick = true;
	Cooltime = 3.f;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	FRotator NewRotation = FRotator(90.f, 0, 90.f);
	MeshComp->AddRelativeRotation(NewRotation);
	SetRootComponent(MeshComp);
	SphereComp->SetupAttachment(MeshComp);
	

	//콜리전 바인딩
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHammer::OnCollisionOverlap);

	if (GEngine)
	{
		//UKismetSystemLibrary::PrintString(this, "Hammer Initialized");
	}
}

void AHammer::BeginPlay()
{
	Super::BeginPlay();

	//UKismetSystemLibrary::PrintString(this, "Hammer BeginPlay");	
}

void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = FRotator(0, 360.f, 0.f);

	AddActorWorldRotation(NewRotation * DeltaTime);

	//FVector Pos = GetActorLocation();
	//UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("OwnerPosition: X=%.2f, Y=%.2f, Z=%.2f"), OwnerPosition.X, OwnerPosition.Y, OwnerPosition.Z));
	//UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("HammerPos: X=%.2f, Y=%.2f, Z=%.2f"), Pos.X, Pos.Y, Pos.Z));
}

void AHammer::Activate()
{
	Super::Activate();
	
}

void AHammer::ApplyDamage()
{
	Super::ApplyDamage();

}

void AHammer::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

