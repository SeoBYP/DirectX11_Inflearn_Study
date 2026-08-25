# DirectX 11 Study — 렌더링 파이프라인부터 엔진 구조까지 직접 따라 만들기

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white)
![DirectX 11](https://img.shields.io/badge/DirectX-11-107C10?logo=windows&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2026%20(v145)-5C2D91?logo=visualstudio&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-0078D6?logo=windows&logoColor=white)
![Type](https://img.shields.io/badge/Type-학습%20프로젝트-lightgrey)

> **인프런 DirectX 11 강의를 따라가며 만든 학습용 엔진입니다.**
> 상용 엔진이 감춰 주는 계층 — 정점 버퍼를 GPU에 올리고, 상수 버퍼로 행렬을 넘기고,
> 셰이더 패스를 골라 드로우콜을 부르기까지 — 를 직접 만들며 확인했습니다.
> 구현하면서 이해한 내용은 **[블로그 32편](https://unialgames.tistory.com/category/DirectX)** 으로 정리했고, 아래 [학습 내용](#-학습-내용--블로그-연재-32편)에 단계별로 묶어 두었습니다.

![SceneDemo](docs/images/SceneDemo.gif)

**[📚 학습 내용](#-학습-내용--블로그-연재-32편)** · [🧱 엔진 구조](#-엔진-구조) · [🖼 실행 화면](#-실행-화면) · [🔧 빌드](#-빌드--실행)

---

## 🧾 한눈에 보기

| 항목 | 내용 |
|---|---|
| **성격** | 강의 기반 학습 프로젝트 (포트폴리오상 **보조 프로젝트**) |
| **언어 / 표준** | C++20 |
| **그래픽 API** | Direct3D 11, Effects11(FX11) 셰이더 프레임워크 |
| **빌드 환경** | Visual Studio 2026 · MSVC v145 · Windows SDK 10.0 · Debug\|x64 |
| **솔루션** | 2개 / 프로젝트 5개 |
| **엔진 코드** | 소스 55개 · 헤더 59개 (서드파티 제외) |
| **데모** | 36개 (씬 기반으로 실행 가능한 것 8개) |
| **셰이더** | `.fx` 33개 |
| **학습 기록** | [블로그 연재 32편](https://unialgames.tistory.com/category/DirectX) |

---

## 📚 학습 내용 — 블로그 연재 32편

강의를 따라가며 **이해한 내용을 매번 글로 정리**했습니다. 아래는 그 32편을 학습 순서대로 7단계로 묶은 것입니다.
각 단계마다 **저장소의 어느 코드에 해당하는지** 함께 적었습니다.

### 1단계 · 엔진 뼈대 — "Unity는 내부에서 뭘 하고 있나"

컴포넌트 기반 구조를 밑바닥부터 만들어 보며, 상용 엔진이 제공하는 개념들이 실제로 어떤 코드인지 확인한 단계입니다.
→ 해당 코드: [`DirectX_InflearnCode_Chapter2/`](DirectX/DirectX_InflearnCode/DirectX_InflearnCode_Chapter2)

| 글 | 다룬 내용 |
|---|---|
| [GameObject, Transform](https://unialgames.tistory.com/entry/DirectX11GameObjectTransform) | 게임 오브젝트와 계층 구조, 로컬/월드 행렬 계산 |
| [Component, MonoBehaviour](https://unialgames.tistory.com/entry/DirectX11ComponentMonobehaviour) | 컴포넌트 생명주기(Awake/Start/Update/LateUpdate)와 스크립트 분리 |
| [SceneManager와 MeshRenderer](https://unialgames.tistory.com/entry/DirectX11SceneManagerAndMeshRenderer) | 씬 전환과 오브젝트 순회, 렌더러 컴포넌트의 역할 |
| [ResourceManager](https://unialgames.tistory.com/entry/DirectX11ResourceManager) | 메시·텍스처·셰이더를 이름으로 관리하고 재사용하는 구조 |
| [RenderManager](https://unialgames.tistory.com/entry/DirectX11RenderManager) | 렌더링 데이터를 모아 상수 버퍼로 밀어 넣는 흐름 |
| [Animation System](https://unialgames.tistory.com/entry/DirectX11AnimationSystem) | 키프레임 기반 스프라이트 애니메이션과 재생 상태 관리 |

### 2단계 · 렌더링 기초 — 정점에서 픽셀까지

→ 해당 코드: [`Client/01~11`](DirectX/DirectX_Client/Client), [`Shaders/01~07`](DirectX/DirectX_Client/Shaders)

| 글 | 다룬 내용 |
|---|---|
| [3D Mesh 렌더링하기](https://unialgames.tistory.com/entry/DirectX113DMeshRendering) | 정점/인덱스 버퍼 구성과 입력 레이아웃 |
| [Sampler의 이해](https://unialgames.tistory.com/entry/DirectX11Sampler) | 필터링과 주소 지정 모드가 화면에 미치는 영향 |
| [Normal의 이해와 활용](https://unialgames.tistory.com/entry/DirectX11NormalMap) | 법선 벡터의 의미와 변환 시 주의점 |
| [Depth Stencil의 이해와 활용](https://unialgames.tistory.com/entry/DirectX11DepthStencil) | 깊이 버퍼가 무엇을 판정하는가, 스텐실의 쓰임 |
| [HeightMap을 활용한 지형 생성](https://unialgames.tistory.com/entry/DirectX11HeightMap) | 높이 맵을 읽어 정점 격자를 만드는 지형 생성 |

### 3단계 · 조명과 재질 — 빛을 항으로 분해하기

→ 해당 코드: [`Client/12~18`](DirectX/DirectX_Client/Client), [`Engine/Light`](DirectX/DirectX_Client/Engine/Light.h), [`Shaders/09~14`](DirectX/DirectX_Client/Shaders)

| 글 | 다룬 내용 |
|---|---|
| [Lighting의 이해와 활용 — Ambient, Diffuse, Specular, Emissive](https://unialgames.tistory.com/entry/DirectX11LightingAmbientDiffuseSpecularEmissive) | 조명을 네 항으로 나눠 각각 무엇을 담당하는지 |
| [Normal Mapping](https://unialgames.tistory.com/entry/DirectX11NormalMapping) | 탄젠트 공간과 법선 맵으로 디테일을 표현하는 원리 |

### 4단계 · 모델과 애니메이션 — 외부 애셋 파이프라인

→ 해당 코드: [`AssimpTool/`](DirectX/DirectX_Client/AssimpTool), [`Engine/Model·ModelAnimator`](DirectX/DirectX_Client/Engine)

| 글 | 다룬 내용 |
|---|---|
| [모델 가져오기(Model Import)](https://unialgames.tistory.com/entry/DirectX11ModelImport) | Assimp로 읽어 자체 포맷(`.mesh`)으로 사전 변환하는 이유 |
| [애니메이션 이해(Animation)](https://unialgames.tistory.com/entry/DrectX11AniamtionData) | 본 계층과 키프레임 데이터, 스키닝의 구조 |
| [애니메이션 트위닝(Animation Tweening)](https://unialgames.tistory.com/entry/DirectX11AnimationTweening) | 두 클립 사이를 보간해 자연스럽게 전환하기 |

### 5단계 · 드로우콜 최적화 — "왜 그리는 횟수가 비용인가"

같은 메시를 여러 번 그릴 때 무엇이 병목이고, GPU에 대량 데이터를 넘기는 방법이 왜 여러 가지인지 다룬 단계입니다.
→ 해당 코드: [`Engine/InstancingManager`](DirectX/DirectX_Client/Engine/InstancingManager.h), [`Engine/RawBuffer·TextureBuffer·StructuredBuffer`](DirectX/DirectX_Client/Engine)

| 글 | 다룬 내용 |
|---|---|
| [인스턴싱(Instancing)과 드로우 콜(Draw Call)](https://unialgames.tistory.com/entry/DirectX11InstancingDrawCall) | 드로우콜이 비용인 이유와 인스턴싱이 줄이는 것 |
| [Mesh, Model, Animation Instancing](https://unialgames.tistory.com/entry/DirectX11MeshModelAnimationInstancing) | 정적 메시 / 모델 / 스키닝 애니메이션을 각각 묶는 방법 |
| [RawBuffer](https://unialgames.tistory.com/entry/DirectX11RawBuffer) | 바이트 단위로 접근하는 버퍼와 컴퓨트 셰이더 연동 |
| [TextureBuffer](https://unialgames.tistory.com/entry/DirectX11TextureBuffer) | 텍스처를 데이터 저장소로 쓰는 방식 |
| [StructuredBuffer](https://unialgames.tistory.com/entry/DirectX11StructuredBuffer) | 구조체 배열을 셰이더에 넘기는 방식과 선택 기준 |

### 6단계 · 공간과 충돌 — 판정의 수학

→ 해당 코드: [`Engine/*Collider`](DirectX/DirectX_Client/Engine), [`Client/CollisionDemo`](DirectX/DirectX_Client/Client/CollisionDemo.cpp)

| 글 | 다룬 내용 |
|---|---|
| [기본 게임 도형(Sphere, AABB, OBB 등)](https://unialgames.tistory.com/entry/DirectX11BasicShapes) | 각 바운딩 볼륨의 표현 방식과 트레이드오프 |
| [기본 게임 도형과 Point Test](https://unialgames.tistory.com/entry/DirectX11BasicShapesPointTest) | 점이 도형 안에 있는지 판정 |
| [기본 게임 도형과 Intersection](https://unialgames.tistory.com/entry/DirectX11BasicShapesIntersection) | 도형 간 교차 검사 |
| [기본 게임 도형과 Raycast](https://unialgames.tistory.com/entry/DirectX11BasicShapeRaycast) | 광선과 도형의 교차, 피킹의 기초 |
| [Triangle의 Point Test, Interaction, Raycast](https://unialgames.tistory.com/entry/DirectX11TrianglePointTestInteractionRaycast) | 삼각형 단위 정밀 판정 |
| [AABB Collision과 OBB Collision](https://unialgames.tistory.com/entry/DirectX11AABBCollisionOBBCollision) | 축 정렬과 지향 경계 상자의 차이, SAT |
| [Collision과 SphereCollider](https://unialgames.tistory.com/entry/DirectX11CollisionAndSphereCollider) | 콜라이더를 컴포넌트로 붙여 씬에서 쓰기 |

### 7단계 · 화면과 연출

→ 해당 코드: [`Engine/Viewport·Billboard·SnowBillboard·Button`](DirectX/DirectX_Client/Engine)

| 글 | 다룬 내용 |
|---|---|
| [Viewport 이해](https://unialgames.tistory.com/entry/DirectX11Viewport) | 뷰포트 변환과 화면 분할 |
| [직교투영(Orthographic Projection)과 UI](https://unialgames.tistory.com/entry/DirectX11OrthographicProjectionAndUI) | 원근과 직교를 한 화면에 섞어 UI 그리기 |
| [스카이박스와 스카이돔](https://unialgames.tistory.com/entry/DrectX11SkyBoxSkyDome) | 배경을 항상 카메라 뒤에 두는 방법 |
| [빌보드(Billboard)와 파티클(Particle)](https://unialgames.tistory.com/entry/DirectX11BillboardAndParticle) | 카메라를 향해 회전하는 쿼드, 셰이더에서 위치 계산하기 |

---

## 🗂 저장소 구조

```
DirectX/
├─ DirectX_Client/              # 메인 — 자체 DX11 엔진
│  ├─ Engine/                   #   정적 라이브러리 (55 cpp / 59 h)
│  ├─ Client/                   #   데모 36종 + 진입점
│  ├─ AssimpTool/               #   Assimp → 자체 포맷 변환 툴
│  ├─ Shaders/                  #   .fx 셰이더 33개
│  ├─ Resources/                #   모델 · 텍스처 · 애셋
│  └─ Libraries/                #   Assimp · DirectXTex · FX11
│
├─ DirectX_InflearnCode/        # 이전 세대 — 엔진 구조 학습 단계
│  ├─ DirectX_InflearnCode/     #   Chapter1: 초기화 · 삼각형 · 파이프라인
│  └─ ..._Chapter2/             #   Chapter2: 컴포넌트 구조 + 매니저 4종
│
└─ imgui-master/                # Dear ImGui (벤더링)
```

두 솔루션은 **같은 내용을 두 번 만든 기록**입니다.
`DirectX_InflearnCode` 에서 컴포넌트 구조와 매니저를 먼저 익히고(1단계),
`DirectX_Client` 에서 그 위에 렌더링 기법과 모델·애니메이션까지 확장했습니다(2~7단계).

---

## 🧱 엔진 구조

`Engine` 프로젝트는 정적 라이브러리로 빌드되어 `Client` / `AssimpTool` 이 링크합니다.

| 영역 | 구성 요소 |
|---|---|
| **코어 루프** | `Game` · `Graphics` · `Viewport` · `TimeManager` · `InputManager` · `IExecute` |
| **씬 · 오브젝트** | `Scene` · `SceneManager` · `GameObject` · `Transform` · `Component` · `MonoBehaviour` |
| **렌더링** | `Shader` · `Pass` · `Technique` (FX11) · `Material` · `Texture` · `Mesh` · `MeshRenderer` |
| **버퍼** | `VertexBuffer` · `IndexBuffer` · `ConstantBuffer` · `StructuredBuffer` · `RawBuffer` · `TextureBuffer` |
| **인스턴싱** | `InstancingManager` · `InstancingBuffer` |
| **지오메트리** | `Geometry` · `GeometryHelper` · `VertexData` · `Primitive3D` |
| **충돌** | `BaseCollider` · `AABBBoxCollider` · `OBBBoxCollider` · `SphereCollider` |
| **모델 · 애니메이션** | `Model` · `ModelMesh` · `ModelAnimation` · `ModelRenderer` · `ModelAnimator` |
| **연출** | `Light` · `Terrain` · `Billboard` · `SnowBillboard` · `Button` |
| **리소스 · 유틸** | `ResourceManager` · `ResourceBase` · `FileUtils` · `MathUtils` · `SimpleMath` · `tinyxml2` |
| **툴 연동** | `ImGUIManager` (Dear ImGui) |

`Component` 를 상속한 렌더러 컴포넌트의 `Update()` 안에서 상수 버퍼를 채우고 드로우콜까지 수행하는 구조입니다.
Unity의 컴포넌트 모델을 참고한 형태로, `GameObject::Update()` → 각 컴포넌트 `Update()` 순으로 흐릅니다.

### AssimpTool

FBX 등 외부 모델을 런타임에 파싱하지 않고 **자체 바이너리 포맷으로 미리 변환**하는 오프라인 툴입니다.
`Converter` 가 Assimp로 읽어 메시(`.mesh`) · 재질 · 애니메이션(`.clip`) 으로 나눠 저장합니다.

---

## 🖼 실행 화면

> 모두 이 저장소를 빌드해 **직접 실행하고 녹화·캡처한 화면**입니다. (Debug\|x64)

### SceneDemo — 모델 인스턴싱 (5단계)

![SceneDemo](docs/images/SceneDemo.gif)

같은 메시를 가진 구조물·유닛을 `InstancingManager` 가 묶어 한 번에 그립니다.
`InstancingManager::Render()` → 각 렌더러의 `RenderInstancing()` 경로입니다.

### SnowDemo — 빌보드 파티클 (7단계)

![SnowDemo](docs/images/SnowDemo.gif)

낙하 위치를 CPU가 아니라 **셰이더에서 시간 기반으로 계산**합니다.
`SnowBillboard::Update()` 가 경과 시간을 상수 버퍼로 넘기면 정점 셰이더가 위치를 만듭니다.

### 그 외

| `OrthographicDemo` (7단계) | `CollisionDemo` (6단계) | `Chapter2` (1단계) |
|---|---|---|
| ![Orthographic](docs/images/orthographic.png) | ![Collision](docs/images/collision.png) | ![Sprite](docs/images/chapter2-sprite-animation.png) |
| 원근(구체)과 직교(UI 쿼드)를 한 화면에 | 콜라이더를 붙인 오브젝트를 씬에 배치 | 키프레임 4장을 0.1초 간격으로 순환 |

`ButtonDemo` · `ViewportDemo` · `TextureBufferDemo` 캡처는 [`docs/images/`](docs/images) 에 있습니다.

---

## 🔧 빌드 & 실행

### 요구 사항

| 항목 | 버전 |
|---|---|
| Visual Studio | 2026 (MSVC **v145**) |
| Windows SDK | 10.0 (설치본 자동 선택) |
| 구성 | **Debug \| x64** — 동봉 라이브러리가 x64 전용이라 Win32는 링크되지 않습니다 |

### 절차

1. `DirectX/DirectX_Client/GameCoding.sln` 을 엽니다.
2. 솔루션 탐색기에서 **`Client` 우클릭 → 시작 프로젝트로 설정**
   `Engine` 은 정적 라이브러리라 F5로 실행할 수 없습니다.
3. 구성이 `Debug | x64` 인지 확인하고 F5.

실행할 데모는 [`Client/Main.cpp`](DirectX/DirectX_Client/Client/Main.cpp) 에서 바꿉니다.

```cpp
desc.app = make_shared<BillBoardDemo>();   // ← 이 줄의 데모 클래스를 교체
```

씬(`CUR_SCENE`) 기반으로 작성되어 실행 화면이 나오는 데모는 다음 8개입니다.
나머지는 학습 중 구조를 씬 기반으로 바꾸면서 이전 `RENDER->` 경로가 주석 처리된 상태입니다.

```
BillBoardDemo   ButtonDemo   CollisionDemo      OrthographicDemo
SceneDemo       SnowDemo     TextureBufferDemo  ViewportDemo
```

리소스는 `..\Shaders\`, `..\Resources\` 같은 **상위 상대 경로**로 참조하므로,
작업 디렉터리가 `Client\` 또는 `Binaries\` 여야 합니다. (VS 기본값인 `$(ProjectDir)` 로 동작)

`DirectX/DirectX_InflearnCode/DirectX_InflearnCode.sln` 도 같은 방식으로 빌드됩니다.
단 Chapter2는 리소스를 파일명만으로 읽어 **작업 디렉터리가 프로젝트 폴더여야** 합니다(VS에서 F5로 실행).

---

## 🔗 링크

- **학습 정리 블로그 (32편)**: <https://unialgames.tistory.com/category/DirectX>
- **재빌드 과정의 문제 진단 기록**: [`docs/troubleshooting/`](docs/troubleshooting/)
- **포트폴리오 수록용 문서**: [`docs/포트폴리오-DirectX11-학습프로젝트.md`](docs/포트폴리오-DirectX11-학습프로젝트.md)

---

<sub>학습 출처: 인프런 DirectX 11 강의. 엔진 코드는 강의를 따라가며 작성한 학습용 코드이며, 독자적인 시스템 설계 기여는 포함되어 있지 않습니다.</sub>
