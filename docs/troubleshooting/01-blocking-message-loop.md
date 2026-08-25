# 애니메이션이 멈춰 있던 원인이 애니메이션 코드가 아니었던 건

> **한 줄 요약** — 스프라이트 애니메이션이 정지해 있어 `Animator` 를 의심했지만, 실제 원인은 진입점의 블로킹 `GetMessage` 루프였다. 메시지를 인위적으로 주입해 화면이 살아나는 것을 확인하고 원인을 확정한 뒤, 논블로킹 루프로 교체했다.

---

## 1. 시작한 배경

VS2026 대응 작업 중 `Chapter2` 의 `Animation` · `Animator` 에 선언만 있고 구현이 없는 함수 5개를 채웠다(`LNK2019`). 빌드는 통과했고 실행 파일도 만들어졌다.

그런데 실행해 보니 **창은 뜨는데 뱀 스프라이트가 움직이지 않았다.** 내가 방금 채운 코드가 잘못됐을 가능성이 가장 커 보였다.

먼저 "정말 안 움직이는가"를 눈이 아니라 수치로 확인했다. 창 클라이언트 영역을 120ms 간격으로 8프레임 캡처해 각 프레임의 픽셀 해시를 비교했다.

```
FRAME 1~8 hash = 6CE4B71C24F95EDFCB2097F523E33021   (8개 전부 동일)
```

`SnakeAnim` 은 키프레임 4장 × 0.1초 = **0.4초 주기**다. 약 1초를 캡처했으니 2.5회 순환했어야 하는데 화면이 한 번도 바뀌지 않았다.

## 2. 설계

"내가 채운 함수가 틀렸다"가 아니라 **"`Animator::Update()` 가 호출되기는 하는가"** 부터 확인하기로 했다. 호출 자체가 안 되고 있다면 함수 내용은 볼 필요가 없다.

호출 경로를 위에서 아래로 따라가며 각 단계가 살아 있는지 확인하는 순서로 잡았다.

```
wWinMain 메시지 루프
  └ Game::Update()
      └ TIME->Update()      · 델타타임이 갱신되는가
      └ SCENE->Update()
          └ Scene::Update()        · 게임오브젝트를 도는가
              └ GameObject::Update()   · 컴포넌트를 도는가
                  └ Animator::Update()     · 여기까지 오는가
```

## 3. 작업 내용

경로를 하나씩 대조했다.

| 단계 | 확인 결과 |
|---|---|
| `TimeManager::Update()` | `QueryPerformanceCounter` 로 매 프레임 델타 계산 — 정상 |
| `Scene::Update()` | `_gameObjects` 전체를 돌며 `Update()` 호출 — 정상 |
| `GameObject::Update()` | `_components` 전체를 돌며 `component->Update()` 호출 — 정상 |
| `AddComponent` | `ComponentType::Animator` = 3, `FIXED_COMPONENT_COUNT` = 4 → `_components[3]` 에 정상 등록 |
| `Animator::Update()` | `Component::Update()` 를 오버라이드 — 심볼 이름 `?Update@Animator@@UEAAXXZ` 의 `U` 가 virtual 을 의미 |

전부 정상이었다. 남은 건 맨 위, 진입점이었다.

```cpp
// DirectX_InflearnCode_Chapter2.cpp
while (GetMessage(&msg, nullptr, 0, 0))     // ← 큐가 비면 여기서 스레드가 잠든다
{
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    else
    {
        G_Game->Update();
        G_Game->Render();
    }
}
```

`GetMessage` 는 **메시지가 올 때까지 블로킹**한다. 메시지가 하나 도착하면 `GetMessage` 가 그것을 꺼내고, 이어지는 `PeekMessage` 는 빈 큐를 보고 `else` 로 빠져 **딱 한 프레임만** 돌린 뒤 다시 잠든다. 입력이 없으면 게임이 멈춘다.

가설을 세웠으면 검증해야 한다. 코드를 고치기 전에, **창에 `WM_NULL` 을 계속 주입하면서** 같은 방식으로 캡처했다. 가설이 맞다면 메시지가 들어오는 동안만 게임이 돌아야 한다.

