// Fill out your copyright notice in the Description page of Project Settings.


#include "Granada.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h" // Usado para manipular a fun��o SetGlocalTimeDilation()

// Sets default values
AGranada::AGranada()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColisorDoAtor = CreateDefaultSubobject<USphereComponent>(FName("Collider"));
	ColisorDoAtor->SetSphereRadius(12.f);
	ColisorDoAtor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = ColisorDoAtor;

	MalhaDoAutor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	MalhaDoAutor->SetRelativeLocation(FVector(3.2f, 0.f, 0.f));
	MalhaDoAutor->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	MalhaDoAutor->SetupAttachment(RootComponent);

	// Som da explos�o
	SomDaExplosao = CreateDefaultSubobject<UAudioComponent>(FName("Explosion sound"));
	SomDaExplosao->bAutoActivate = false;
	SomDaExplosao->bAutoDestroy = true;
	SomDaExplosao->SetupAttachment(MalhaDoAutor);

	Explosao = CreateDefaultSubobject<UParticleSystemComponent>(FName("Explosion particle"));
	Explosao->bAutoActivate = false;
	Explosao->bAutoDestroy = true;
	Explosao->SetupAttachment(MalhaDoAutor);

	ConstructorHelpers::FObjectFinder<UParticleSystem>ParticulaExplosao(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	
	ConstructorHelpers::FObjectFinder<USoundCue>SomGranada(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>Granada(TEXT("StaticMesh'/Game/FBX/granada.granada'"));

	if (Granada.Succeeded() && SomGranada.Succeeded() && ParticulaExplosao.Succeeded())
	{
		MalhaDoAutor->SetStaticMesh(Granada.Object);
		SomDaExplosao->SetSound(SomGranada.Object);
		Explosao->SetTemplate(ParticulaExplosao.Object);
	}
	
}

// Called when the game starts or when spawned
void AGranada::BeginPlay()
{
	Super::BeginPlay();
	TempoDevagar(0.1);
	MalhaDoAutor->SetSimulatePhysics(true);
	MalhaDoAutor->SetEnableGravity(true);
	GetWorldTimerManager().SetTimer(Temporizador, this, &AGranada::ExplodirGranada, 5.f, false);
}

void AGranada::TempoDevagar(float DilatacaoDoTempo)
{
	// Define a dilata��o do tempo global que deve estar entre 0 e 1 em float
	// Como par�metro deve ser passado o MUNDO e a dilata��o de tempo
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), DilatacaoDoTempo);
	// Essa fun��o executa a restaura��o do tempo ap�s 3.0 * a dilata��o do tempo
	GetWorldTimerManager().SetTimer(TimerDoTempo, this, &AGranada::RestaurarTempo, 3.f * DilatacaoDoTempo, false);
}

void AGranada::RestaurarTempo()
{
	// Aqui o tempo global ser� restaurado ao normal que � 1.0f e depois o Handle de tempo TimerDoTempo ser� liberado
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorldTimerManager().ClearTimer(TimerDoTempo);
}


void AGranada::ExplodirGranada()
{
	SetLifeSpan(5.5f);
	SomDaExplosao->Play();
	TempoDevagar(0.05); // Antes da explos�o o tempo ser� mais lento e depois de 2*0.05 ele voltar� ao normal.
	Explosao->ActivateSystem();
	MalhaDoAutor->SetVisibility(false);
}

void AGranada::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Ser� colocado na tela durante 5 segundos o texto indicado
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Destroyed Grenade");
	// Aqui estamos excluindo o manupulador(handle) de nome Temporizador
	// A partir daqui ele ser� invalidado
	GetWorldTimerManager().ClearTimer(Temporizador);
}


// Called every frame
void AGranada::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

