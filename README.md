정리 Slide : https://docs.google.com/presentation/d/1OZ-jnF9TqLvEYXjug0RYpU7S4z75fitwBm7maFgsNM0/edit?usp=drive_link

Code convention
- Base : https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine
- 개인 규칙
  - 멤버 함수, 변수 작성 순서 : 멤버 함수 우선 작성
  - static, const는 일반 멤버 변수와 함수보더 앞에 온다.
  - Access 별 작성 순서 : public, protect, private
  - 같은 Access 내에서 엔진 Func들의 호출 Flow가 있을 경우 해당 Flow에 맞게 순서를 지킨다.
  - if를 사용 시 무조건 scope를 생성한다.
  - if state 확인 시 true, nullptr 등 상세 히 조건을 적는다.

작업 기간 : 24.12 ~  
작업 인원 : 1인  
작업 환경 : UE 5.5, Rider, Git  
사용 플러그인 : GAS  

개발 정리 블로그 : https://blog.naver.com/yoon20002000/223755810465