```
FRAME 1 hash=A341DE1E...
FRAME 2 hash=8ADDFCC2...
FRAME 3 hash=731797F0...    (4~6 동일)
FRAME 7 hash=A341DE1E...    ← 프레임 1과 같은 해시 = 한 바퀴 순환
FRAME 8 hash=BB8EE1B2...
DISTINCT_FRAMES = 4 / 8
```

**서로 다른 프레임이 정확히 4개**, 키프레임 수와 일치했다. 프레임 7이 프레임 1의 해시로 돌아온 것은 루프가 한 주기를 완주했다는 뜻이다. 가설이 확정됐고, 동시에 **내가 채운 5개 함수가 정상 동작한다는 것도 함께 증명**됐다.

## 4. 고민한 부분과 선택

게임 루프를 어떻게 돌릴지에 세 가지 선택지가 있었다.

| 대안 | 내용 | 트레이드오프 |
|---|---|---|
| **A. `WM_TIMER` 로 구동** | `GetMessage` 를 유지하고 타이머 메시지로 프레임을 돌린다 | 유휴 시 CPU를 안 쓴다. 하지만 타이머 해상도(기본 ~15.6ms)에 프레임률이 묶이고, 게임 루프를 메시지 큐에 얹는 비표준 구조가 된다 |
| **B. `PeekMessage` 논블로킹 루프** | `while (msg.message != WM_QUIT)` 안에서 메시지가 없으면 곧바로 `Update`/`Render` | 게임 루프의 표준형. 대신 유휴 상태에서도 코어 하나를 계속 점유한다 |
| **C. 렌더 스레드 분리** | 메시지 펌프와 렌더 루프를 별도 스레드로 | 구조적으로 가장 깔끔하나 동기화 비용과 복잡도가 크다 |

**B를 선택했다.**

```cpp
MSG msg = {};
while (msg.message != WM_QUIT)
{
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    else { G_Game->Update(); G_Game->Render(); }
}
```

## 5. 선택 이유

- **같은 저장소의 `Chapter1` 이 이미 B를 쓰고 있었다.** 즉 이 루프는 Chapter2로 넘어오면서 Win32 프로젝트 템플릿의 기본 `GetMessage` 루프로 **되돌아간 것**이지, 의도한 설계가 아니었다. 원래 형태로 복구하는 것이 맞다.
- A는 프레임률이 타이머 해상도에 묶인다. 이 프로젝트는 `desc.vsync = false` 로 프레임 상한 자체를 두지 않는 구조라 방향이 어긋난다.
- C는 이 규모(스프라이트 2개짜리 학습 프로젝트)에 과하다. 얻는 것보다 잃는 복잡도가 크다.
- B의 단점인 CPU 점유는 이미 `Chapter1`·`Client` 가 동일하게 감수하고 있는 조건이라, 여기만 다르게 갈 이유가 없었다.

`MSG msg;` 를 `MSG msg = {};` 로 바꾼 것도 함께 필요했다. 초기화하지 않으면 `msg.message` 가 미초기화 값이라 루프 진입 자체가 불안정하다.

## 6. 결과

같은 조건(메시지 주입 **없이**, 120ms × 8프레임)으로 재측정했다.

| | 수정 전 | 수정 후 |
|---|---|---|
| 서로 다른 프레임 | **1 / 8** | **4 / 8** |
| 화면 | 완전 정지 | 키프레임 4장 순환 |

빌드도 함께 확인했다.

```
DirectX_InflearnCode.sln  리빌드  종료 코드 0, 오류 0
  DirectX_InflearnCode.vcxproj          -> DirectX_InflearnCode_Chapter1.exe
  DirectX_InflearnCode_Chapter2.vcxproj -> DirectX_InflearnCode_Chapter2.exe
```

**변경량은 2줄이었다.** 처음 의심했던 애니메이션 코드는 한 글자도 건드리지 않았다.

## 남은 것

- 논블로킹 루프라 유휴 시에도 코어 하나를 계속 점유한다. `vsync` 를 켜거나 프레임 상한을 두는 것이 다음 단계다. **CPU 사용률은 측정하지 않았다.**
