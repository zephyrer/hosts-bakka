# 소개 #

Bakka(바까)는 호스트를 변경해서 테스트 할 일이 있을때 이를 자동으로 설정해주는 간단한 유틸리티입니다.

![http://farm3.static.flickr.com/2494/3835450151_decdaf2e73.jpg](http://farm3.static.flickr.com/2494/3835450151_decdaf2e73.jpg)

사용법은 매우 쉽습니다. 원하는 버튼을 살짝 눌러주세요~

자동으로 특정 URL로 요청을 보내고 도메인 정보를 받아와 윈도우 호스트 파일(C:\Windows\System32\drivers\etc\hosts)에 셋팅합니다.

이후 브라우저를 재시작만 해주시면 끝.

(참고: 이곳에서 배포되는 설치 버전은 [Daum 아고라](http://agora.media.daum.net/) 개발에 필요한 설정이 기본으로 셋팅되어 있으며 Daum 사내에서만 이용 가능합니다.)

# 서비스 적용 방법 #

Bakka를 다른 서비스에서도 이용할 수 있도록 변경할 수 있습니다. 아래 절차를 하나씩 차근차근 따라해 보세요~

1. **먼저 호스트 정보를 인터넷으로 서비스할 수 있는 공간이 필요합니다.**

> 호스트 정보를 웹에서 내려줄 수 있도록 구성합니다. 가급적이면 보안상 해당 회사내에서만 접속 가능한 서버가 좋습니다.

> ex) http://www.likejazz.com/hosts.php

2. **Bakka.ini 를 수정합니다.**

> 버튼 이름, 버튼을 눌렀을때 접속하는 서버, 파일명을 기입합니다. 버튼은 총 5개 이며 이 중 맨 마지막 "원래대로" 버튼을 제외한 4개 버튼은 임의로 변경해서 자유로이 이용하실 수 있습니다.

> 버튼 4개를 전부 사용할 필요는 없으며 필요없는 설정은 삭제하면 자동으로 "사용 안함"으로 비활성화 됩니다.

```
Bakka.ini

[button_1]
name=버튼에 표시될 이름
host=www.likejazz.com
file=hosts.php
```

> "버튼에 표시될 이름" 버튼을 누를 경우 http://www.likejazz.com/hosts.php 로 요청을 합니다.

3. **Bakka를 배포합니다.**

> 배포에는 2가지 방법이 있습니다.

  1. NSIS 인스톨러로 묶어서 배포하는 방법
  1. Bakka.exe, Bakka.ini 파일만 압축해서 별도로 배포하는 방법

> 인스톨러로 묶는 것이 가장 편하지만 Bakka는 최소한의 윈도우 기본 라이브러리만 사용하므로 굳이 인스톨러가 필요치는 않습니다.

> 실행파일(Bakka.exe)과 설정파일(Bakka.ini)만 같은 디렉토리에 두면 어디서든 실행이 가능합니다.


# 적용 사례 #

![http://farm4.static.flickr.com/3487/3836415690_5afaa0633e.jpg](http://farm4.static.flickr.com/3487/3836415690_5afaa0633e.jpg)

"비밀 프로젝트 적용"을 누르면 비밀 프로젝트에서 사용 하는 호스트 정보를 받아와 자동으로 셋팅합니다. 물론 "원래대로"를 누를 경우 이 정보가 모두 삭제됩니다.

Bakka.ini 파일만 수정하면 얼마든지 다른 형태로 변경 할 수 있으며 커스터마이징 가능합니다.

적용 사례에서 호스트 정보를 받아오는 페이지는 간단한 PHP로 작성했으며 소스코드는 아래와 같습니다. 다른 호스트 정보들도 이 같은 형태로 만드시면 됩니다. 호스트 파일은 EUC-KR 입니다. UTF-8로 작성한 경우 반드시 EUC-KR로 컨버팅 해주어야 한글이 깨지지 않습니다.

```
<?php
 
$DATA="# 비밀 프로젝트 호스트 설정
 
127.0.0.1               secret-1.daum.net
127.0.0.1               secret-2.daum.net
127.0.0.1               secret-3.daum.net
127.0.0.1               secret-4.daum.net
127.0.0.1               secret-5.daum.net
";

Header("Content-type: text/plain; charset=euc-kr\n");
echo (iconv("UTF-8", "EUC-KR", $DATA));

?>
```

# 기타 #

  * 실행시 항상 트레이(tray)에 프로그램이 상주합니다. 마우스 우측 클릭 Exit를 선택해 종료할 수 있습니다.
  * ESC 키를 누르면 프로그램이 종료됩니다.
  * Bakka.ini 파일은 유니코드로 작성해야 하며 호스트 정보를 내려주는 서비스는 EUC-KR로 서비스 되어야 합니다. 프로그램 전체는 유니코드로 되어 있으나 윈도우 호스트 파일(C:\Windows\System32\drivers\etc\hosts)은 EUC-KR 포맷입니다.
  * **_호스트 설정 변경 후에는 반드시 브라우저를 재시작 해주세요._** DNS 정보가 캐시되어 있기 때문에 바로 적용되지 않습니다. 이는 ipconfig /flushdns 를 해도 마찬가지 입니다.
  * Firefox 브라우저를 사용하는 경우 [ShowIP](https://addons.mozilla.org/en-US/firefox/addon/590) 애드온을 이용하시면 호스트 적용 여부를 간단히 확인할 수 있습니다.