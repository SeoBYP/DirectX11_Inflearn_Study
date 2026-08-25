# DirectX 11 Study — 렌더링 파이프라인부터 엔진 구조까지 직접 따라 만들기

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white)
![DirectX 11](https://img.shields.io/badge/DirectX-11-107C10?logo=windows&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2026%20(v145)-5C2D91?logo=visualstudio&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-0078D6?logo=windows&logoColor=white)
![Type](https://img.shields.io/badge/Type-학습%20프로젝트-lightgrey)

> **인프런 DirectX 11 강의를 따라가며 만든 학습용 엔진입니다.**
> 코드 자체는 강의를 따라간 것이지만, **이 저장소를 3년 뒤 새 툴체인에서 다시 굴러가게 만드는 과정**에서
> 툴셋 이전 · 빌드 순서 경합 · 렌더링 아티팩트 원인 규명 같은 작업을 직접 했습니다.
> 그 진단 기록이 [기술 사례](docs/troubleshooting/)에 정리되어 있습니다.

![SceneDemo](docs/images/SceneDemo.gif)

**[🧩 기술 사례 — 문제 진단 기록](docs/troubleshooting/)** · **[📝 학습 정리 블로그 32편](https://unialgames.tistory.com/category/DirectX)** · [🧱 엔진 구조](#-엔진-구조) · [🔧 빌드](#-빌드--실행)

---

## 📖 이 저장소가 증명하는 것

Unity·Unreal 같은 상용 엔진이 **내부에서 무엇을 하고 있는지**를 직접 만들어 보며 확인한 기록입니다.
정점 버퍼를 GPU에 올리고, 상수 버퍼로 행렬을 넘기고, 셰이더 패스를 골라 드로우콜을 부르는 과정을
엔진이 감춰 주지 않는 레벨에서 다뤘습니다.

- **렌더 파이프라인**: 정점/인덱스 버퍼 → 입력 레이아웃 → 상수 버퍼 → 셰이더 패스 → 드로우콜
- **엔진 아키텍처**: GameObject / Component 구조, 씬 그래프, 리소스 매니저, 렌더 매니저
- **조명·재질**: Ambient / Diffuse / Specular / Emissive, 노멀 매핑
- **모델·애니메이션**: Assimp 모델 임포트, 스키닝 애니메이션, 트위닝
- **최적화 기법**: 인스턴싱과 드로우콜, StructuredBuffer / RawBuffer / TextureBuffer
- **공간·충돌**: AABB · OBB · Sphere, Point Test / Intersection / Raycast

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
`DirectX_InflearnCode` 에서 컴포넌트 구조와 매니저를 먼저 익히고,
`DirectX_Client` 에서 그 위에 렌더링 기법과 모델·애니메이션까지 확장했습니다.

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

### SceneDemo — 모델 인스턴싱

![SceneDemo](docs/images/SceneDemo.gif)

같은 메시를 가진 구조물·유닛을 `InstancingManager` 가 묶어 한 번에 그립니다.
`InstancingManager::Render()` → 각 렌더러의 `RenderInstancing()` 경로입니다.

### SnowDemo — 빌보드 파티클

![SnowDemo](docs/images/SnowDemo.gif)

낙하 위치를 CPU가 아니라 **셰이더에서 시간 기반으로 계산**합니다.
`SnowBillboard::Update()` 가 경과 시간을 상수 버퍼로 넘기면 정점 셰이더가 위치를 만듭니다.

### 그 외

| `OrthographicDemo` | `CollisionDemo` | `Chapter2` |
|---|---|---|
| ![Orthographic](docs/images/orthographic.png) | ![Collision](docs/images/collision.png) | ![Sprite](docs/images/chapter2-sprite-animation.png) |
| 원근(구체)과 직교(UI 쿼드)를 한 화면에 | 콜라이더를 붙인 오브젝트를 씬에 배치 | 키프레임 4장을 0.1초 간격으로 순환 |

`ButtonDemo` · `ViewportDemo` · `TextureBufferDemo` 캡처는 [`docs/images/`](docs/images) 에 있습니다.

---

## 🧩 기술적 도전과 해결

강의를 따라간 코드 자체보다, **그 코드를 다시 굴러가게 만드는 과정**에서 한 일이 실무와 가깝습니다.
각 항목은 배경 → 설계 → 작업 → 고민과 선택 → 선택 이유 → 측정 결과 순으로 정리했습니다.

### 애니메이션이 멈춰 있던 원인이 애니메이션 코드가 아니었던 건

스프라이트가 정지해 있어 방금 구현한 `Animator` 를 의심했지만, 호출 경로를 위에서부터 대조하니 진입점이 블로킹 `GetMessage` 루프였다. 코드를 고치기 전에 **창에 메시지를 주입해 화면이 살아나는 것을 먼저 확인**해 원인을 확정했다. 서로 다른 프레임 **1/8 → 4/8**(키프레임 수와 일치), 변경량은 2줄.
→ [상세: 진단 과정과 대안 비교](docs/troubleshooting/01-blocking-message-loop.md)

### 빌보드 풀숲의 앞뒤가 뒤집혀 보이는 문제

"블렌드 상태 누수"와 "깊이 테스트 미동작"이라는 그럴듯한 가설을 세웠지만, 픽셀 diff로 **둘 다 기각**했다(각각 **0.00%**, **8.29%** 변화). 추측을 멈추고 깊이값을 색으로 출력한 결과, **무더기 하나가 깊이 하나를 공유**하는 빌보드 기법 자체의 한계임이 드러났다. 코드만 읽고 판단했다면 틀린 수정을 넣었을 것이다.
→ [상세: 가설 3개를 측정으로 기각한 과정](docs/troubleshooting/02-billboard-depth-artifact.md)

### 3년 전 코드를 새 툴체인에서 되살리기

VS2022(v143) → VS2026(v145) 이전 과정에서, **병렬 빌드에서만 터지는 `LNK1104`** 와 **클린 클론에서만 무한 대기하는 빌드**를 찾아 고쳤다. 전자는 `#pragma comment(lib)` 가 링커에만 말하고 빌드 시스템에는 아무것도 알리지 않아서, 후자는 `xcopy` 가 대상 폴더 부재 시 되묻기 때문이었다. 둘 다 산출물이 커밋되어 있던 탓에 그동안 드러나지 않았다.
→ [상세: 툴셋 이전 · 빌드 순서 · 클린 클론 재현](docs/troubleshooting/03-build-system.md)

---

## 💡 강의 코드와 실제 엔진 작업의 차이

위 세 건을 하며 알게 된 것들입니다. **강의를 따라갈 때는 한 번도 마주치지 않았던 종류의 문제**였습니다.

**증상과 원인은 대개 다른 계층에 있다.**
"애니메이션이 안 움직인다"의 원인은 애니메이션 코드가 아니라 `WinMain` 의 메시지 루프였다. 증상이 보이는 곳부터 파고들면 엉뚱한 데서 시간을 쓴다. 호출 경로를 위에서부터 훑어 "여기까지는 살아 있다"를 하나씩 확정하는 편이 빨랐다.

**가설은 측정으로 죽여야 한다.**
빌보드 문제에서 그럴듯한 가설 세 개를 세웠고 셋 다 수치로 기각했다. 특히 첫 가설(블렌드 상태 누수)은 코드 근거까지 있어 확신했지만 픽셀 차이가 0.00%였다. 그대로 밀어붙였다면 문제는 남고 코드만 늘었을 것이다.

**렌더링 버그는 눈으로 판단하면 안 된다.**
"뒤 풀이 앞에 보인다"는 인상만으로는 정렬 문제인지 깊이 문제인지 구분되지 않았다. 깊이값을 색으로 출력하고 나서야 무더기가 단색으로 찍히는 것이 보였고, 그때 원인이 확정됐다. 셰이더는 런타임 컴파일이라 리빌드 없이 실험할 수 있다는 점도 그때 알았다.

**빌드 시스템도 코드다.**
`#pragma comment(lib, ...)` 는 링커에게만 말한다. MSBuild는 그 의존성을 모르므로 병렬로 빌드해도 된다고 판단하고, 그래서 **직렬에서는 성공하고 병렬에서만 터지는** 버그가 나온다. 빌드 순서는 선언해야 생긴다.

**"내 컴퓨터에서는 됩니다"는 재현해야 없앨 수 있다.**
산출물이 커밋되어 있는 동안은 아무 문제가 없었다. 추적을 해제하고 **폴더를 실제로 지운 뒤 빌드**하자마자 `xcopy` 가 입력을 기다리며 멈췄다. 클린 상태를 만들어 보지 않으면 영영 안 보이는 종류의 버그다.

**작은 수정이 정답인 경우가 많다.**
세 건의 최종 변경량은 각각 2줄, 0줄(원인 규명만), 프로젝트 설정 몇 줄이었다. 진단에 쓴 시간이 수정에 쓴 시간보다 훨씬 길었고, 그게 정상이었다.

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
desc.app = make_shared<SnowDemo>();   // ← 이 줄의 데모 클래스를 교체
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

## 📚 학습 기록 — 블로그 연재 32편

구현하면서 이해한 내용을 [블로그](https://unialgames.tistory.com/category/DirectX)에 정리했습니다.

| 주제 | 다룬 내용 |
|---|---|
| **엔진 구조** | GameObject·Transform / Component·MonoBehaviour / SceneManager·MeshRenderer / ResourceManager / RenderManager / Animation System |
| **렌더링 기초** | 3D Mesh 렌더링 / Sampler / Normal / Depth Stencil / HeightMap 지형 생성 |
| **조명 · 재질** | Ambient · Diffuse · Specular · Emissive / Normal Mapping |
| **모델 · 애니메이션** | Model Import / Animation / Animation Tweening |
| **최적화 · 버퍼** | 인스턴싱과 드로우콜 / Mesh·Model·Animation Instancing / RawBuffer / TextureBuffer / StructuredBuffer |
| **공간 · 충돌** | AABB·OBB Collision / Sphere·AABB·OBB 도형 / Point Test / Intersection / Raycast |
| **기타** | 스카이박스와 스카이돔 / Viewport / 직교투영과 UI / 빌보드와 파티클 |

---

## 🔗 링크

- **기술 사례 — 문제 진단 기록**: [`docs/troubleshooting/`](docs/troubleshooting/)
- **학습 정리 블로그**: <https://unialgames.tistory.com/category/DirectX>

---

<sub>학습 출처: 인프런 DirectX 11 강의. 엔진 코드는 강의를 따라가며 작성한 것이며, 위 「기술적 도전과 해결」은 이 저장소를 VS2026 환경에서 다시 빌드·실행 가능하게 만드는 과정에서 직접 진단하고 수정한 내용입니다.</sub>
