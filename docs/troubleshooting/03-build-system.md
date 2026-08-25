# 3년 전 코드를 새 툴체인에서 되살리기 — 빌드 시스템도 코드다

> **한 줄 요약** — VS2022(v143) 기준 코드를 VS2026(v145)에서 다시 빌드하며, 툴셋 이전뿐 아니라 **병렬 빌드에서만 터지는 링크 경합**과 **클린 클론에서만 멈추는 빌드**를 찾아 고쳤다. 둘 다 "내 컴퓨터에서는 되던" 문제였다.

---

## 1. 시작한 배경

개발 환경이 VS2022에서 VS2026으로 바뀌면서 솔루션이 열리지 않았다.

```
error MSB8020: Visual Studio 2022에 대한 빌드 도구(플랫폼 도구 집합 = 'v143')를 찾을 수 없습니다.
```

시스템에 설치된 MSVC 툴셋을 확인했다.

```
/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231
```

`14.51`(v145) 하나뿐이었다. v143은 VS2022용이라 별도 구성 요소로 설치해야 한다. 설치를 시도했으나 실패했고, 로그가 원인을 정확히 알려줬다.

```
vs.willow.iselevated : False
Commands with --quiet or --passive should be run elevated from the beginning.
Exit Code: 5007
```

`--passive` 는 비대화형 모드라 UAC 대화상자를 띄울 수 없다. 계정이 관리자여도 프로세스가 승격되지 않았으면 그대로 종료된다.

## 2. 설계

두 갈래가 있었다.

- **v143 설치** — 프로젝트 원본 설정 그대로. 동봉된 서드파티 라이브러리가 전부 v143 산출물이라 링크 호환성 문제가 없다.
- **v145 리타겟** — 다운로드 불필요. 다만 v143으로 빌드된 `Effects11.lib` · `DirectXTex.lib` 정적 라이브러리와의 링크가 미검증.

**결정 전에 v145로 전체 빌드를 실제로 돌려 링크까지 확인**하기로 했다. 되는지 안 되는지 모르는 상태로 고르는 것보다, 측정하고 고르는 편이 낫다.

## 3. 작업 내용

### 툴셋 이전

`-p:PlatformToolset=v145` 를 명령줄로만 넘겨 프로젝트 파일을 건드리지 않고 시험 빌드했다. 결과는 **전체 성공**이었다.

```
Engine.vcxproj     -> Libraries\Lib\Engine\Engine.lib
AssimpTool.vcxproj -> Binaries\AssimpTool.exe
Client.vcxproj     -> Binaries\Client.exe
오류 0
```

v143으로 빌드된 서드파티 정적 라이브러리와도 문제없이 링크됐다(`LNK4099` PDB 경고만 발생). 근거가 확보됐으므로 v145 리타겟으로 결정하고 vcxproj 5개를 수정했다. Chapter1은 구성 하나만 `v142` 로 남아 있어 함께 정리했다.

이 과정에서 **툴셋과 무관한 기존 버그 두 개가 함께 드러났다.**

| 오류 | 원인 |
|---|---|
| `C2039: 'Add': 'SnowBillboard'의 멤버가 아닙니다` | `.cpp` 에 정의만 있고 헤더 선언 누락. 커밋된 헤더에도 없었다 |
| `LNK2019` 5건 | Chapter2 `Animation`·`Animator` 의 선언만 있고 구현 없음 |

이전 빌드 산출물이 저장소에 커밋되어 있어 그동안 드러나지 않던 것들이었다. **즉 커밋된 `.lib`·`.exe` 는 현재 소스와 일치하지 않는 상태였다.**

### 병렬 빌드에서만 터지는 링크 경합

전체 빌드를 병렬(`-m`)로 돌리자 간헐적으로 실패했다.

```
LINK : fatal error LNK1104: 'Libraries\Lib\Engine\Engine.lib' 파일을 열 수 없습니다. [Engine.vcxproj]
  Client.vcxproj -> Binaries\Client.exe
```

Client가 `Engine.lib` 을 읽는 동안 Engine이 같은 파일을 쓰려다 실패했다. 의존성 선언을 확인했다.

```
GameCoding.sln 의 ProjectDependencies       : 0
Client.vcxproj / AssimpTool.vcxproj 의 ProjectReference : 0

Client/pch.h:3      #pragma comment(lib, "Engine/Engine.lib")
AssimpTool/pch.h:4  #pragma comment(lib, "Engine/Engine.lib")
```

**`#pragma comment(lib, ...)` 는 링커에게만 말하고 빌드 시스템에는 아무것도 알리지 않는다.** MSBuild 입장에서 Client와 Engine은 완전히 독립된 프로젝트이므로 동시에 빌드해도 된다고 판단한다. 직렬(`-m:1`)로 돌리면 성공하고 병렬에서만 터지는 이유가 이것이다. **VS의 기본 빌드도 병렬이므로 IDE에서도 간헐적으로 재현된다.**

### 클린 클론에서만 멈추는 빌드

별개로, 저장소에 빌드 산출물 414개가 추적되고 있었다. 무시 규칙 파일명이 `C++.gitignore` 여서 git이 읽지 않았기 때문이다.

