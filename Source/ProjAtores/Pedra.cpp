// Fill out your copyright notice in the Description page of Project Settings.


#include "Pedra.h"

#include "Components/SphereComponent.h"

// Sets default values
APedra::APedra()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColisorPedra = CreateDefaultSubobject<USphereComponent>(FName("Colisor"));
	ColisorPedra->SetSphereRadius(12.f);
	ColisorPedra->SetSphereRadius(ECollisionEnabled::QueryAndPhysics);
	RootComponent = ColisorPedra;

	MalhaPedra = CreateDefaultSubobject<UStaticMeshComponent>(FName("Malha"));
	MalhaPedra->SetRelativeLocation(FVector(2.3f, 0.f, 0.f));
	MalhaPedra->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	MalhaPedra->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>Pedra(TEXT("StaticMesh'/Game/FBX/fbx_f_1500_v_752.fbx_f_1500_v_752'"));
	if(Pedra.Succeeded()) {
		MalhaPedra->SetStaticMesh(Pedra.Object);
	}

	
}

// Called when the game starts or when spawned
void APedra::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APedra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

