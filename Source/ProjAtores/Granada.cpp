// Fill out your copyright notice in the Description page of Project Settings.


#include "Granada.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h" // Usado para manipular a função SetGlocalTimeDilation()

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

	// Som da explosão
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
	// Define a dilatação do tempo global que deve estar entre 0 e 1 em float
	// Como parâmetro deve ser passado o MUNDO e a dilatação de tempo
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), DilatacaoDoTempo);
	// Essa função executa a restauração do tempo após 3.0 * a dilatação do tempo
	GetWorldTimerManager().SetTimer(TimerDoTempo, this, &AGranada::RestaurarTempo, 3.f * DilatacaoDoTempo, false);
}

void AGranada::RestaurarTempo()
{
	// Aqui o tempo global será restaurado ao normal que é 1.0f e depois o Handle de tempo TimerDoTempo será liberado
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorldTimerManager().ClearTimer(TimerDoTempo);
}

// Cria um Impulso radial de força e raio
void AGranada::ImpulsoRadial(float Raio, float Forca)
{
	//NOTA:Busca todos os objetos da cena pelo mundo percorrendo um container(coleção de elementos)
	//     Os Iteradores de Ator(TActorIterator) e os Iteradores de Object(TObjectIterator) sempre retorna
	//     uma lista real de todos os atores/objetos que ainda estão ativaos no mundo do jogo.
	//     
	//OBS: Essas funções podem ser usadas para procurar todas as instâncias de atores e objetos, ou apenas
	//     subclasses específicas, em tempo de execução, removendo a necessidade de manter matrizes dinâmicas
	//     de atores e tendo que lembrar de remover os atores após a destruição.
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nome: %s, %x"), *ActorItr->GetName(), *ActorItr);
		UStaticMeshComponent* RaizDoStaticMesh;
		RaizDoStaticMesh = Cast<UStaticMeshComponent>(ActorItr->GetRootComponent());
		if(RaizDoStaticMesh)
		{
			if(RaizDoStaticMesh->Mobility == EComponentMobility::Movable)
			{
				RaizDoStaticMesh->SetSimulatePhysics(true);
				RaizDoStaticMesh->AddRadialImpulse(GetActorLocation(), Raio, Forca, ERadialImpulseFalloff::RIF_Linear, true);
			}
		}
	}

	// Iteração sobre os Objetos por isso o uso do TObjectIterator
	// NOTE: O objetivo do TObjectIterator é essencialmente encontrar todos os objetos de um tipo e colocar os resultados em um
	//       ObjectArray que o TObjectIterator  finaliza com nullptr.
	// O construtor de TOjectIterator chama a função GetObjectsOfClass() para limitar o escopo da pesquisa a uma classe UObject
	// escolhida em TObjectIterator<UObject>
	for (TObjectIterator<USkeletalMeshComponent> It; It; ++It)
	{
		// Verifica se é do mesmo mundo
		if (It->GetWorld() == GetWorld())
		{
			UE_LOG(LogTemp, Warning, TEXT("Nome: %s End: %X \n"), *It->GetName(), *It);
			It->SetSimulatePhysics(true);
			It->AddRadialImpulse(GetActorLocation(), Raio, Forca, ERadialImpulseFalloff::RIF_Linear, true);
		}
	}
}

void AGranada::ExplodirGranada()
{
	SetLifeSpan(5.5f);
	SomDaExplosao->Play();
	TempoDevagar(0.05); // Antes da explosão o tempo será mais lento e depois de 2*0.05 ele voltará ao normal.
	Explosao->ActivateSystem();
	MalhaDoAutor->SetVisibility(false);
	ImpulsoRadial(5000.f, 1000.f);
}

void AGranada::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Será colocado na tela durante 5 segundos o texto indicado
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Destroyed Grenade");
	// Aqui estamos excluindo o manupulador(handle) de nome Temporizador
	// A partir daqui ele será invalidado
	GetWorldTimerManager().ClearTimer(Temporizador);
}


// Called every frame
void AGranada::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