```
git check-ignore -v ".../DirectX_InflearnCode.pch"
→ 매치 없음 (무시 규칙 미적용)
```

`.gitignore` 를 신설하고 산출물을 인덱스에서 제거한 뒤, **"새로 클론한 상태"를 실제로 재현**했다. 추적 해제한 폴더를 지우고 빌드했더니 **10분이 지나도 끝나지 않았다.** 로그를 보니 멈춘 게 아니라 입력을 기다리고 있었다.

```
...\Libraries\Include\Engine은(는) 대상은 파일 이름입니까
아니면 디렉터리 이름입니까? (F = 파일, D = 디렉터리)
```

Engine의 PreBuildEvent가 헤더를 복사하는데, **대상 폴더가 없으면 `xcopy` 가 F/D를 되묻는다.** 그 폴더가 그동안 커밋되어 있었기 때문에 아무도 이 문제를 만나지 않았던 것이다.

## 4. 고민한 부분과 선택

### 빌드 순서를 어떻게 보장할 것인가

| 대안 | 트레이드오프 |
|---|---|
| **A. 솔루션에 `ProjectDependencies` 추가** | `.sln` 만 고치면 된다. 하지만 솔루션 빌드에만 적용되고, 프로젝트를 단독으로 빌드하면 무효 |
| **B. `ProjectReference` 추가 (링크까지 위임)** | 표준적. 다만 `#pragma comment(lib)` 와 중복으로 링크 입력이 들어간다 |
| **C. `ProjectReference` + 링크 위임 끄기** | 순서만 보장하고 링크는 기존 방식 유지 |

**C를 선택했다.**

```xml
<ProjectReference Include="..\Engine\Engine.vcxproj">
  <Project>{4f5c1012-31bd-4947-831e-bf2cc095398f}</Project>
  <ReferenceOutputAssembly>false</ReferenceOutputAssembly>
  <LinkLibraryDependencies>false</LinkLibraryDependencies>
</ProjectReference>
```

### 산출물 추적을 어디까지 해제할 것인가

전부 지우면 간단하지만, `Binaries/` 안의 `assimp-vc143-mtd.dll` 은 **실행에 필요한 런타임 DLL인데 그 폴더에만 있고 복사해 주는 빌드 단계가 없었다.** 통째로 무시하면 클론한 사람이 실행할 수 없다.

```gitignore
**/Binaries/*
!**/Binaries/*.dll
```

## 5. 선택 이유

- **C를 고른 이유**: 목적은 "빌드 순서 보장" 하나였다. B는 링크 동작까지 바꾸므로 문제 범위를 넘어선다. 기존 링크 방식을 그대로 둔 채 필요한 것만 얻는 편이 되돌리기도 쉽다. A는 프로젝트를 단독 빌드할 때 무효가 되는데, 실제로 이번 작업 중 `Client.vcxproj` 를 단독 지정해 빌드한 적이 있어 그 한계를 직접 겪었다.
- **DLL 예외를 둔 이유**: 산출물 정리의 목적은 저장소를 가볍게 하는 것이지, **클론한 사람이 실행하지 못하게 만드는 게 아니다.** 원래는 DLL을 `Libraries/` 로 옮기고 post-build 복사를 넣는 것이 옳지만, 그건 빌드 구조 변경이라 이번 범위를 넘는다고 판단해 예외 처리로 남기고 기록했다.
- **`xcopy` 를 `/I` 만 붙이지 않고 `mkdir` 을 함께 넣은 이유**: `/I` 는 "여러 파일을 복사할 때" 대상을 디렉터리로 간주한다. `*.inl` 복사는 대상 파일이 하나뿐이라 조건에 걸리지 않을 수 있다. 확실하게 하려면 폴더 존재를 먼저 보장해야 한다.

## 6. 결과

```
# 병렬 리빌드 (수정 전 간헐 실패 → 수정 후)
종료 코드 0 · 오류 0 · LNK1104 발생 0회
  Engine.vcxproj -> Engine.lib / AssimpTool.exe / Client.exe

# 클린 클론 재현 (생성물 폴더 삭제 후 리빌드)
종료 코드 0 · 오류 0
Libraries/Include/Engine 68개 파일 재생성

# 저장소
추적 파일 1165 → 751개 (산출물 414개 해제)
```

프로젝트 설정이 실제로 반영됐는지도 MSBuild에 직접 질의해 확인했다.

```json
{ "PlatformToolset": "v145", "VCToolsVersion": "14.51.36231" }
{ "ProjectReference": [{ "Identity": "..\\Engine\\Engine.vcxproj",
    "LinkLibraryDependencies": "false", "ReferenceOutputAssembly": "false" }] }
```

## 남은 것

- **`.git` 히스토리에는 과거 산출물이 그대로 남아 약 236MB다.** 추적 해제는 히스토리를 줄이지 않는다. 클론 용량을 줄이려면 히스토리 재작성이 필요하고, 이미 push된 커밋을 갈아엎는 작업이라 별도 판단이 필요하다.
- **Release 구성은 검증하지 않았다.** Debug|x64 만 빌드·실행을 확인했다.
