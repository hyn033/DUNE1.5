# 20242446 김효은

<p>- 샌드웜의 [ 가까운 유닛 탐색 ] 구현</p>
  <p>1. for 문을 이용하여 초기 목적지를 정함</p>
     <p>( for 문을 돌렸을 때 가장 마지막으로 찾은 유닛의 위치가 초기 목적지이다. )</p>
  <p>2. for 문을 하나 더 만들어 새 목적지를 찾도록 함</p>
     <p>( 새 목적지는 초기 목적지와 비교하기 위해 유닛의 위치를 임시 저장해놓는 목적지이다. )</p>
  <p>3. 최종 목적지 설정</p>
     <p>- 샌드웜의 현 위치의 열에서 새 목적지의 열을 뺌 -> 정수로 변환</p>
     <p>- 샌드웜의 현 위치의 열에서 초기 목적지의 열을 뺌 -> 정수로 변환</p>
     <p>- 두 정수를 비교하여 작은 정수에 해당하는 목적지를 초기 목적지에 할당</p>
       <p>ex) 새 목적지의 정수가 작으면, 초기 목적지에 새 목적지를 할당</p>
           <p>초기 목적지의 정수가 작으면, 초기 목적지 값을 유지</p>
     <p>- 모든 유닛의 위치와 비교를 끝낸 시점에 초기 목적지 값으로 최종 목적지 설정</p>
