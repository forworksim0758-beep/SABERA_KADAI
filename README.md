# SABERA_KADAI

課題名: しりとりアプリ

## ビルド方法と実行方法

### (1) ファイルの用意

以下のファイルがあることを確認する。

- Makefile
- func_1.c
- func_1.h
- main_1.c
- dict.txt

### (2) 実行ファイル生成

以下をターミナルで実行する。

```
make
```

実行ファイル `siritori` が生成される。

### (3) 実行ファイルの実行

以下をターミナルで実行する。

```
./siritori
```

ゲームが始まる。

## 実装した機能

### 基本機能

- 直前の単語を、表示できるようにする

  画面をクリアする処理はしていないため常に直前は表示されるが、「prev1」と入力しても表示可能である。

- 任意の単語を、入力できるようにする

  SKK(日本語入力システム)用の標準辞書skkdicの記録内、あるいは変更を加えていれば`dict.txt`側に記録されている単語なら入力が可能。
  存在しなかった場合は、`dict.txt`側の不備の場合もあるため猶予の減少やゲーム終了は発生しない。

- 直前の単語の末尾と、入力した単語の先頭を比較して、同じ場合だけ単語を更新する。違う場合はエラーを表示する。

  最後が捨て仮名(ぃ, ぁ, ゃ...)のときは、次回はその文字を並字(い、あ、や...)に変換したものから始まる。
  最後が伸ばし棒のときは、次回は伸ばし棒一個前の文字から始まる。例えば「あいすてぃー」なら「い」から始まる。

  <img width="646" height="346" alt="image" src="https://github.com/user-attachments/assets/3b9af130-a827-4c3a-8296-049ab48b3b8c" />

- 末尾が「ん」で終わる単語が入力されたら、ゲームを終了する

  <img width="651" height="73" alt="image" src="https://github.com/user-attachments/assets/a7f810b0-e93a-4ec5-830a-fbcbdc218672" />

- 過去に使用した単語が入力されたら、ゲームを終了する

  <img width="652" height="299" alt="image" src="https://github.com/user-attachments/assets/50e752da-5b0b-4769-9f96-b4942276a264" />

- ゲーム中や終了後に、最初からやり直せるリセット機能をつける

  プレイ中は「restart」と入力すると難易度選択に戻る。

  <img width="640" height="72" alt="image" src="https://github.com/user-attachments/assets/508cb765-b2bd-43be-b689-e713a079a038" />

  ゲーム終了時は自動的に難易度選択に戻る。

  <img width="632" height="333" alt="image" src="https://github.com/user-attachments/assets/1b182ced-1eb4-40b8-b570-1f7665955e36" />

### 追加機能

- 任意個前に受理された単語を表示する機能

  「prev{Num}」({Num}は数字で、ゲームが始まってから何個目の単語かを指定する)で指定可能。範囲外はエラーとして処理する。

  <img width="627" height="258" alt="image" src="https://github.com/user-attachments/assets/8f0ece9d-2b99-41b1-ac98-d3f5f8a07121" />

- 難易度選択機能

  ゲーム開始時、またリセットをしたときに難易度を選択する。

  <img width="620" height="26" alt="image" src="https://github.com/user-attachments/assets/3af23d82-d877-4ffd-8f1a-86f651875016" />

  - EASYの場合

    ゲーム終了までに5回の猶予を得る。

    <img width="635" height="389" alt="image" src="https://github.com/user-attachments/assets/1ae3ae5f-3d34-44ce-858f-ea3640e02a7e" />

  - NORMALの場合

    基本仕様通りのゲームとなり、猶予は1回のみ。

    <img width="652" height="149" alt="image" src="https://github.com/user-attachments/assets/5151161b-3343-4a2c-bd35-2a78b0076e65" />

  - DIFFICULTの場合

    猶予は1回のみ。かつ前回入力した単語の字数以上でない場合も終了の対象となる。

    <img width="637" height="181" alt="image" src="https://github.com/user-attachments/assets/59ebcb7c-62c8-46df-8db1-1c6dfd5b2156" />

  - QUITの場合

    ゲームを終了する。

- 実在しない単語の拒否機能

  SKK(日本語入力システム)用の標準辞書skkdicを利用して、当該辞書に存在しない単語は使えないようにする。

  <img width="536" height="46" alt="image" src="https://github.com/user-attachments/assets/f4ba64f1-cf47-4b1f-85e2-c1c8f6709aaf" />

  「あいあんさいど」を入力すると以下のように継続可能。

  <img width="634" height="116" alt="image" src="https://github.com/user-attachments/assets/8665b753-eafe-4803-806a-18bc692a8280" />

  「あいあんほげ」を入力すると拒否される。このとき失敗にはならない。

  <img width="639" height="70" alt="image" src="https://github.com/user-attachments/assets/5d4250d2-2c2b-4965-980c-825094f86fea" />
