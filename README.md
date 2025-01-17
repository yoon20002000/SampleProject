Code convention
- Base : https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine
- 개인 규칙
  - 멤버 함수, 변수 작성 순서 : 멤버 함수 우선 작성
  - static, const는 일반 멤버 변수와 함수보더 앞에 온다.
  - Access 별 작성 순서 : public, protect, private
  - 같은 Access 내에서 엔진 Func들의 호출 Flow가 있을 경우 해당 Flow에 맞게 순서를 지킨다.
  - if를 사용 시 무조건 scope를 생성한다.
  - if state 확인 시 true, nullptr 등 상세 히 조건을 적는다.

Player
- 이동(WASD)
![Example Image](./ReadMeResource/WASD.gif)
- 점프(Space)
![Example Image](./ReadMeResource/Jump.gif)
- 공격(ML)
![Example Image](./ReadMeResource/Shot.gif)
- 특수 공격(MR)
- 특수 공격 사용시 이동 불가 및 Cost로 HP 15 사용
![Example Image](./ReadMeResource/SPShot.gif)

- 상호 작용(F)
- 상호 작용을 통해 Field에 배치 된 Interaction Actor와 상호작용
![Example Image](./ReadMeResource/Heal.gif)


Interaction Actor
- 상호작용하여 상호작용 대상에게 적용할 GE 들을 보유
- 정상적으로 상호작용 Activate 성공 시 보유 중인 GE를 이용해 적용
![Example Image](./ReadMeResource/Heal.gif)

AISpawner
- AICharacter를 특정 주기에서 Field에 특정 수치만큼 계속 Spawn 하게 함.
![Example Image](./ReadMeResource/Spawn.gif)

영상 링크 : https://youtu.be/CozS-0GnoGw
![Video](./ReadMeResource/2025-01-17%2011%2002%2056.mp4)
